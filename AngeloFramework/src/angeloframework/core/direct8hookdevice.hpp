#include <functional>
#include "AngeloFramework/utils/IDirect3DDevice8Proxy.h"

namespace AngeloFramework
{
class Direct8HookDevice: public IDirect3DDevice8Proxy
{
    public:
    using CallbackFuntion = std::function<void(IDirect3DDevice8*)>;
    Direct8HookDevice(IDirect3DDevice8* device, CallbackFuntion callback, CallbackFuntion callbackLost) 
        : IDirect3DDevice8Proxy(device), m_callback(callback), m_callbackLost(callbackLost) {}
    STDMETHOD(EndScene)()
    {
        if(m_callback)
            m_callback(static_cast<IDirect3DDevice8*>(this));
        // Call original
        return IDirect3DDevice8Proxy::EndScene();
    }
    STDMETHOD(Reset)
        (D3DPRESENT_PARAMETERS* presentParam)
    {
        if (m_callbackLost)
            m_callbackLost(static_cast<IDirect3DDevice8*>(this));
        return IDirect3DDevice8Proxy::Reset(presentParam);
    }
    private:
    CallbackFuntion m_callback;
    CallbackFuntion m_callbackLost;
};
}