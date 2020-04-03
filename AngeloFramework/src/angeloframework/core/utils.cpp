#include <d3d9.h>
#include <d3d8to9.hpp>
#include "AngeloFramework/core/utils.hpp"

class IDirect3DDevice8;

namespace AngeloFramework
{
namespace Core
{
    IDirect3DDevice9* ConvertD3D8ToD3D9(IDirect3DDevice8* device)
    {
        Direct3DDevice8* wrapper = reinterpret_cast<Direct3DDevice8*>(device);
        return reinterpret_cast<IDirect3DDevice9*>(wrapper->GetProxyInterface());
    }

    Point2D GetScreenSize(IDirect3DDevice8* device)
    {
        auto d3d9 = ConvertD3D8ToD3D9(device);
        D3DDEVICE_CREATION_PARAMETERS cparams;
        RECT rect;

        d3d9->GetCreationParameters(&cparams);
        GetWindowRect(cparams.hFocusWindow, &rect);
        Point2D point = {rect.right-rect.left, rect.bottom-rect.top};
        return point;
    }
}
}