#include "CCore.h"
//#include "CGame.h"

void CGame::SetCameraPos(Vector3D pos, float r1, float r2, float r3, float r4)
{
    _asm {
		sub esp, 0x400
			LEA ECX, DWORD PTR SS : [ESP + 0x388]
			mov EAX, r1
			MOV DWORD PTR SS : [ECX], EAX
			mov EAX, r2
			MOV DWORD PTR SS : [ECX + 0x4], EAX
			mov EAX, r3
			MOV DWORD PTR SS : [ECX + 0x8], EAX
                                                                                                                                                                                                       // mov EAX, r4
                                                                                                                                                                                                       // MOV DWORD PTR SS : [ECX + 0xC], EAX
			PUSH ECX
			MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			lea EAX, pos
			PUSH EAX
			MOV EAX, 0x00425390
			CALL EAX;  Game.00425390
			MOV ECX, EAX
			MOV EAX, 0x00425510
			CALL EAX;  Game.00425510
			MOV ECX, EAX
			MOV EAX, 0x005DB000
			CALL EAX;  entire camera change - Game.005DB000
			MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			MOV EAX, 0x00425390
			CALL EAX;  Game.00425390
			MOV ECX, EAX
			MOV EAX, 0x005C8B50
			CALL EAX;  Game.005C8B50
			CMP EAX, 0x6
			JNB SHORT end
			MOV ECX, DWORD PTR DS : [0x65115C];  Game.006F9440
			MOV EAX, 0x00425390
			CALL EAX; Game.00425390
			MOV ECX, EAX
			MOV EAX, 0x005F5C60
			CALL EAX; Game.005F5C60
		end:
		add esp, 0x400
    }
}

void CGame::CameraUnlock()
{
    _asm {
		MOV ECX, DWORD PTR DS : [0x65115C];
			PUSH 0
			MOV EAX, 0x00425390
			CALL EAX;  Game.00425390
			MOV ECX, EAX
			MOV EAX, 0x00425510
			CALL EAX;  Game.00425510
			MOV ECX, EAX
			MOV EAX, 0x005DB5D0
			CALL EAX;  Game.005DB5D0
			MOV ECX, DWORD PTR DS : [0x65115C]; 
			MOV EAX, 0x00425390
			CALL EAX;  Game.00425390
			MOV ECX, EAX
			MOV EAX, 0x005F19C0
			CALL EAX;  Game.005F19C0
    }
}

void CGame::SetCameraPos(Vector3D pos, Vector3D rot)
{
    this->SetCameraPos(pos, rot.x, rot.y, rot.z, 0.0);
}

bool CGame::IsGameRunning()
{
    if (*(DWORD*)((*(DWORD*)0x6F9464) + 0xE4) == 0)
        return false;
    return true;
}

DWORD CGame::GetPlayerBase() { return *(DWORD*)((*(DWORD*)0x6F9464) + 0xE4); }

Vector3D CGame::GetPlayerPosition()
{
    DWORD playerBase = this->GetPlayerBase();
    Vector3D position = { 0.0, 0.0, 0.0 };
    if (playerBase) {
        position.x = *(float*)(playerBase + 0x24);
        position.y = *(float*)(playerBase + 0x28);
        position.z = *(float*)(playerBase + 0x2C);
    }
    return position;
}
Vector3D CGame::GetPlayerRotation()
{
    DWORD playerBase = this->GetPlayerBase();
    Vector3D rotation = { 0.0, 0.0, 0.0 };
    if (playerBase) {
        rotation.x = *(float*)(playerBase + 0x30);
        rotation.y = *(float*)(playerBase + 0x34);
        rotation.z = *(float*)(playerBase + 0x38);
    }
    return rotation;
}

void CGame::ToggleHUD(bool state)
{
    if (state != 1)
        (*(byte*)0x006613D4) |= 1 << 0;
    else
        (*(byte*)0x006613D4) &= ~(1 << 0);
}
