#ifndef C_MODCONTROL
#define C_MODCONTROL
#include <vector>
#include <memory>
#include "CGenericMode.hpp"
#include "CCoreController.hpp"
#include "game/CGame.h"

enum BlockStatus:size_t
{
    BLOCK_GUI = 0b1,
    BLOCK_MODE = 0b10, 
};

class CModeManager {
private:
    std::vector<std::pair<CGenericMode*,std::string>> m_modes;
    size_t m_currentModeIndex;
    CGenericMode* m_currentMode;
    CCoreController m_coreController;
    size_t m_InputBlockingStatus;

    bool m_isGUIVisible;

    CScene m_scene;

    void RenderMainMenu();
    void AddMode(CGenericMode*, const std::string name = "Unknown mode");
public:
    CModeManager();
    ~CModeManager();
    void setCoreController(const CCoreController controller);
    void InitializeModes(CGame* game);

    bool IsActive();
    bool OnVKKey(USHORT key);
    void OnMouseMove(int x, int y);
    void OnMouseButtons(unsigned short buttons);
    void Render();

    bool isFreeCam();

    void switchToNextMode();
    void switchToMode(size_t id);
    void updateBlocking(bool shouldBlock, BlockStatus type);
    
    void turnGUIstate(bool shouldBeOn);

};

#endif
