// dllmain.cpp : Defines the entry point for the DLL application.

#include "CCore.h"
#include <Windows.h>

CCore		*core = NULL;

BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		//CHooks hook;
		//hook.ApplyThem();
		DisableThreadLibraryCalls(hModule);
		CCore* coreInstance = new CCore();
		core = coreInstance;
		core->getHook()->ApplyThem();
		core->SetModule(hModule);
		core->getModControl()->Init();
		break;
	}
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		core->getHook()->UnloadThem();
		break;
	}
	return TRUE;
}




