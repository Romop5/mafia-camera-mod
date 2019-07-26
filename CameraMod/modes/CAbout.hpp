#ifndef CABOUT_MODE_HPP
#define CABOUT_MODE_HPP
#include "Windows.h"
#include "game/CGenericGame.hpp"
#include "modes/CModeController.hpp"
#include "modes/CGenericMode.hpp"
#include <utilslib/logger.hpp>
#include "version.hpp"
#include "common/imgui_utils.hpp"

class CAbout: public CGenericMode
{
    protected:
        ImFont* m_font;
    public:
        CAbout(){
            ImGuiIO& io = ImGui::GetIO();
            m_font = io.Fonts->AddFontDefault();
            m_font = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 50);
        }

        virtual bool onVKKey(USHORT key) {return false;}
        virtual void onMouseMove(int x, int y) {}
        virtual void onMouseButtons(unsigned short buttons) {}
        virtual void onTick() {}
        virtual void onRender() {

            bool shouldRenderOverlay = true;
            ImGui::SetNextWindowPosCenter();
            ImGui::SetNextWindowSize(ImVec2(400,200));
            ImGui::Begin("Example: Simple overlay", &shouldRenderOverlay, 0 | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);

            ImGui::Dummy(ImVec2(1,30));
            ImGui::PushFont(m_font);
            ImGui::Utils::TextColoredCentered(ImVec4(1.0f,0.0f,0.0f,1.0f), "Camera Mod v2");
            ImGui::PopFont();
            ImGui::Dummy(ImVec2(1,20));
            ImGui::Utils::TextColoredCentered(ImVec4(0.9f,0.9f,0.9f,1.0f), "Created by Roman 'Romop5' Dobias");
            ImGui::Separator();
            ImGui::Utils::TextCentered("Build: %s", getVersionString().c_str());
           ImGui::Utils::TextColoredCentered(ImVec4(0.6f,0.6f,0.6f,1.0f),"https://github.com/Romop5/mafia-camera-mod");
            ImGui::Separator();
            ImGui::Utils::TextCentered("Press F2 to control GUI");
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
