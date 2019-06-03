#ifndef CORECONTROLLER_HPP
#define CORECONTROLLER_HPP
#include <functional>

class CCoreController
{
    public:
        std::function<void(bool)> m_blockGameInput;
        std::function<void(bool)> m_blockGUIInput;
        std::function<void(bool)> m_hideGUI;
};
#endif
