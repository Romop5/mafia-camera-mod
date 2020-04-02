#include <functional>
#include "AngeloFramework/utils/IDirect3DDevice8Proxy.h"

namespace AngeloFramework
{
class Direct8HookDevice: public IDirect3DDevice8Proxy
{
    public:
    using CallbackFuntion = std::function<void(IDirect3DDevice8*)>;
    Direct8HookDevice(IDirect3DDevice8* device, CallbackFuntion callback) : IDirect3DDevice8Proxy(device), m_callback(callback) {}
    STDMETHOD(EndScene)()
    {
        if(m_callback)
            m_callback(static_cast<IDirect3DDevice8*>(this));
        // Call original
        return IDirect3DDevice8Proxy::EndScene();
    }
    private:
    CallbackFuntion m_callback;
};
}