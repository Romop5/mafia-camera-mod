#ifndef ANGELOFRAMEWORK_CORE_MANAGER_HPP
#define ANGELOFRAMEWORK_CORE_MANAGER_HPP

#include "AngeloFramework/utils/game_hooks.hpp"
#include "AngeloFramework/game/offsets.hpp"
#include "AngeloFramework/core/direct8hookdevice.hpp"

#include "angeloframework/core/imgui/CImGUIAdaptor.hpp"

#include "angeloframework/core/utils.hpp"

namespace AngeloFramework
{
class Manager
{
    public:
    Manager()
    {

    }
    bool Initialize()
    {
        /*
            Hook D3D8
        */
        auto originald3d8DevicePtr = reinterpret_cast<IDirect3DDevice8**>(GameOffsets::GetEngineAddress("D3D8"));
        auto d3d8 = *originald3d8DevicePtr;

        auto callbackFunc = [&](IDirect3DDevice8* device)->void {
             this->onRender(device);
        };
        m_d3d8Hook = std::make_unique<Direct8HookDevice>(d3d8,callbackFunc);
        m_hooks.AddObjectReplacement(reinterpret_cast<DWORD>(originald3d8DevicePtr),reinterpret_cast<DWORD>(d3d8));

        /*
            Initialize ImGUI with D3D9
        */
        auto d3d9 = AngeloFramework::Core::ConvertD3D8ToD3D9(d3d8);
        auto screenSize = AngeloFramework::Core::GetScreenSize(d3d8);
        m_imGuiAdaptor.Initialize(d3d9,screenSize);
        return true;
    }
    private:
    void onRender(IDirect3DDevice8* device)
    {
        m_imGuiAdaptor.Win32NewFrame();

        m_imGuiAdaptor.Render();
    }
    std::unique_ptr<Direct8HookDevice> m_d3d8Hook;
    GameHooks m_hooks;

    CImGUIAdaptor m_imGuiAdaptor;
};
} // namespace AngeloFramework


#endif
