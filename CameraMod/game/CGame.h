#ifndef C_GAME
#define C_GAME

#include <vector>
#include <forward_list>
#include <map>

#include "Windows.h"
#include "common/structures.h"

#include "CGenericGame.hpp"
#include "CScript.h"

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
	_pad(_unk6, 0xF0);
	bool		controlState;		//	040C-040D
	_pad(_unk7, 0x93);
	INVENTARY	inventary;			//	04A0-0520
	_pad(_unk8, 0xD4);
	float		inCarRotation;		//	05F4-05F8
	_pad(_unk9, 0x4C);
	float		health;				//	0644-0648
	_pad(_unk10, 0x492);
	bool 		isControlled;		//  0ADA
} PED;


struct PED_State
{
	glm::mat4 transform;
	byte	animState;
	bool	isDucking;
	bool	isAiming;	
	bool	isReloading;
};

struct Attribute
{
	DWORD type;		// or length ? 
};

/**
 * @brief Describes a script command
 * 
 * Each command is made of opcode (an unique command ID) and may have additional payload.
 * The payload is modelled using @Attribute class. 
 */
struct Opcode
{
	DWORD   m_opcode;
	Attribute* m_attribute;
	const char* getName() const
	{
		if(m_opcode >= g_commandID)
		{	
			return "Unknown command";
		}
		return g_scriptCommands[m_opcode];
	}
};

struct Vtable
{
	DWORD* swapFunction(size_t index, DWORD* newPtr)
	{
		index *= 4;
		auto oldPtr = *reinterpret_cast<DWORD**>(this+index);
		DWORD protect;
		DWORD** address = reinterpret_cast<DWORD**>(this+index);
        VirtualProtect(address, 4, PAGE_READWRITE, &protect);
		*address = newPtr;
        VirtualProtect(address, 4, protect, &protect);
		return oldPtr;
	}
};

struct Script
{
	Vtable*	m_vtable;			// vtable
	char*	m_name;				// 0x4 - string name of script
	char*	m_sourceCode;		// 0x8 - string literal with full code
	byte 	unk2[4];
	Opcode* m_assemblyStart;	// 0x10
	Opcode* m_assemblyEnd;		// 0x14
	byte 	unk3[4];
	DWORD	m_currentOpcodeID;  // 0x1C
	DWORD	m_nextOpcodeID;     // 0x20
	byte 	unk4[0x40];
	DWORD	m_isSleeping;  		// 0x64 - or is stopped

	const char* getName() const
	{
		if(m_name)
			return m_name;
	
		static char ptrStr[100];
		sprintf(ptrStr,"Script %p",this);
		return ptrStr;
	}

	const char* getSource() const
	{
		return (m_sourceCode?m_sourceCode:"");
	}

	Opcode* getOpcodeAtPosition(size_t i)
	{
		if(getOpcodesCount() < i)
			return m_assemblyStart;
		return &m_assemblyStart[i];
	}
	size_t getOpcodesCount() const 
	{
		ptrdiff_t distance = m_assemblyEnd-m_assemblyStart;
		return distance;
	}
	
	static Vtable* getVtable()
	{
		//return reinterpret_cast<Vtable*>(0x0063C0A4);
		return reinterpret_cast<Vtable*>(0x0063B45C);
	} 
	static Vtable* getVtable1()
	{
		return reinterpret_cast<Vtable*>(0x0063C0A4);
	}

	void forceRun()
	{
		using initScript_t = void __fastcall(Script* s);
		auto initFunc = reinterpret_cast<initScript_t*>(0x005BAAE0);
		initFunc(this);
	} 

	void recompile()
	{
		// TODO: we should deallocated properly to prevent leaks, but scripts are small (1kb max, so who cares?) 
		//this->m_assemblyStart = nullptr;
		//this->m_assemblyEnd = nullptr;
		using recompile_T = void __fastcall(Script* s);
		auto recompileFunc = reinterpret_cast<recompile_T*>(0x005AF780);
		recompileFunc(this);
	}
};

/**
 * @brief Contains important handles
 * 
 * In Mafia 1.0, the object is located at 0x006F9440
 */
struct ScriptMachine
{
	byte 		unk1[0x58];
	Script**	m_scriptsPoolStart;		// 0x58 - an array of ptrs to script structures	
	Script**	m_scriptsPoolEnd;		// 0x5C - ptr to end, each element has 4 bytes

	size_t getCountOfScripts() const
	{
		ptrdiff_t distance = m_scriptsPoolEnd-m_scriptsPoolStart;
		return distance;
	}
	Script* getScriptAtIndex(const size_t i)
	{
		if(i >= getCountOfScripts())
			return nullptr;
		return m_scriptsPoolStart[i];
	}	
};


///////////////////////////////////////////////////////////////////////////
// INTERFACES
///////////////////////////////////////////////////////////////////////////
class CHuman;

class CMafiaRecordingInfo: public CGenericRecordingInfo
{
	private:
	size_t m_currentReplayIndex;
	std::vector<PED_State> m_Frames;
	bool m_isRecording;

	public:
	CMafiaRecordingInfo(): m_currentReplayIndex(0),m_isRecording(true) {}
	// PUBLIC API
    virtual bool IsRecording() const override 
	{
		return m_isRecording;	
	}
    virtual bool IsReplaying() const override { return !this->IsRecording(); }

    virtual size_t getFramesCount() const override
	{
		return m_Frames.size();
	}
    virtual size_t getEventsCount() const override
	{
		return 0;
	}
    virtual size_t getCurrentFrameIndex() const override
	{
		return m_currentReplayIndex;
	}
	friend class CPlayerRecording;
};

class CPlayerRecording: public CGenericObjectRecording
{
	private:
	CMafiaRecordingInfo m_state;
	public:
	CPlayerRecording() {}

	// PUBLIC API
    virtual const std::string dumpJSON() const
	{
		return "";
	}
    virtual void loadFromJSON(const std::string& json) {}

	// INTERNAL API
	void updateState(CHuman& human);
	void recordState(CHuman& human);
	CMafiaRecordingInfo* getInfo() { return &m_state; }
};

class CMafiaObject: public CGenericObject
{
	private:
	/// Ingame object's address
	DWORD m_mafiaAdress;
	
	///////////////////////////////////////////////////////
	// RECORDING
	public:
	enum RecordingState
	{
		NONE,
		RECORDING,
		PLAYING	
	};
	private: 
	CPlayerRecording m_recording;
	std::forward_list<CPlayerRecording> m_recordings;
	RecordingState m_currentState;
	///////////////////////////////////////////////////////
	public:
	CMafiaObject(): m_currentState(NONE) {}
	virtual DWORD getMafiaAddress() const { return m_mafiaAdress; }
	void setMafiaAddress(const DWORD address) { m_mafiaAdress = address; }
	friend class CGame;
	friend class CMafiaRecordingInfo;
};

class CHuman: public CMafiaObject
{
	private:
	PED* toPED() {return reinterpret_cast<PED*>(getMafiaAddress());}
	PED* toPED() const {return reinterpret_cast<PED*>(getMafiaAddress());}

	public:
	virtual const std::string& getName() const override;
    virtual const std::string& getType() const override {
		static const std::string classString = "HUMAN"; 
		return classString;
	}

    virtual glm::mat4 getTransform() const override; 
    virtual void setTransform(const glm::mat4&) override;
    virtual const std::string dumpJSON() const override;

	void setState(const PED_State& state);
	const PED_State getState() const;
};

class CLocalPlayer: public CHuman
{
	public:
	virtual DWORD getMafiaAddress() const override 
	{
		return *(DWORD*)((*(DWORD*)0x6F9464) + 0xE4);
	}
};

/*class CGameObjectManager
{
	private:
	std::map<DWORD, CMafiaObject> m_objects;

	auto begin() { return m_objects.begin(); }
	auto end() { return m_objects.begin(); }

	bool hasObject(CMafiaObject& obj)
	{
		auto reference = m_objects.find(obj.getMafiaAddress());
		return (reference != m_objects.end());
	}

	void addObject(CMafiaObject& obj)
	{
		m_objects[obj.getMafiaAddress()] = obj;
	}
};
*/
class CGame: public CGenericGame {
private:
    enum COLOR:DWORD 
    {
        COLOR_RED = 0x00FF0000,
        COLOR_WHITE = 0x00FFFFFF,
    };

//	CGameObjectManager m_objManager;
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

	CLocalPlayer m_localPlayer;
	
    std::vector<std::unique_ptr<CGenericGameSettingBase>> m_settings;
public:
	virtual void onTick() override;
    virtual CGenericObject* getLocalPlayer() override;
    virtual const CGenericObject* getLocalPlayer() const;

    virtual void SetCameraTransform(const glm::mat4& transform) override;
    virtual void UnlockCameraTransform() override;
    virtual const glm::mat4 GetCameraTransform() const override;

    virtual void createObjectFromJSON(const std::string) override;

    virtual void LockControls(bool shouldBeLocked);

    virtual std::vector<std::unique_ptr<CGenericGameSettingBase>>& getSettings();

    ///////////////////////////////////////////////////////////////////////////
    // OPTIONAL METHODS (may be implemented by game)
    ///////////////////////////////////////////////////////////////////////////
    virtual void ToggleHUD(bool shouldBeVisible) override;
    virtual const std::string& GetVersionString() const override;
    virtual void PrintDebugMessage(const std::string& message) override;

    /// Record object's state starting this moment
    virtual void startRecording(CGenericObject* object) override;
    virtual void clearRecording(CGenericObject* object) override;
    virtual CGenericObjectRecording* saveRecording(CGenericObject* object) override;
    virtual void playRecording(CGenericObject* object, CGenericObjectRecording* record) override;
    virtual CGenericRecordingInfo* getRecordingInfo(CGenericObject* object) override;

	
	ScriptMachine* getScriptMachine() { return *reinterpret_cast<ScriptMachine**>(0x0065115C); }
};

#endif
