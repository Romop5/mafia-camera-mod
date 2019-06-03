#ifndef CMODECONTROLLER_HPP
#define CMODECONTROLLER_HPP
#include <functional>

class CModeController
{
    public:
        std::function<void(bool)> m_blockInput;
};
#endif
