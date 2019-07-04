#ifndef C_HOOKS
#define C_HOOKS
#include <functional>
struct IDirect3DDevice8;

using MessageHandler_t = std::function<bool(void*)>;

class CHooks {
public:
    void setInputMessageHandler(MessageHandler_t handler); 
    void ApplyThem();
    void UnloadThem();

    ///@Returns current D3D8 instance, used by LS3D engine
    IDirect3DDevice8* getD3D8Driver();

    ///@brief Replaces D3D8 driver and returns the original one
    IDirect3DDevice8* replaceDirectXDriver(IDirect3DDevice8* newDriver);
};

#endif
