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

std::set<Script*> g_perFrameScripts;

class ScriptInspector
{
    std::map<Script*,std::string> m_editBoxes;
    public:
    void render()
    {
        for(auto& script: g_perFrameScripts)
        {
            std::stringstream ss;
            ss << "Name: " << script;
            ImGui::Begin(ss.str().c_str(), nullptr,0);
            if(ImGui::Button("Recompile"))
                script->recompile();
                
            if(ImGui::Button("NextStep"))
            {
                script->m_nextOpcodeID++;
                script->m_currentOpcodeID = script->m_nextOpcodeID;
                script->forceRun();
            }
            int sleep = script->m_isSleeping;
            ImGui::InputInt("IsSleeping",&sleep);
            script->m_isSleeping = sleep;

            std::stringstream txt;
            txt << "Name: " << script->getName() << "\n"
            << "Opcode ID: " << script->m_currentOpcodeID << "\n";
            ImGui::Text(txt.str().c_str());

            static bool toggleSourceEditing = false;
            if(m_editBoxes.count(script) == 0)
            {
                m_editBoxes[script] = script->m_sourceCode;
                m_editBoxes[script].reserve(1000);
            }
            if(ImGui::Button("Toggle view/edit of source"))
            {
                toggleSourceEditing = !toggleSourceEditing;
            }
            if(toggleSourceEditing)
            {
                if(ImGui::Button("Save"))
                {
                    script->m_sourceCode = strdup(m_editBoxes[script].c_str());
                }
                ImGui::SameLine();
                if(ImGui::Button("Reload"))
                {
                    m_editBoxes[script] = script->m_sourceCode;
                }
                ImGui::Utils::InputTextMultiline("Script:", &m_editBoxes[script]);
            } else {
                std::stringstream txt;
                txt << "Script: " << script->getSource();
                ImGui::Text(txt.str().c_str());
            }
            
            ImGui::End();
        }
    }
};

/*struct ScriptCommand
{

};
*/
//std::map<Script*,ScriptCommand> g_perScriptCommand;

CGame* g_game = nullptr;

class CSandbox: public CGenericMode
{
    protected:
        ImFont* m_font;
        ScriptInspector m_inspector;
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

            m_inspector.render();
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
