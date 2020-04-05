#ifndef ANGELOFRAMEWORK_GAME_INTERNALS_HPP
#define ANGELOFRAMEWORK_GAME_INTERNALS_HPP

#include <tuple>
#include "angeloframework/game/offsets.hpp"

/*
	Macros for convenient definition of class member functions or static variables.
	The purpose is to allow an uniform style of definition, independent of implementation
	which can be changed any time in macro definition.

	Note: symbols are declared (..._STATIC_MEMEBER, ..._CLASS_METHOD_DECLARE) and getters are used to
	get ptr to particular object (e.g. to function or symbol).
*/

#define ANGELO_INTERNAL_NS AngeloFramework::Game::Internal

#define ANGELO_STATIC_MEMBER(cls,name, address10,address12,membertype)\
 const auto g_##cls##name = MafiaAddress(address10, address12);\
 using g_##cls##name##_type=membertype

#define ANGELO_STATIC_MEMBER_GET_PTR(cls,name)\
 reinterpret_cast<ANGELO_INTERNAL_NS::g_##cls##name##_type*>(ANGELO_INTERNAL_NS::g_##cls##name.get())

#define ANGELO_STATIC_MEMBER_GET(cls,name)\
 reinterpret_cast<ANGELO_INTERNAL_NS::g_##cls##name##_type>(ANGELO_INTERNAL_NS::g_##cls##name.get())

#define ANGELO_CLASS_METHOD_DECLARE(cls,methodname, address10,address12)\
 const auto cls##_##methodname = MafiaAddress(address10, address12)

#define ANGELO_CLASS_TYPE_METHOD_DECLARE(cls,methodname, address10,address12,rettype,...)\
ANGELO_CLASS_METHOD_DECLARE(cls,methodname,address10,address12); using func_##cls##_##methodname = rettype __fastcall(void*, void*,__VA_ARGS__)

#define ANGELO_CLASS_METHOD_GET(cls,methodname)\
 ANGELO_INTERNAL_NS::cls##_##methodname.get()

#define ANGELO_CLASS_TYPE_METHOD_GET(cls,methodname)\
reinterpret_cast<ANGELO_INTERNAL_NS::func_##cls##_##methodname*>(ANGELO_CLASS_METHOD_GET(CProgram,SetSourceCode))

#define ANGELO_CLASS_TYPE_METHOD_CALL(cls,methodname,...)\
ANGELO_CLASS_TYPE_METHOD_GET(cls, methodname)(this, 0, __VA_ARGS__);

namespace AngeloFramework
{
	struct MafiaAddress
	{
		std::pair<void*, void*> m_addresses;
		MafiaAddress(DWORD a10, DWORD a12)
		{
			m_addresses = std::make_pair(reinterpret_cast<void*>(a10), reinterpret_cast<void*>(a12));
		}
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
}
#endif