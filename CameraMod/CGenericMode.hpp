#ifndef CGENERIC_MODE_HPP
#define CGENERIC_MODE_HPP
#include "Windows.h"
#include "CGame.h"
#include "CModeController.hpp"
#include <utilslib/logger.hpp>
class CGenericMode
{
    protected:
        CGame* m_gameController;
        CModeController m_modeController;
    public:
        CGenericMode(): m_gameController(NULL) {}
        void setGameDriver(CGame* game) { this->m_gameController = game;}
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
