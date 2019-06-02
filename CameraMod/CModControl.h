#include "CCore.h"

#ifndef C_MODCONTROL
#define C_MODCONTROL

enum ControlStates {
    CMOD_WALKING,
    CMOD_FREECAMERA, 
};
class CModControl {
private:
    ControlStates m_currentState;
public:
    CModControl();
    bool IsActive();
    bool OnVKKey(USHORT key);
    void OnMouseMove(int x, int y);
    void OnMouseButtons(unsigned short buttons);
    void Render();

    bool isFreeCam();

};

#endif
