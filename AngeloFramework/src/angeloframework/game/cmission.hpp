#ifndef ANGELOFRAMEWORK_GAME_CMISSION_HPP
#define ANGELOFRAMEWORK_GAME_CMISSION_HPP

#include "internals.hpp"

namespace AngeloFramework
{
	namespace Game
	{
		class CCamera;
		struct CPlayer;
		class I3D_Scene;
		class I3D_Frame;

		class CMission;
		namespace Internal
		{
			ANGELO_STATIC_MEMBER(CMission, g_CMission, 0x65115C, 0x647E60, AngeloFramework::Game::CMission*);
		};
		
		struct CMafiaVector
		{
			union {
				float v[3];
				struct
				{
					float x, y, z;
				};
			};
		};
		struct CEntity
		{
			unsigned long vtable;		// 0x0
			byte _pad0[0xC];
			unsigned long objectType;	// 0x10
			byte _pad1[0x10];
			CMafiaVector position;		// 0x20
			CMafiaVector rotation;		// 0x2C
		};

		struct CPlayer : public CEntity
		{
			byte unk1[0x30];	// 0x38
			I3D_Frame*	m_frame;	// 0x68
		};
		class CGame
		{
		public:
			byte _pad0[0x4C];
			CCamera* mCamera;		// 0x4C
			byte _pad1[0x94];	
			CPlayer* mLocalPlayer;	// 0xE4
		};
		class CMission
		{
		private:
			byte unk1[0x10];
			I3D_Scene* m_Scene;	//0x10
			byte unk2[0x10];
			CGame* m_Game;		//0x24

		public:
			static CMission* GetMission()
			{
				auto object = *ANGELO_STATIC_MEMBER_GET_PTR(CMission, g_CMission);
				return object;
			}
			CGame* GetGame()
			{
				return m_Game;
			}
		};
	}
}
#endif