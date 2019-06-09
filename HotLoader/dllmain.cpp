// dllmain.cpp : Defines the entry point for the DLL application.

//#include <utilslib/logger.hpp>
#include <Windows.h>

using onRenderCallGame_t =  void (__stdcall *)(void*);
using onRenderCall_t =  void (__stdcall *)(void*, void*);
void on_tick();

static bool g_isLoaded = false;
static bool g_shouldUnload = false;
onRenderCallGame_t p_originalMethod = NULL;
HMODULE g_cameraModule = NULL;

struct LS3DEngine
{
    DWORD* *vtable;

    static inline LS3DEngine* getEngine() 
    {
        return reinterpret_cast<LS3DEngine*>(*(DWORD*)((*(DWORD*)0x0065115C)+0x10));
    }
};


/*
struct LS3DRenderer
{
    DWORD* *vtable;

    static inline LS3DRenderer* getRenderer() 
    {
        return reinterpret_cast<LS3DRenderer*>(*(DWORD*)0x00ED1238);
    }
};
*/

// 0056DD0F | FF51 54                  | call dword ptr ds:[ecx+54]                                | called from this point
void __stdcall onRender(void* p_engine)
{
    p_originalMethod(p_engine);
    // Call camera mode ontick
    on_tick();
/*
    LS3DRenderer* p_renderer = LS3DRenderer::getRenderer();
    onRenderCall_t p_renderRoutine = reinterpret_cast<onRenderCall_t>(p_renderer->vtable[0x1C >> 2]);
    // Call engine render routine
    p_renderRoutine(p_renderer, p_engine);
    // Call camera mode ontick
    on_tick();
    */
}

void onUnload()
{
    g_shouldUnload = true;
}

/*
 * Following function is responsible for:
 * - loading CameraMod
 * - reloading 
 */
void on_tick()
{
    if(!g_isLoaded)
    {
        g_cameraModule = LoadLibrary("CameraMod.dll");
        if(g_cameraModule == NULL)
        {
            MessageBox(NULL, TEXT("Failed to load CameraMod.dll"), TEXT("Failed"), MB_OK);
        }

        // Store unload triggerer 
        using trigger_t = void (*) (void (*callback)());
        trigger_t procAddress = reinterpret_cast<trigger_t>(GetProcAddress(g_cameraModule, TEXT("storeUnloaderCallback")));
        if(procAddress)
        {
            procAddress(&onUnload);
        } else {
            MessageBox(NULL, TEXT("Failed to set unload triggerer"), TEXT("Failed"), MB_OK);
        }
        g_isLoaded = true;
    }

    if(g_shouldUnload)
    {
        FreeLibrary(g_cameraModule);
        g_shouldUnload = false;
    }
}


BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        // HACK: following code replaces LS3D's engine class method which is responsible for
        // render() routine
        //MessageBox(NULL, TEXT("A"), TEXT("B"), MB_OK);
        p_originalMethod = reinterpret_cast<onRenderCallGame_t>(LS3DEngine::getEngine()->vtable[0x54>>2]);
        LPVOID vtableAddressSlot = &LS3DEngine::getEngine()->vtable[0x54>>2];

        DWORD protect;
        VirtualProtect(vtableAddressSlot, 4, PAGE_READWRITE, &protect);
        LS3DEngine::getEngine()->vtable[0x54>>2] = reinterpret_cast<DWORD*>(&onRender);
        VirtualProtect(vtableAddressSlot, 4, protect, &protect);
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        // Revert hook
        DWORD protect;
        LPVOID vtableAddressSlot = &LS3DEngine::getEngine()->vtable[0x54>>2];
        VirtualProtect(vtableAddressSlot, 4, PAGE_READWRITE, &protect);
        LS3DEngine::getEngine()->vtable[0x54>>2] = reinterpret_cast<DWORD*>(p_originalMethod);
        VirtualProtect(vtableAddressSlot, 4, protect, &protect);
        break;
    }
    return TRUE;
}

extern "C" __declspec(dllexport) void reload()
{
    if(g_isLoaded == 

    g_isLoaded = true;
}
