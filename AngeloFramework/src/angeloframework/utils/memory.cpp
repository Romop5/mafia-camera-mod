// Required due to psapi.h
#include <cmath>
#include <Windows.h>
#include <psapi.h>


namespace AngeloFramework
{
void InstallHook(DWORD original, DWORD newfunc, char* patch)
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


DWORD GetValueAt(DWORD address)
{
    DWORD result = 0;
    DWORD old;
    VirtualProtect((LPVOID)address, 4, PAGE_READWRITE, &old);
    result = *(DWORD*)((DWORD)address);
    // restore protection
    VirtualProtect((LPVOID)address, 4, old, &old);
    return result;
}

bool SetValueAt(DWORD address, DWORD value)
{
    DWORD old;
    VirtualProtect((LPVOID)address, 4, PAGE_READWRITE, &old);
    *(DWORD*)((DWORD)address) = value;
    // restore protection
    VirtualProtect((LPVOID)address, 4, old, &old);
    return true;
}

}