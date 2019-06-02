#include "CRawInput.h"

// Processes WM_INPUT messages and calls onKey/mouse callbacks
//https://docs.microsoft.com/en-us/windows/desktop/inputdev/raw-input
void CRawInput::ProcessMessage(LPMSG message)
{
    UINT dwSize;

    GetRawInputData((HRAWINPUT)message->lParam, RID_INPUT, NULL, &dwSize,
        sizeof(RAWINPUTHEADER));
    LPBYTE lpb = new BYTE[dwSize];
    if (lpb) {

        if (GetRawInputData((HRAWINPUT)message->lParam, RID_INPUT, lpb, &dwSize,
                sizeof(RAWINPUTHEADER))
            == dwSize) {

            RAWINPUT* raw = (RAWINPUT*)lpb;

            if (raw->header.dwType == RIM_TYPEKEYBOARD) {

                if (!(raw->data.keyboard.Flags & RI_KEY_BREAK))
                {
                    auto virtualKeyCode = raw->data.keyboard.VKey;
                    // Call all registered onKeyPressed callbacks
                    for(auto callback: this->m_onKeyPressedHandlers)
                        callback(virtualKeyCode);
                }

            } else if (raw->header.dwType == RIM_TYPEMOUSE) {
                this->OnMouseTick(&raw->data.mouse);
            }
            delete[] lpb;
        }
    }
}

void CRawInput::OnMouseTick(RAWMOUSE* mouse)
{
    auto x = mouse->lLastX;
    auto y = mouse->lLastY;
    // Call all registered mouse movement handlers
    for(auto handler: this->m_onMouseMoveHandlers)
        handler(x,y);

    // Call all registered mouse movement handlers
    for(auto handler: this->m_onMouseButtonsUpdateHandlers)
        handler(mouse->usButtonFlags);
}
