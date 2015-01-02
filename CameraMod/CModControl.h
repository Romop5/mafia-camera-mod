#include "CCore.h"

#ifndef C_MODCONTROL
#define C_MODCONTROL

struct CamPoint
{
	Vector3D position;
	Vector3D rotation;
	int uniqueID;
};

struct CamPath
{
	int uniqueID;
	int speed;
	bool useLookAt;
	int lookAtPoint;
	int pointA, pointB;
};
enum ControlStates{
	CMOD_FREECAM,
	CMOD_PICKING,
	// mod will 'play' paths
	CMOD_PLAYER,
};
class CModControl
{
private:
	bool		p_bIsActive;
	unsigned char p_bControlState;

	Vector3D	p_vectCamera;
	Vector3D	p_vectCameraRot;
	std::vector <CamPoint> points;
	std::vector <CamPath> paths;
	bool p_bBlockInput;

	void UpdateCam();
	void ActivateFreeCam(bool state);

	int pickedPoint;
	int pickedPointSecond;

	int playingID;
	int playingTime;
	float playintSpeed;

	CGUIButton*	p_Test;
	CGUITableList* listPoints,*listPaths;
	CGUIEditbox*	speedBox;
	int		pointID;

	CGUIGroup*	groupPoints, *groupPaths,*groupSettings,*groupAbout;
	CGUIButton*	butPoints, *butPaths,*butSettings,*butAbout;
	CGUIButton*	butPointsAddNew, *butPointsDelete;
	CGUILabel*	pointALab, *pointBLab;

	CGUIButton*	butPathsSwap, *butPathsDelete;
	CGUIEditbox*	editPathsSpeed;
	CGUICheckbox*	gameHasHUD;

public:

	CGUIEditbox	*editSettingsSpeed;
	CModControl();
	bool IsActive();
	void OnVKKey(USHORT key);
	void OnMouseMove(int x, int y);
	void OnMouseButtons(unsigned short buttons);

	bool isFreeCam();

	void BlockInput(bool should);
	bool IsInputBlocked();
	unsigned char GetState();
	
	void Render();

	void ToggleMod();

	void Init();

	void	onGUIClick(CGUIElement* elem);
	void	onGUISelectElement(CGUIElement* elem, int ID);
	void	onGUIElementChanged(CGUIElement* elem);

	CamPoint*		GetPointID(int ID);
};

#endif