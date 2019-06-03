#ifndef C_MODCONTROL
#define C_MODCONTROL
#include <vector>
#include "CGenericMode.hpp"
#include "CCoreController.hpp"
#include "CGame.h"

enum BlockStatus:size_t
{
    BLOCK_GUI = 0b1,
    BLOCK_MODE = 0b10, 
};

class CModeManager {
private:
    std::vector<CGenericMode*> m_modes;
    size_t m_currentModeIndex;
    CGenericMode* m_currentMode;
    CCoreController m_coreController;
    size_t m_InputBlockingStatus;

    bool m_isGUIVisible;


public:
    CModeManager();
    void setCoreController(const CCoreController controller);
    void InitializeModes(CGame* game);

    bool IsActive();
    bool OnVKKey(USHORT key);
    void OnMouseMove(int x, int y);
    void OnMouseButtons(unsigned short buttons);
    void Render();

    bool isFreeCam();

    void switchToNextMode();
    void updateBlocking(bool shouldBlock, BlockStatus type);
    
    void turnGUIstate(bool shouldBeOn);
};

#endif
