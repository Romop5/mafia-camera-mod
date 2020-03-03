#ifndef CABOUT_MODE_HPP
#define CABOUT_MODE_HPP
#include "modes/CGenericMode.hpp"
#include "common/imgui_utils.hpp"

class CAbout: public CGenericMode
{
    protected:
        ImFont* m_font;
    public:
        CAbout();

        virtual bool onVKKey(USHORT key) {return false;}
        virtual void onMouseMove(int x, int y) {}
        virtual void onMouseButtons(unsigned short buttons) {}
        virtual void onTick() {}
        virtual void onRender();

        virtual void activate() override;
        virtual void deactivate() override;
};
#endif
