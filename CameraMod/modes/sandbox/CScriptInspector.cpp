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
        }
            
        ImGui::SameLine();
        if(m_state == PAUSED)
        {
            if(ImGui::Button("NextStep"))
            {
                forceState(SINGLESTEP);
            }
        }

        std::stringstream txt;
        txt << "Address [ " << m_script << "]"
         << "| Vars: " << m_script->m_fltArrayLength 
         << "| Frames: " << m_script->m_frameArrayLength 
         << "| Actors: " << m_script->m_actorArrayLength;
        ImGui::Text(txt.str().c_str());

        if (m_script->hasVariables() && ImGui::TreeNode("Variables"))
        {
            for(size_t i = 0; i < m_script->m_fltArrayLength; i++)
            {
                ImGui::PushID(i);
                ImGui::Text(std::to_string(i).c_str());
                ImGui::SameLine();
                ImGui::DragFloat("##var",&m_script->m_fltArray[i]);
                ImGui::PopID();
            }

            ImGui::TreePop();
            ImGui::Separator();
        }

        if (m_script->hasFrames() && ImGui::TreeNode("Frames"))
        {
            // List frames
            std::stringstream frames;
            frames << "Frames:\n"; 
            for(size_t i = 0; i < m_script->m_frameArrayLength; i++)
            {
                auto frame = m_script->m_frameArray[i];
                if(frame)
                {
                    frames << "-"<<i<<"-" << frame->frameName << "\n";
                }
            }
            ImGui::Text(frames.str().c_str());
            ImGui::TreePop();
            ImGui::Separator();
        }

        if (m_script->hasActors() && ImGui::TreeNode("Actors"))
        {
            // List Actors
            std::stringstream actors;
            actors << "Actors:\n"; 
            for(size_t i = 0; i < m_script->m_actorArrayLength; i++)
            {
                auto object = m_script->m_actorArray[i];
                if(object)
                {
                    actors << "-"<<i<<"- " << object->getObjectType() << " - "
                    << object->frame->frameName;
                    if(object->hasModel())
                        actors << " (" <<  object->frame->frameModel << ")";
                    actors << "\n";
                }
            }
            ImGui::Text(actors.str().c_str());
            ImGui::TreePop();
            ImGui::Separator();
        }


        if(ImGui::Button("Toggle view/edit of source"))
        {
            m_toggleSourceEditing = !m_toggleSourceEditing;
        }
        auto frameSize = ImVec2(0,0);
        if(m_toggleSourceEditing)
        {
            if(ImGui::Button("Save"))
            {
                m_script->m_sourceCode = strdup(m_editor.GetText().c_str());
            }
            ImGui::SameLine();
            if(ImGui::Button("Reload"))
            {
                m_editor.SetText(m_script->m_sourceCode);
            }
            m_editor.Render("Editor");
        } else {
            ImGui::BeginChildFrame(1,frameSize,ImGuiWindowFlags_NoBackground);
            for(size_t i = 0; i < m_script->getOpcodesCount(); i++)
            {
                auto opcode = m_script->getOpcodeAtPosition(i);
                auto command = opcode->getName();
                if(!command)
                    command = "UNK";

                std::stringstream txt;
                txt << command;
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