#include <utilslib/logger.hpp>
#include <Windows.h>
#include "angeloframework/angelo.hpp"

AngeloFramework::Framework framework;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);
        //MessageBox(NULL, "Angelo injected", "Angelo", 0);

 //       _asm int 3;

        framework.Load();
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}