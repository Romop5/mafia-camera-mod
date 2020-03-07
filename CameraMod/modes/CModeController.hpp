#ifndef CMODECONTROLLER_HPP
#define CMODECONTROLLER_HPP
#include <functional>
#include "CScene.hpp"

class CGraphics;

class CModeController
{
    public:
        std::function<void(bool)> m_blockInput;
        std::function<CScene&()> m_getScene;
        std::function<CGraphics*()> m_getGraphics;
        std::function<bool()> m_isGUIVisible;
};
#endif
