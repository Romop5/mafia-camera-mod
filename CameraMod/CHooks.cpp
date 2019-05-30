/*#include "CHooks.h"
*/

#define POINTER_64 __ptr64

#include "CDirect3DDevice8Proxy.h"

#include "CCore.h"

extern CCore* core;

DWORD peekOriginal = NULL;
char peekOriginalPatch[5];
char peekOriginalPatchGamePhys[5];
char peekOriginalPatchGamePhys2[5];
char peekOriginalPatchGameSpeed[5];
IDirect3DDevice8* pointer, *original;

BOOL WINAPI myPeekMessage(
	_Out_     LPMSG lpMsg,
	_In_opt_  HWND hWnd,
	_In_      UINT wMsgFilterMin,
	_In_      UINT wMsgFilterMax,
	_In_      UINT wRemoveMsg
	)
{
	UninstallHook(peekOriginal, peekOriginalPatch);
	// call func
	bool result = PeekMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg) == 1;
	InstallHook((void*)peekOriginal, myPeekMessage, peekOriginalPatch);

	// do our code
	if (result)
	{
		switch (lpMsg->message)
		{
		case WM_INPUT:
		{
			core->getRawInput()->ProcessMessage(lpMsg);
			if (core->getModControl()->IsActive())		// block whole game input when mod is active
				return false;
		}
		default:
			break;
		}
	}
	return result;
}

//naked
_declspec(naked) void GamePhysics()
{
	_asm pushad
	if (core->IsGamePhysicRunning())
	{
		_asm
		{
			XOR ESI, ESI
			jumpHere :
			MOV ECX, DWORD PTR SS : [EBP + 0xF0]
				TEST ECX, ECX
				JE end
				MOV EAX, DWORD PTR SS : [EBP + 0xF4]
				SUB EAX, ECX
				SAR EAX, 2
				CMP ESI, EAX
				JNB end
				MOV ECX, DWORD PTR DS : [ECX + ESI * 0x4]
				PUSH EBX
				MOV EAX, 0x0044BEA0
				CALL EAX
				INC ESI
				JMP SHORT jumpHere
			end :
		}
	}
	_asm popad
	_asm {
		push 0x005E1050
			retn;
	}
}

_declspec(naked) void GamePhysicsNPCS()
{
	_asm pushad
	if (!core->IsGamePhysicRunning())
	{
		_asm
		{
			popad
			push 0x005E107A 
			retn
		}
	}
	_asm {
		popad
		JE skipThat
		MOV EDX, DWORD PTR SS : [EBP + 0x118]

		push 0x005E1060
			retn;
	skipThat:
		push 0x005E1073
			retn;
	}
}

void GetGameSpeed(unsigned int actualSpeed,unsigned int& newspeed)
{

	/*unsigned int desiredSpeed = 100;
	if (core->getModControl()->gamespeedBox != NULL)
		desiredSpeed = (unsigned int)atoi(core->getModControl()->gamespeedBox->GetInput());
	//return (unsigned int) (actualSpeed*desiredSpeed/100);
	newspeed = (actualSpeed * desiredSpeed / 100);*/
	unsigned int desiredSpeed = 100;
	if (core->getModControl()->editSettingsSpeed != NULL)
		desiredSpeed = (unsigned int)atoi(core->getModControl()->editSettingsSpeed->GetInput());
	//return (unsigned int) (actualSpeed*desiredSpeed/100);
	newspeed = (actualSpeed * desiredSpeed / 100);
	//newspeed = actualSpeed;
}

_declspec(naked) void GameSpeed()
{
	_asm {
		sub ESP, 0x4
		mov EAX, ESP
		pushad
		push EAX
		push EBX
		call GetGameSpeed
		add ESP, 0x8
		popad

			//PUSH EAX
			MOV EAX, 0x005E0C20
			CALL EAX
			// now EAX holds game speed;
			push 0x005FFDCF
			retn;
	}
}

// Apply all project hooks
void	CHooks::ApplyThem()
{
	// hook for keyboard
	peekOriginal = (DWORD)GetProcAddress(
		GetModuleHandle(TEXT("user32.dll")),
		"PeekMessageW");
	InstallHook((void*)peekOriginal, myPeekMessage, peekOriginalPatch);

        //TODO: store and get width
	HWND mafiaWindow = (HWND) *(DWORD*) 0x101C167C;

	// hook for directx
	//IDirect3DDevice8* pointer,*original;
	original = (IDirect3DDevice8*) *(DWORD*) 0x101C167C;
	pointer = new CDirect3DDevice8Proxy(original);
	core->getGraphics()->SetDevice(pointer);
	core->getGraphics()->Init();

	*(DWORD*)0x101C167C = (DWORD)pointer;

	// game physics

	InstallHook((void*)0x005E1029, GamePhysics, peekOriginalPatchGamePhys);
	InstallHook((void*)0x005E1058, GamePhysicsNPCS, peekOriginalPatchGamePhys2);
	InstallHook((void*)0x005FFDC9, GameSpeed, peekOriginalPatchGameSpeed);

}

void	CHooks::UnloadThem()
{
	*(DWORD*)0x101C167C = (DWORD)original;
	Sleep(100);

	UninstallHook(0x005E1029,peekOriginalPatchGamePhys);
	UninstallHook(0x005E1058, peekOriginalPatchGamePhys2);
	UninstallHook(0x005FFDC9, peekOriginalPatchGameSpeed);
	UninstallHook(peekOriginal, peekOriginalPatch);
	
	core->getGraphics()->Unload();
	core->getGame()->CameraUnlock();

	

}
