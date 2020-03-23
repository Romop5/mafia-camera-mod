/*
 * This module enables us to extract D3D9 proxy device from D3D8 device when
 * D3D8to9 wrapper is used
 */
struct IDirect3DDevice8;
struct IDirect3DDevice9;

class CD3D9Accessor {
public:
    static IDirect3DDevice9* castFromD3D8(IDirect3DDevice8* device);
};
