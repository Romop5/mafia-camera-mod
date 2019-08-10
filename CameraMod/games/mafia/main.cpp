#include "sdk/CAPI.hpp"
#include "sdk/CGenericGame.hpp"

#include "CGame.h"

/// Returns game's wrapper version. Used to verify API/ABI compability
extern "C" size_t cameramod_game_get_module_version()
{

}

/// Initialize game module
extern "C" bool cameramod_game_initialize()
{

}

/// Return game handler
extern "C" CGenericGame* cameramod_game_get_module()
{

}

/// Unload game module
extern "C" bool cameramod_game_unload()
{

}