#ifndef CABOUT_MODE_HPP
#define CABOUT_MODE_HPP
#include "Windows.h"
#include "game/CGenericGame.hpp"
#include "modes/CModeController.hpp"
#include "modes/CGenericMode.hpp"
#include <utilslib/logger.hpp>
#include "version.hpp"

class CAbout: public CGenericMode
{
    protected:
        CGenericGame* m_gameController;
        CModeController m_modeController;
    public:
        CAbout(): m_gameController(NULL) {}
        void setGameDriver(CGame* game) { this->m_gameController = game;}
        void setModeController(CModeController controller) {this->m_modeController = controller; }

        virtual bool onVKKey(USHORT key) {return false;}
        virtual void onMouseMove(int x, int y) {}
        virtual void onMouseButtons(unsigned short buttons) {}
        virtual void onTick() {}
        virtual void onRender() {
            bool shouldRenderOverlay = true;
            ImGui::Begin("Example: Simple overlay", &shouldRenderOverlay, 0 | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav);
            ImGui::Text("Camera Mod - created by Romop5");
            ImGui::Text("Version: %s", getVersionString().c_str());
            ImGui::End();
        }

        virtual void activate() {
            utilslib::Logger::getInfo() << "activate() ABOUT" << std::endl;
        }
        virtual void deactivate() {}
};
#endif
