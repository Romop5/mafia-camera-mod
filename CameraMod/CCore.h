#ifndef C_CORE
#define C_CORE

#define WIN32_LEAN_AND_MEAN
#include <iostream>
#include <stdio.h>
#include <vector>

#include "tools.h"
// include classes

#include "CGUI.h"
#include "CGame.h"
#include "CGraphics.h"
#include "CHooks.h"
#include "CModControl.h"
#include "CRawInput.h"

static void DetachIt(void* cor);

// CCore is singleton class used to control whole parts of mod
class CCore {
private:
    CHooks p_CHook;
    CGraphics p_CGraphics;
    CRawInput p_CRawinput;
    CGame p_CGame;
    CModControl p_CModControl;
    CGUI p_CGUI;

    bool p_isGamePhys;

public:
    CCore() { this->p_isGamePhys = true; }
    bool IsGamePhysicRunning() { return this->p_isGamePhys; }
    void SetGamePhysic(bool state) { this->p_isGamePhys = state; }
    CHooks* getHook() { return &this->p_CHook; }
    CGraphics* getGraphics() { return &this->p_CGraphics; }

    CRawInput* getRawInput() { return &this->p_CRawinput; }

    CGame* getGame() { return &this->p_CGame; }

    CModControl* getModControl() { return &this->p_CModControl; }

    CGUI* getGUI() { return &this->p_CGUI; }

    void ModDetach()
    {

        CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)DetachIt, this, 0,
            0); // hlavne vlakno
            // FreeLibrary(this->p_DLL);
    }

    void SetModule(HMODULE dll) { this->p_DLL = dll; }

    HMODULE p_DLL;
};
void DetachIt(void* cor)
{
    CCore* corePointer = (CCore*)cor;
    FreeLibraryAndExitThread(corePointer->p_DLL, 0);
}

#endif