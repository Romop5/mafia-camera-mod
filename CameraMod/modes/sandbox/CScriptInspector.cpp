#include "CScriptInspector.hpp"
#include <sstream>

void ScriptInspector::render()
{
    if(!m_script)
        return;
    if(!m_isVisible)
        return;

    std::stringstream ss;
    ss << "Name: " << m_script->getName();
    if(ImGui::Begin(ss.str().c_str(), &m_isVisible,0))
    {

        const char* pauseButtons[] = {"Running","Paused"};
        ImVec4 buttonColors[] = {ImVec4(0.0,1.0,0.0,1.0),ImVec4(1.0,0.0,0.0,1.0)};
        ImGui::PushStyleColor(ImGuiCol_Button,buttonColors[shouldBePaused()]);
        if(ImGui::Button(pauseButtons[shouldBePaused()]))
            forceState((shouldBePaused()?RUNNING:PAUSED));
        ImGui::PopStyleColor();
        
        ImGui::SameLine();
        if(ImGui::Button("Recompile"))
        {
            m_script->recompile();
            m_script->m_isSleeping = false;
        }
            
        ImGui::SameLine();
        if(ImGui::Button("NextStep"))
        {
            forceState(SINGLESTEP);
        }
        bool sleep = m_script->m_isSleeping;
        ImGui::Checkbox("IsSleeping",&sleep);
        m_script->m_isSleeping = sleep;

        std::stringstream txt;
        txt << "Name: " << m_script->getName() << " [ " << m_script << "]\n"
        << "Opcode ID: " << m_script->m_currentOpcodeID << "\n";
        ImGui::Text(txt.str().c_str());

        if(ImGui::Button("Toggle view/edit of source"))
        {
            m_toggleSourceEditing = !m_toggleSourceEditing;
        }
        auto frameSize = ImVec2(0,0);
        if(m_toggleSourceEditing)
        {
            if(ImGui::Button("Save"))
            {
                m_script->m_sourceCode = strdup(m_editBox.c_str());
            }
            ImGui::SameLine();
            if(ImGui::Button("Reload"))
            {
                m_editBox = m_script->m_sourceCode;
            }
            
            ImGui::SetNextWindowSize(ImVec2(0.0f,0.0f),0);
            ImGui::Utils::InputTextMultiline("Script:", &m_editBox);
        } else {
            ImGui::BeginChildFrame(1,frameSize,ImGuiWindowFlags_NoBackground);
            for(size_t i = 0; i < m_script->getOpcodesCount(); i++)
            {
                auto opcode = m_script->getOpcodeAtPosition(i);
                auto command = opcode->getName();
                if(!command)
                    command = "UNK";

                std::stringstream txt;
                txt << opcode << " - " << command;
                bool isMarkedLine = (m_script->m_currentOpcodeID == i);
                auto color = (isMarkedLine)?ImVec4(1.0,0.0,0.0,1.0):ImVec4(1.0,1.0,1.0,1.0);
                
                bool hasBreakpoint = (m_breakPoints.count(i) > 0);
                auto bpColor = (hasBreakpoint)?ImGui::Utils::COLOR_RED:ImGui::Utils::COLOR_WHITE;
                ImGui::PushID(i);
                auto isPushed = ImGui::Utils::TextColoredButton(bpColor,"-");
                ImGui::PopID();
                if(isPushed)
                {
                    if(hasBreakpoint)
                    {
                        m_breakPoints.erase(i);
                    } else {
                        m_breakPoints.insert(i);
                    }
                }
                ImGui::SameLine();
                ImGui::TextColored(color,txt.str().c_str());
                // Display arguments
                std::stringstream argumentString;
                for(size_t i = 0; i < opcode->getArgumentCount(); i++)
                {
                    auto argument = opcode->getArgumentAsString(i);
                    if(i > 0)
                        argumentString << ", ";
                    argumentString << argument;
                }
                ImGui::SameLine();
                ImGui::TextColored(color,argumentString.str().c_str());
            }
            ImGui::EndChildFrame();
        }
    }    
    ImGui::End();
}