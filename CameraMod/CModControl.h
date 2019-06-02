#ifndef C_MODCONTROL
#define C_MODCONTROL
#include <vector>
#include "CGenericMode.hpp"
#include "CGame.h"

class CModControl {
private:
    std::vector<CGenericMode*> m_modes;
    size_t m_currentModeIndex;
    CGenericMode* m_currentMode;
public:
    CModControl();
    void InitializeModes(CGame* game);
    bool IsActive();
    bool OnVKKey(USHORT key);
    void OnMouseMove(int x, int y);
    void OnMouseButtons(unsigned short buttons);
    void Render();

    bool isFreeCam();

    void switchToNextMode();

};

#endif
