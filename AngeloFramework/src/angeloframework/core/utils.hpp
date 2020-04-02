#ifndef ANGELOFRAMEWORK_CORE_UTILS_HPP
#define ANGELOFRAMEWORK_CORE_UTILS_HPP

#include "AngeloFramework/game/utils.hpp"

struct IDirect3DDevice9;
struct IDirect3DDevice8;
namespace AngeloFramework
{
namespace Core
{
    IDirect3DDevice9* ConvertD3D8ToD3D9(IDirect3DDevice8* device);
    Point2D GetScreenSize(IDirect3DDevice8* device);
}
}
#endif