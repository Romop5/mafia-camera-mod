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

            auto mafiaGame = reinterpret_cast<CGame*>(this->m_gameController);
            auto machine = mafiaGame->getScriptMachine();

            bool shouldRenderOverlay = true;
            ImGui::SetNextWindowPosCenter();
            ImGui::SetNextWindowSize(ImVec2(400,200));
            ImGui::Begin("Example: Simple overlay", &shouldRenderOverlay,0);

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
            ImGui::End();
        }

        virtual void activate() override {
            utilslib::Logger::getInfo() << "activate() ABOUT" << std::endl;
            this->m_modeController.m_blockInput(true);
        }
        virtual void deactivate() override {
            this->m_modeController.m_blockInput(false);
        }
};
#endif
