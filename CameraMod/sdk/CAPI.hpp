/**
 * @file CAPI.hpp
 * @author Roman Dobias
 * @brief Defines C API for accessing game implementations
 * 
 * Following functions and classes in SDK are used to intercommunicate 
 * between cameramod's core and game's wrapper.
 */

#ifndef CAMERAMOD_SDK_API_HPP
#define CAMERAMOD_SDK_API_HPP

#include "sdk/CGenericGame.hpp"

/// Returns game's wrapper version. Used to verify API/ABI compability
extern "C" size_t cameramod_game_get_module_version();

/// Initialize game module
extern "C" bool cameramod_game_initialize();

/// Return game handler
extern "C" CGenericGame* cameramod_game_get_module();

/// Unload game module
extern "C" bool cameramod_game_unload();

#endif