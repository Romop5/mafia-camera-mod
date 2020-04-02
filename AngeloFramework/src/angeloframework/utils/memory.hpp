#ifndef ANGELOFRAMEWORK_UTILS_MEMORY
#define ANGELOFRAMEWORK_UTILS_MEMORY

#define POINTER_64 __ptr64
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace AngeloFramework
{
// hook original function to call newfunc and copy original 5 bytes to patch
// string
void InstallHook(DWORD original, DWORD newfunc, char* patch);

void UninstallHook(DWORD original, char* patch);

char ConvertToASCII(unsigned short VK);

DWORD GetModuleBaseAddress(LPCSTR moduleName);

DWORD GetAddressBasedOnOldModule(DWORD address, DWORD oldModule, DWORD newModule);

DWORD GetValueAt(DWORD address);
bool SetValueAt(DWORD address, DWORD value);
}
#endif
