#ifndef C_GAME
#define C_GAME

#include "structures.h"

class CGame
{
private:
public:
	void CGame::SetCameraPos(Vector3D pos, float r1, float r2, float r3, float r4);
	void CGame::SetCameraPos(Vector3D pos, Vector3D rot);
	// CameraUnlock means that camera will return to player
	void CGame::CameraUnlock();	

	bool	IsGameRunning();
	DWORD GetPlayerBase();
	Vector3D GetPlayerPosition();
	Vector3D GetPlayerRotation();
	void	ToggleHUD(bool state);
};

#endif