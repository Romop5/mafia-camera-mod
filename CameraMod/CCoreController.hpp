#ifndef CORECONTROLLER_HPP
#define CORECONTROLLER_HPP
#include <functional>

class CGraphics;

class CCoreController
{
    public:
        std::function<void(bool)> m_blockGameInput;
        std::function<void(bool)> m_blockGUIInput;
        std::function<void(bool)> m_hideGUI;
        std::function<void(void)> m_exitMod;
        std::function<CGraphics*()> m_getGraphics;
};
#endif
