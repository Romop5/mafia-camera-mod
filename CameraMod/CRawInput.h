#ifndef C_RAWINPUT
#define C_RAWINPUT
#include <utilslib/callbacks/callbacks.hpp>
#include "CCore.h"

using func_key_handler = std::function<void(USHORT)>;

class CRawInput {
private:
    bool isClickedLeft;
    bool isClickedRight;

public:
    void ProcessMessage(LPMSG message);
    void OnVKKey(USHORT vk);
    void OnMouseTick(RAWMOUSE* mouse);
    LONG x, y;
    utilslib::Callbacks<func_key_handler> m_keyHandlers;
};

#endif
