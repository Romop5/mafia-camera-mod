#ifndef C_CORE
#define C_CORE

#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <stdio.h>
#include <vector>

#include "common/tools.h"
#include "common/dynamic_module.hpp"

#include "sdk/CGenericGame.hpp"
#include "sdk/CGenericGraphics.hpp"
#include "sdk/CGenericInput.hpp"
#include "modes/CModeManager.hpp"


static void DetachIt(void* cor);

// CCore is singleton class used to control whole parts of mod
class CCore {
private:
    CModeManager p_CModeManager;
    DynamicModule m_gameWrapper;

    CGenericGame* m_game;
    bool m_isGUIacceptingInput;
    
public:
    CCore(): m_isGUIacceptingInput(false), m_game(nullptr) {}

    CGenericGame* getGame() { return this->m_game; }

    DynamicModule* getWrapper() { return &this->m_gameWrapper; }
    CModeManager* getModControl() { return &this->p_CModeManager; }

    void ModDetach()
    {
        if(m_TriggerUnload != NULL)
        {
            // Trigger unload by calling our superior module's callback
            m_TriggerUnload();
            this->getGame()->PrintDebugMessage("Calling unload callback");
        } else {
            this->getGame()->PrintDebugMessage("Trying our hack");
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

    //IDirect3DDevice8* m_originalD3DDriver;

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
