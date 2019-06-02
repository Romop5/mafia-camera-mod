#ifndef CGENERIC_MODE_HPP
#define CGENERIC_MODE_HPP
#include "Windows.h"
#include "CGame.h"
class CGenericMode
{
    private:
        CGame* m_gameController;
    public:
        CGenericMode(): m_gameController(NULL) {}
        void setGameDriver(CGame& game) { this->m_gameController = &game;}
        virtual bool onVKKey(USHORT key) {return false;}
        virtual void onMouseMove(int x, int y) {}
        virtual void onMouseButtons(unsigned short buttons) {}
        virtual void onTick() {}
};
#endif
