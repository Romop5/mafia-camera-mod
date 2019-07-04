#ifndef C_CORE
#define C_CORE

#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <stdio.h>
#include <vector>

#include "common/tools.h"

#include "game/CGame.h"
#include "game/CGraphics.h"
#include "game/CHooks.h"
#include "game/CRawInput.h"
#include "modes/CModeManager.hpp"

static void DetachIt(void* cor);

// CCore is singleton class used to control whole parts of mod
class CCore {
private:
    CHooks p_CHook;
    CGraphics p_CGraphics;
    CRawInput p_CRawinput;
    CGame p_CGame;
    CModeManager p_CModeManager;

    bool p_isGamePhys;

    bool m_isGUIacceptingInput;
    

public:
    CCore(): m_isGUIacceptingInput(false) { this->p_isGamePhys = true;}
    bool IsGamePhysicRunning() { return this->p_isGamePhys; }
    void SetGamePhysic(bool state) { this->p_isGamePhys = state; }
    CHooks* getHook() { return &this->p_CHook; }
    CGraphics* getGraphics() { return &this->p_CGraphics; }

    CRawInput* getRawInput() { return &this->p_CRawinput; }

    CGame* getGame() { return &this->p_CGame; }

    CModeManager* getModControl() { return &this->p_CModeManager; }

    void ModDetach()
    {
        if(m_TriggerUnload != NULL)
        {
            // Trigger unload by calling our superior module's callback
            m_TriggerUnload();
            this->getGame()->writeToConsole(CGame::COLOR_RED, "Calling unload callback");
        } else {
            this->getGame()->writeToConsole(CGame::COLOR_RED, "Trying our hack");
            // If not available, falling back to our unloading hack
            // which basically starts up a new thread and deload us
            // However, this can cause crashes when the main thread
            // is inside this DLL code in the moment of unloading
            CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DetachIt, this, 0, 0);
        }
    }

    void SetModule(HMODULE dll) { this->p_DLL = dll; }

    HMODULE p_DLL;

    bool Initialize();
    bool Unload();

    IDirect3DDevice8* m_originalD3DDriver;

    // This function is called by CCore when unloading is desired
    // If not specified, skipped
    std::function<void(void)> m_TriggerUnload;
};

void DetachIt(void* cor)
{
    CCore* corePointer = (CCore*)cor;
    FreeLibraryAndExitThread(corePointer->p_DLL, 0);
}
#endif
