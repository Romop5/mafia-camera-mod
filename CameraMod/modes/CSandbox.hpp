#ifndef CSANDBOX_MODE_HPP
#define CSANDBOX_MODE_HPP
#include "Windows.h"
#include "game/CGenericGame.hpp"
#include "modes/CModeController.hpp"
#include "modes/CGenericMode.hpp"
#include <utilslib/logger.hpp>
#include "version.hpp"
#include "common/imgui_utils.hpp"
#include <sstream>
#include <set>

using originalDelete_t = void __fastcall (Script* script);
using originalUpdate_t = void __fastcall (Script* script,DWORD EDX, DWORD deltaTime);

originalDelete_t* g_originalDeleter = nullptr;
originalUpdate_t* g_originalUpdater = nullptr;

originalDelete_t* g_originalDeleter1 = nullptr;
originalUpdate_t* g_originalUpdater1 = nullptr;


class ScriptInspector
{
    std::string m_editBox;
    Script* m_script;
    bool toggleSourceEditing = true;
    bool shouldForcefullyPause = false;
    public:
    ScriptInspector(): m_script(nullptr) {}
    ScriptInspector(Script* owner): m_script(owner) {
        m_editBox.reserve(1000);
        m_editBox = m_script->m_sourceCode;
    }
    bool shouldBePaused() const { return shouldForcefullyPause; }
    void render()
    {
        if(!m_script)
            return;
        std::stringstream ss;
        ss << "Name: " << m_script;
        ImGui::Begin(ss.str().c_str(), nullptr,0);

        const char* pauseButtons[] = {"Pause","Unpause"};
        if(ImGui::Button(pauseButtons[shouldForcefullyPause]))
           this->shouldForcefullyPause = !shouldForcefullyPause;

        ImGui::SameLine();
        if(ImGui::Button("Recompile"))
            m_script->recompile();
            
        ImGui::SameLine();
        if(ImGui::Button("NextStep"))
        {
            m_script->m_nextOpcodeID++;
            m_script->m_currentOpcodeID = m_script->m_nextOpcodeID;
            m_script->forceRun();
        }
        int sleep = m_script->m_isSleeping;
        ImGui::InputInt("IsSleeping",&sleep);
        m_script->m_isSleeping = sleep;

        std::stringstream txt;
        txt << "Name: " << m_script->getName() << "\n"
        << "Opcode ID: " << m_script->m_currentOpcodeID << "\n";
        ImGui::Text(txt.str().c_str());

        if(ImGui::Button("Toggle view/edit of source"))
        {
            toggleSourceEditing = !toggleSourceEditing;
        }
        if(toggleSourceEditing)
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
            //std::stringstream txt;
            //txt << "Script: " << m_script->getSource();
            //ImGui::Text(txt.str().c_str());
            for(size_t i = 0; i < m_script->getOpcodesCount(); i++)
            {
                auto opcode = m_script->getOpcodeAtPosition(i);
                auto command = opcode->getName();
                if(!command)
                    command = "UNK";

                std::stringstream txt;
                if(m_script->m_currentOpcodeID == i)
                    txt << "== ";
                txt << opcode << " - " << command;
                ImGui::Text(txt.str().c_str());
            }
        }
        
        ImGui::End();
    }
};

std::set<Script*> g_perFrameScripts;
std::map<Script*,ScriptInspector> g_perFrameScriptsObjects;

CGame* g_game = nullptr;

class CSandbox: public CGenericMode
{
    protected:
        ImFont* m_font;
    public:
        CSandbox(){
            ImGuiIO& io = ImGui::GetIO();
            m_font = io.Fonts->AddFontDefault();
            m_font = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 50);
        }

        virtual bool onVKKey(USHORT key) {return false;}
        virtual void onMouseMove(int x, int y) {}
        virtual void onMouseButtons(unsigned short buttons) {}
        virtual void onTick() {}
        virtual void onRender() {

            ImGui::ShowDemoWindow();
            
            auto mafiaGame = reinterpret_cast<CGame*>(this->m_gameController);
            auto machine = mafiaGame->getScriptMachine();

            bool shouldRenderOverlay = true;
            ImGui::SetNextWindowPosCenter();
            ImGui::SetNextWindowSize(ImVec2(400,200));
            ImGui::Begin("Example: overlay", &shouldRenderOverlay,0);

            std::string scripts = std::string("Count: ") + std::to_string(machine->getCountOfScripts()); 
            ImGui::Text(scripts.c_str());
            auto numberOfScripts = machine->getCountOfScripts();

            auto min = [](size_t a, size_t b) { return (a>b)?b:a;};
            numberOfScripts = min(10, numberOfScripts);
            
            std::stringstream pool;
            pool << "Pool: " << machine->m_scriptsPoolStart << " - " << machine->m_scriptsPoolEnd;
            ImGui::Text(pool.str().c_str());
            for(size_t i = 0; i < numberOfScripts; i++)
            {
                if(i > 0)
                    ImGui::Separator();
                auto script = machine->getScriptAtIndex(i);
                ImGui::Text(script->m_name);
                ImGui::Text(script->m_sourceCode);
                std::stringstream out;
                out << "Opcode: " << script->m_currentOpcodeID;
                ImGui::Text(out.str().c_str());
            }

            ImGui::Separator();
            std::stringstream ss;
            ss << "Script count: " << g_perFrameScripts.size();
            ImGui::Text(ss.str().c_str());
            ImGui::End();

            for(auto& scriptWindow: g_perFrameScriptsObjects)
            {
                scriptWindow.second.render();
            }
        }

        virtual void activate() override {
            utilslib::Logger::getInfo() << "activate() ABOUT" << std::endl;

            //using onRenderCallGame_t =  void (__stdcall *)(Script* this, DWORD deltaTime);

            g_game = reinterpret_cast<CGame*>(this->m_gameController);
            g_originalDeleter = reinterpret_cast<originalDelete_t*>(Script::getVtable()->swapFunction(0,(DWORD*) scriptDelete));
            g_originalUpdater = reinterpret_cast<originalUpdate_t*>(Script::getVtable()->swapFunction(0x1C/4,(DWORD*) scriptUpdate));

            
            g_originalDeleter1 = reinterpret_cast<originalDelete_t*>(Script::getVtable1()->swapFunction(0,(DWORD*) scriptDelete1));
            g_originalUpdater1 = reinterpret_cast<originalUpdate_t*>(Script::getVtable1()->swapFunction(0x1C/4,(DWORD*) scriptUpdate1));
        }

        static void __thiscall scriptUpdate(Script* script, DWORD deltaTime)
        {
            g_perFrameScripts.insert(script);
            if(g_perFrameScriptsObjects.count(script) == 0)
            {
                g_perFrameScriptsObjects[script] = ScriptInspector(script);
            }
            if(!g_perFrameScriptsObjects[script].shouldBePaused())
                g_originalUpdater(script,0,deltaTime);
        }

        static void __thiscall scriptDelete(Script* script)
        {
            g_perFrameScripts.erase(script);
            g_originalDeleter(script);
        }

         static void __thiscall scriptUpdate1(Script* script, DWORD deltaTime)
        {
            g_perFrameScripts.insert(script);
            if(g_perFrameScriptsObjects.count(script) == 0)
            {
                g_perFrameScriptsObjects[script] = ScriptInspector(script);
            }
            if(!g_perFrameScriptsObjects[script].shouldBePaused())
                g_originalUpdater1(script,0,deltaTime);
        }

        static void __thiscall scriptDelete1(Script* script)
        {
            g_perFrameScripts.erase(script);
            g_originalDeleter1(script);
        }

        virtual void deactivate() override {
            reinterpret_cast<originalDelete_t*>(Script::getVtable()->swapFunction(0,(DWORD*) g_originalDeleter));
            reinterpret_cast<originalUpdate_t*>(Script::getVtable()->swapFunction(0x1C/4,(DWORD*) g_originalUpdater));

            reinterpret_cast<originalDelete_t*>(Script::getVtable1()->swapFunction(0,(DWORD*) g_originalDeleter1));
            reinterpret_cast<originalUpdate_t*>(Script::getVtable1()->swapFunction(0x1C/4,(DWORD*) g_originalUpdater1));
        
            g_perFrameScripts.clear();
        }
};
#endif
