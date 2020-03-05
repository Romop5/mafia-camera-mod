#ifndef CGENERIC_MODE_HPP
#define CGENERIC_MODE_HPP
#include "Windows.h"
#include "game/CGenericGame.hpp"
#include "modes/CModeController.hpp"
#include <utilslib/logger.hpp>

// Fwd dec 
class CGame;

class CGenericMode
{
    protected:
        CGenericGame* m_gameController;
        CModeController m_modeController;
    public:
        CGenericMode(): m_gameController(NULL) {}
        void setGameDriver(CGenericGame* game) { this->m_gameController = game;}
        void setModeController(CModeController controller) {this->m_modeController = controller; }

        virtual bool onVKKey(USHORT key) {return false;}
        virtual void onMouseMove(int x, int y) {}
        virtual void onMouseButtons(unsigned short buttons) {}
        virtual void onTick() {}
        virtual void onRender() {}

        virtual void activate() {
            utilslib::Logger::getInfo() << "activate() generic" << std::endl;
        }
        virtual void deactivate() {}
};
#endif
