#include <tuple>
#include "angeloframework/game/offsets.hpp"

#define ANGELO_CLASS_METHOD_DECLARE(cls,methodname, address10,address12)\
 const auto cls##_##methodname = MafiaAddress(address10, address12)

#define ANGELO_CLASS_TYPE_METHOD_DECLARE(cls,methodname, address10,address12,rettype,...)\
ANGELO_CLASS_METHOD_DECLARE(cls,methodname,address10,address12); using func_##cls##_##methodname = rettype __fastcall(void*, void*,__VA_ARGS__)

#define ANGELO_CLASS_METHOD_GET(cls,methodname)\
 AngeloFramework::Game::Internal::cls##_##methodname.get()

#define ANGELO_CLASS_TYPE_METHOD_GET(cls,methodname)\
reinterpret_cast<AngeloFramework::Game::Internal::func_##cls##_##methodname*>(ANGELO_CLASS_METHOD_GET(CProgram,SetSourceCode))

#define ANGELO_CLASS_TYPE_METHOD_CALL(cls,methodname,...)\
ANGELO_CLASS_TYPE_METHOD_GET(cls, methodname)(this, 0, __VA_ARGS__);

namespace AngeloFramework 
{
	struct MafiaAddress
	{
		std::pair<void*,void*> m_addresses;
		MafiaAddress(DWORD a10, DWORD a12)
		{
			m_addresses = std::make_pair(reinterpret_cast<void*>(a10), reinterpret_cast<void*>(a12));
		}
		/*MafiaAddress(void* a10, void* a12)
		{
			m_addresses = std::make_pair(a10, a12);
		}*/
		void* get() const
		{
			auto version = GameOffsets::GetEngineVersion();
			switch (version)
			{
			case GameOffsets::VERSION_10:
				return m_addresses.first;
			case GameOffsets::VERSION_12:
				return m_addresses.second;
			default:
				return 0;
			}
		}
	};
	namespace Game
	{
		class Frame;
		class Actor;
		namespace Internal
		{
			//ANGELO_CLASS_METHOD_DECLARE(CProgram, SetSourceCode, 0x0, 0x0);
			ANGELO_CLASS_TYPE_METHOD_DECLARE(CProgram, CProgram, 0x005AF2F0, 0x0, void);
			ANGELO_CLASS_TYPE_METHOD_DECLARE(CProgram, Init, 0x005AF780, 0x0, void);
			ANGELO_CLASS_TYPE_METHOD_DECLARE(CProgram, CallProcess, 0x005BABD0, 0x0, void,size_t);
			ANGELO_CLASS_TYPE_METHOD_DECLARE(CProgram, SetSourceCode, 0x0, 0x0,void, const char* sourceCode);
			ANGELO_CLASS_TYPE_METHOD_DECLARE(CProgram, Done, 0x005AF8A0, 0x0, void);
			ANGELO_CLASS_TYPE_METHOD_DECLARE(CProgram, Compile, 0x005BA6A0, 0x0, void,char*,bool,int*,int*);
		};
		class CProgramInstruction
		{
			size_t m_commandID;
			void* m_arguments;
		};
		class CProgram
		{
			char*						m_name;				// 0x4 - string name of script
			char*						m_sourceCode;		// 0x8 - string literal with full code
			byte 						unk2[4];
			CProgramInstruction*		m_assemblyStart;	// 0x10
			CProgramInstruction*		m_assemblyEnd;		// 0x14
			byte 						unk3[4];
			DWORD						m_currentOpcodeID;  // 0x1C
			DWORD						m_nextOpcodeID;     // 0x20
			DWORD						m_tickCount;     	// 0x24	- incremented with per-frame delta
			byte 						unk4[0x20];
			float*						m_fltArray;			// 0x48
			DWORD						m_fltArrayLength;	// 0x4C
			Frame**						m_frameArray;		// 0x50
			DWORD						m_frameArrayLength;	// 0x54
			Actor**						m_actorArray;		// 0x58
			DWORD						m_actorArrayLength;	// 0x5C
			byte 						unk5[0x4];
			byte						m_isCommandBlockActivated;	// 0x64
		public:
			/*
				Original in-game virtual methods
			*/
			CProgram() = delete;
			virtual ~CProgram() = 0;	// 0th
			virtual bool SaveToScene2BinFile(void* fileStructure) = 0;	// 1th
			virtual size_t SaveGameSaveGetSize() = 0;	// 2th

			virtual bool SaveGameSave(void* saveStructure) = 0; // 3th
			virtual bool SaveGameLoad(void* saveStructure) = 0; // 4th
			/// Parse argument and store it in instruction
			virtual bool DecodeParams(const char* argument, CProgramInstruction* instruction) = 0; // 5th
			/// Returns instruction ID
			virtual size_t DecodeInstruction(const char* line, size_t* consumedCharacters) = 0; // 6th
			/// Process current instruction
			virtual void Process(size_t elapsedTicks) = 0; // 7th

			/// Decompiles command at position in CProgram's compiled sequence to a human-readable string
			virtual void DecompileCommand(int commandOrder, char* stringToReceive) = 0; // 8th


			/// Loads script name and script source code from scene2.bin file
			//virtual void LoadFromScene2BinFile(void* fileStructure) = 0;

			/*
				Non-virtual non-static in-game methods
			*/
			void SetSourceCode(const char* sourceCode)
			{
				return ANGELO_CLASS_TYPE_METHOD_CALL(CProgram, SetSourceCode, sourceCode);
			}
		};
	}
}
