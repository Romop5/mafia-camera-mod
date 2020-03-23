#include <psapi.h>
#include "tools.h"

namespace AngeloFramework
{
void InstallHook(void* original, void* newfunc, char* patch)
{
    // save bytes
    DWORD old;
    VirtualProtect((LPVOID)original, 5, PAGE_READWRITE, &old);
    memcpy(patch, (void*)original, 5);

    // jmp to new func
    *(BYTE*)original = 0xE9; // E9 stands for x86 JMP
    *(DWORD*)((DWORD)original + 1) = ((DWORD)newfunc - (DWORD)original) - 5;
    // restore protection
    VirtualProtect((LPVOID)original, 5, old, &old);
}

void UninstallHook(DWORD original, char* patch)
{
    // restore bytes
    DWORD old;
    VirtualProtect((LPVOID)original, 5, PAGE_READWRITE, &old);
    memcpy((void*)original, patch, 5);
    VirtualProtect((LPVOID)original, 5, old, &old);
}

// using arc tangens for 2 variables
float RotationTo180(float x, float y) { return atan2(x, y); }

float RotationTo360(float x, float y)
{
    float result = atan2(x, y) * RADIAN; // actually, this converts to degrees
    if (result < 0) {
        result = 360.0f - result * (-1);
    }
    return result;
}

float ASinTo180(float x) { return asin(x) * RADIAN; }

bool IsInRadius2D(int pointX, int pointY, int a, int b, int radius)
{
    if ((pointX >= a - radius && pointX <= a + radius) && (pointY >= b - radius && pointY <= b + radius))
        return true;
    return false;
}

bool IsInRect(int pointX, int pointY, int Rx, int Ry, int Rx2, int Ry2)
{
    if ((pointX >= Rx && pointX <= Rx2) && (pointY >= Ry && pointY <= Ry2))
        return true;
    return false;
}

char ConvertToASCII(unsigned short VK)
{
    unsigned char kbs[256];
    GetKeyboardState(kbs);
    char buff[4];
    int rre = ToAsciiEx(VK, MapVirtualKey(VK, MAPVK_VK_TO_VSC), kbs,
        (LPWORD)&buff, 0, GetKeyboardLayout(0));
    if (rre == 1) {
        return buff[0];
    }
    return NULL;
}

DWORD GetModuleBaseAddress(LPCSTR moduleName)
{
    MODULEINFO info;
    HMODULE h_Module = GetModuleHandle(moduleName);
    bool status = GetModuleInformation(GetCurrentProcess(), h_Module, &info,sizeof(MODULEINFO));
    if(status == 0)
        return 0;
    return reinterpret_cast<DWORD>(info.lpBaseOfDll);
}

DWORD GetAddressBasedOnOldModule(DWORD address, DWORD oldModule, DWORD newModule)
{
    return (address-oldModule)+newModule;
}
}