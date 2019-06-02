#ifndef CGENERIC_MODE_HPP
#define CGENERIC_MODE_HPP
#include "Windows.h"
#include "CGame.h"
#include <utilslib/logger.hpp>
class CGenericMode
{
    protected:
        CGame* m_gameController;
    public:
        CGenericMode(): m_gameController(NULL) {}
        void setGameDriver(CGame* game) { this->m_gameController = game;}
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
