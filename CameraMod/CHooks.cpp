//#include "CHooks.h"
#include "CCore.h"
#include <functional>

extern CCore* core;

namespace GameOffsets
{
    // LS3D_BASE is where ls3df.dll is usually loaded. However, this can vary due
    // to address space randomization, so relative addresses are used below
    const DWORD LS3D_BASE = 0x10000000;
    const DWORD LS3D_D3D_DRIVER = 0x101C167C;
};

MessageHandler_t userHandler;

DWORD peekOriginal = NULL;
char peekOriginalPatch[5];
char peekOriginalPatchGamePhys[5];
char peekOriginalPatchGamePhys2[5];
char peekOriginalPatchGameSpeed[5];

BOOL WINAPI myPeekMessage(
    _Out_     LPMSG lpMsg,
    _In_opt_  HWND hWnd,
    _In_      UINT wMsgFilterMin,
    _In_      UINT wMsgFilterMax,
    _In_      UINT wRemoveMsg
    )
{
    // Enable original function & call it
    UninstallHook(peekOriginal, peekOriginalPatch);
    bool result = PeekMessageW(lpMsg, hWnd, wMsgFilterMin, wMsgFilterMax, wRemoveMsg) == 1;
    // Restore our hook
    InstallHook((void*)peekOriginal, myPeekMessage, peekOriginalPatch);

    // do our code
    if (result)
    {
        switch (lpMsg->message)
        {
            // WM_INPUT messages contains RawInput messages with key strokes & keyboard state
            // See https://docs.microsoft.com/en-us/windows/desktop/inputdev/raw-input
            case WM_INPUT:
                //Returning zero in WM_INPUT indicates that message has been processed a thus can be
                //deleted
               return userHandler(lpMsg);
            default: break;
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

	//unsigned int desiredSpeed = 100;
        //desiredSpeed = actualSpeed;
	//newspeed = (actualSpeed * desiredSpeed / 100);
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

void CHooks::setInputMessageHandler(MessageHandler_t handler)
{
    userHandler = handler;
}

// Apply all project hooks
void	CHooks::ApplyThem()
{
    // Hook game's ProcessMessage routine -> needed for hooking keyboard
    peekOriginal = (DWORD)GetProcAddress(
            GetModuleHandle(TEXT("user32.dll")),
            "PeekMessageW");
    InstallHook((void*)peekOriginal, myPeekMessage, peekOriginalPatch);

    //TODO: store and get width
    //HWND mafiaWindow = (HWND) *(DWORD*) 0x101C167C;

    // game physics
    InstallHook((void*)0x005E1029, GamePhysics, peekOriginalPatchGamePhys);
    InstallHook((void*)0x005E1058, GamePhysicsNPCS, peekOriginalPatchGamePhys2);
    InstallHook((void*)0x005FFDC9, GameSpeed, peekOriginalPatchGameSpeed);

}

///@Returns current D3D8 instance, used by LS3D engine
IDirect3DDevice8* CHooks::getD3D8Driver()
{
    auto loadedBase = GetModuleBaseAddress(TEXT("ls3df.dll"));
    auto loadedAddress = GetAddressBasedOnOldModule(GameOffsets::LS3D_D3D_DRIVER, GameOffsets::LS3D_BASE, loadedBase);
    return (IDirect3DDevice8*) *(DWORD*) loadedAddress;
}

///@brief Replaces D3D8 driver and returns the original one
IDirect3DDevice8* CHooks::replaceDirectXDriver(IDirect3DDevice8* newDriver)
{
    auto oldDriverPtr = this->getD3D8Driver();

    auto loadedBase = GetModuleBaseAddress(TEXT("ls3df.dll"));
    auto loadedAddress = GetAddressBasedOnOldModule(GameOffsets::LS3D_D3D_DRIVER, GameOffsets::LS3D_BASE, loadedBase);

    *(IDirect3DDevice8**) loadedAddress =  newDriver;
    return oldDriverPtr;
}


void CHooks::UnloadThem()
{
    Sleep(100);

    UninstallHook(0x005E1029,peekOriginalPatchGamePhys);
    UninstallHook(0x005E1058, peekOriginalPatchGamePhys2);
    UninstallHook(0x005FFDC9, peekOriginalPatchGameSpeed);
    UninstallHook(peekOriginal, peekOriginalPatch);
}
