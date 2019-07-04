#ifndef CMODECONTROLLER_HPP
#define CMODECONTROLLER_HPP
#include <functional>
#include "CScene.hpp"

class CModeController
{
    public:
        std::function<void(bool)> m_blockInput;
        std::function<CScene&()> m_getScene;
};
#endif