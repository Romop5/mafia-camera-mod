#ifndef TOOLS
#define TOOLS
#define WIN32_LEAN_AND_MEAN

#define RADIAN (180.0f /  3.14159f)
#define TORADIAN (3.14159f/180.0f)

#define POINTER_64 __ptr64
#include <Windows.h>

// hook original function to call newfunc and copy original 5 bytes to patch string
static void InstallHook(void* original, void* newfunc, char* patch)
{
	// save bytes 
	DWORD old;
	VirtualProtect((LPVOID)original, 5, PAGE_READWRITE, &old);
	memcpy(patch, (void*) original, 5);

	// jmp to new func
	*(BYTE*)original = 0xE9;			// E9 stands for x86 JMP
	*(DWORD*)((DWORD)original + 1) = ((DWORD)newfunc - (DWORD)original) - 5;
	// restore protection
	VirtualProtect((LPVOID)original, 5, old, &old);
}

static void UninstallHook(DWORD original, char* patch)
{
	// restore bytes
	DWORD old;
	VirtualProtect((LPVOID)original, 5, PAGE_READWRITE, &old);
	memcpy((void*)original, patch, 5);
	VirtualProtect((LPVOID)original, 5, old, &old);
}

// using arc tangens for 2 variables
static float RotationTo180(float x, float y)
{
	return atan2(x, y);
}

static float RotationTo360(float x, float y)
{
	float result = atan2(x, y)*RADIAN;	// actually, this converts to degrees
	if (result < 0)
	{
		result = 360.0f - result*(-1);
	}
	return result;
}

static float ASinTo180(float x)
{
	return asin(x)*RADIAN;
}

static bool IsInRadius2D(int pointX, int pointY, int a, int b, int radius)
{
	if ((pointX >= a - radius && pointX <= a + radius) && (pointY >= b - radius && pointY <= b + radius))
		return true;
	return false;
}

static bool IsInRect(int pointX, int pointY, int Rx, int Ry,int Rx2,int Ry2)
{
	if ((pointX >= Rx && pointX <= Rx2) && (pointY >= Ry && pointY <= Ry2))
		return true;
	return false;
}

static char	ConvertToASCII(unsigned short VK)
{
	unsigned char kbs[256];
	GetKeyboardState(kbs);
	char buff[4];
	int rre = ToAsciiEx(VK, MapVirtualKey(VK, MAPVK_VK_TO_VSC), kbs, (LPWORD)&buff, 0, GetKeyboardLayout(0));
	if (rre == 1)
	{
		return buff[0];
	}
	return NULL;
}
#endif
