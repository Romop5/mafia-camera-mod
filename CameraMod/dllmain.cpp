// dllmain.cpp : Defines the entry point for the DLL application.

#include "CCore.h"
#include <utilslib/logger.hpp>
#include <Windows.h>

CCore* core = NULL;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        // CHooks hook;
        // hook.ApplyThem();
        DisableThreadLibraryCalls(hModule);
        utilslib::Logger::getLogger()->openAs("cameramod.log");
        utilslib::Logger::getInfo() << "CameraMod injected !" << std::endl;
        //MessageBox(NULL, "LOL", "WTF", MB_OK);
        CCore* coreInstance = new CCore();
        core = coreInstance;

        if(core->Initialize())
        {
             utilslib::Logger::getInfo() << "CameraMod initialized correctly !" << std::endl;
        }
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        core->Unload();
        break;
    }
    return TRUE;
}
