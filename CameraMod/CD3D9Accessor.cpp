#include "CD3D9Accessor.h"
#include <d3d8to9.hpp>

struct IDirect3DDevice8;
IDirect3DDevice9* CD3D9Accessor::castFromD3D8(IDirect3DDevice8* device)
{
    Direct3DDevice8* wrapper = reinterpret_cast<Direct3DDevice8*>(device);
    return wrapper->GetProxyInterface();
}
