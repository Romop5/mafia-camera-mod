#ifndef C_RAWINPUT
#define C_RAWINPUT
#include <utilslib/callbacks.hpp>
// For RAWMOUSE/Raw input
#include <Windows.h>

// Virtual Key (VK) handler type
using func_key_handler = std::function<bool(unsigned short)>;

// Mouse movement handler type
using mouse_handler = std::function<void(long, long)>;

// Mouse buttons handler type
using mouse_buttons_handler = std::function<void(unsigned short)>;

class CRawInput {
private:
    bool isClickedLeft;
    bool isClickedRight;

    void OnMouseTick(RAWMOUSE* mouse);

public:
    bool ProcessMessage(LPMSG message);
    utilslib::Callbacks<func_key_handler> m_onKeyPressedHandlers;
    utilslib::Callbacks<mouse_handler> m_onMouseMoveHandlers;
    utilslib::Callbacks<mouse_buttons_handler> m_onMouseButtonsUpdateHandlers;
};

#endif
