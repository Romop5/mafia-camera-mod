#ifndef C_HOOKS
#define C_HOOKS

struct IDirect3DDevice8;

class CHooks {
public:
    void ApplyThem();
    void UnloadThem();

    ///@Returns current D3D8 instance, used by LS3D engine
    IDirect3DDevice8* getD3D8Driver();

    ///@brief Replaces D3D8 driver and returns the original one
    IDirect3DDevice8* replaceDirectXDriver(IDirect3DDevice8* newDriver);
};

#endif
