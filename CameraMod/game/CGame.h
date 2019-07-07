#ifndef C_GAME
#define C_GAME

#include <vector>

#include "Windows.h"
#include "common/structures.h"

#include "CGenericGame.hpp"

enum ObjectTypes
{
	OBJECT_LOCALPLAYER = 0x2,
	OBJECT_VEHICLE = 0x4,
	OBJECT_ENEMY = 0x1B
};

using byte = unsigned char;

#define _pad(x,y) byte x[y]

typedef struct _FRAME {
	_pad(_unk1, 0x100);		// 000-100
	char*	frameName;		// 100-104
	_pad(_unk2, 0x50);
	char*	frameModel;		// 154-158
} FRAME;
typedef struct _OBJECT {
	DWORD		vtable;				//  0000-0004
	_pad(__unk1,0xC);				//	0004-0010
	DWORD		objectType;			//	0010-0014
	_pad(__unk2, 0x10);				//	0014-0024
	Vector3D	position;			//	0024-0030
	Vector3D	rotation;			//	0030-003C
	_pad(__unk3, 0x21);				//  003C-005D
	bool		isActive;			//	005D-005E
	_pad(__unk4, 0xA);
	FRAME*		frame;				//	0068-006C
} OBJECT;
typedef struct _VEHICLE {
	OBJECT		object;				//  0000-006C
	_pad(__unk1, 0x238);
	float		engineDamage;		//	02A4-02A8	(0-100f)
	_pad(__unk2, 0x164);
	Vector3D	position;			//	040C-0418
	_pad(__unk3, 0x78);

	float		gasState;			//	0490-0494	(0/1.0f)
	_pad(__unk4, 0x88);
	bool		hornState;			//	051C-051D
	bool		sirenState;			//	051D-051E
	_pad(__unk5, 0x12A);

	int			speedGear;			//	0648-064C
	_pad(__unk6, 0xC4);
	float		wheelRotation;		//	0710-0714
	_pad(__unk7, 0x608);
	float		petrolLevel;		//	0D1C-0D20
	_pad(__unk8, 0x8);
	Vector3D	rotation;			//	0D28-0D34
	_pad(__unk9, 0xC);
	Vector3D	rotationSecond;		//	0D40-0D4C
	_pad(__unk10, 0x1230);
	Vector3D	speed;				//	1F7C-1F88
	_pad(__unk11, 0x10C);
	int			engineShotDamage;	//	2094-2098
} VEHICLE;

typedef struct _INVENTARY_SLOT {
	DWORD		weaponType;
	DWORD		ammoLoaded;
	DWORD		ammo;
	DWORD		__unk;		// maybe weaponObject
} INVETARY_SLOT;

typedef struct _INVENTARY {
	_INVENTARY_SLOT slot[8];
} INVENTARY;

typedef struct _PED {
	OBJECT		object;				//  0000-006C
	_pad(_unk0, 0x4);				//	006C-0070
	byte		animStateLocal;		//	0070-0071
	byte		isInAnimWithCarLocal;//	0071-0072
	_pad(_unk1, 0x2);				//	0072-0074
	byte		animState;			//	0074-0075
	byte		isInAnimWithCar;	//	0075-0076
	_pad(_unk2, 0x22);
	VEHICLE*	playersCar;			//	0098-009C
	VEHICLE*	carLeavingOrEntering;//	009C-00A0
	_pad(_unk3, 0x144);
	bool		isDucking;			//	01E4-01E5
	bool		isAiming;			//	01E5-01E6
	_pad(_unk4, 0x16);
	bool		isReloading;		//	01FC-01FD
	_pad(_unk5, 0x11B);
	FRAME*		frm_neck;			//	0318-031C
	_pad(_unk6, 0x184);
	INVENTARY	inventary;			//	04A0-0520
	_pad(_unk7, 0xD4);
	float		inCarRotation;		//	05F4-05F8
	_pad(_unk8, 0x4C);
	float		health;				//	0644-0648
	_pad(_unk9, 0x492);
	bool 		isControlled;		//  0ADA
} PED;


struct PED_State
{
	byte	animState;
	bool	isDucking;
	bool	isAiming;	
	bool	isReloading;
};


///////////////////////////////////////////////////////////////////////////
// INTERFACES
///////////////////////////////////////////////////////////////////////////
class CPlayerRecording: public CGenericObjectRecording
{
    virtual const std::string dumpJSON() const
	{
		return "";
	}
    virtual void loadFromJSON(const std::string& json) {}
};

class CHuman: public CGenericObject
{
	private:
	DWORD m_object;
	PED* toPED() {return reinterpret_cast<PED*>(m_object);}
	PED* toPED() const {return reinterpret_cast<PED*>(m_object);}

	public:
	virtual const std::string& getName() const override;
    virtual const std::string& getType() { return "HUMAN";}

    virtual glm::mat4 getTransform() const override; 
    virtual void setTransform(const glm::mat4&) override;
    virtual const std::string dumpJSON() const override;
};


class CGame: public CGenericGame {
private:
    enum COLOR:DWORD 
    {
        COLOR_RED = 0x00FF0000,
        COLOR_WHITE = 0x00FFFFFF,
    };
private:
    void SetCameraPos(Vector3D pos, float r1, float r2, float r3,
        float r4);
    void SetCameraPos(Vector3D pos, Vector3D rot);
    // CameraUnlock means that camera will return to player
    void CameraUnlock();

    bool IsGameRunning();

    DWORD GetPlayerBase() const;

    int GetGameVersion() const;

    bool isWindowed() const;

    size_t getScreenWidth() const;
    size_t getScreenHeight() const;

    void writeToConsole(DWORD colour, const char* message);

	void internalLockControls(bool shouldLock);
    void internalToggleHUD(bool shouldBeHidden);

public:
    virtual CGenericObject* getLocalPlayer() override;
    virtual CGenericObject* getLocalPlayer() const;

    virtual void SetCameraTransform(const glm::mat4& transform) override;
    virtual void UnlockCameraTransform() override;
    virtual const glm::mat4 GetCameraTransform() const override;

    virtual void createObjectFromJSON(const std::string) override;

    virtual void LockControls(bool shouldBeLocked);

    ///////////////////////////////////////////////////////////////////////////
    // OPTIONAL METHODS (may be implemented by game)
    ///////////////////////////////////////////////////////////////////////////
    virtual void ToggleHUD(bool shouldBeVisible) override;
    virtual const std::string& GetVersionString() const override;
    virtual void PrintDebugMessage(const std::string& message) override;

    /// Record object's state starting this moment
    virtual void startRecording(CGenericObject& object) override;
    virtual void clearRecording(CGenericObject& object) override;
    virtual CGenericObjectRecording& saveRecording(CGenericObject& object) override;
};

#endif
