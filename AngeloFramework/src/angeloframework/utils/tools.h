#ifndef ANGELOFRAMEWORK_TOOLS
#define ANGELOFRAMEWORK_TOOLS

#define RADIAN (180.0f / 3.14159f)
#define TORADIAN (3.14159f / 180.0f)

#define POINTER_64 __ptr64
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace AngeloFramework
{
// hook original function to call newfunc and copy original 5 bytes to patch
// string
void InstallHook(void* original, void* newfunc, char* patch);

void UninstallHook(DWORD original, char* patch);

// using arc tangens for 2 variables
float RotationTo180(float x, float y);

float RotationTo360(float x, float y);
float ASinTo180(float x);

bool IsInRadius2D(int pointX, int pointY, int a, int b, int radius);

bool IsInRect(int pointX, int pointY, int Rx, int Ry, int Rx2, int Ry2);

char ConvertToASCII(unsigned short VK);

DWORD GetModuleBaseAddress(LPCSTR moduleName);

DWORD GetAddressBasedOnOldModule(DWORD address, DWORD oldModule, DWORD newModule);
}
#endif
