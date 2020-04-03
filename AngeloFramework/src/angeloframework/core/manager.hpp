#ifndef ANGELOFRAMEWORK_CORE_MANAGER_HPP
#define ANGELOFRAMEWORK_CORE_MANAGER_HPP

#include "AngeloFramework/utils/game_hooks.hpp"
#include "AngeloFramework/game/offsets.hpp"
#include "AngeloFramework/core/direct8hookdevice.hpp"

#include "angeloframework/core/imgui/CImGUIAdaptor.hpp"

#include "angeloframework/core/utils.hpp"

#include "imgui.h"

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
            D3DRECT r;
            r.x1 = 0;
            r.y1 = 0;
            r.x2 = 200;
            r.y2 = 100;
            auto color = D3DCOLOR_XRGB(255, 0, 0);
            device->Clear(1, &r, D3DCLEAR_TARGET, color, 1.0, 1.0);

            m_imGuiAdaptor.Render();
        };

        auto callbackLostFunc = [&](IDirect3DDevice8* device)->void {
            m_imGuiAdaptor.Invalidate();
        };

    
        m_d3d8Hook = std::make_unique<Direct8HookDevice>(d3d8,callbackFunc, callbackLostFunc);
        m_hooks.AddObjectReplacement(reinterpret_cast<DWORD>(originald3d8DevicePtr),reinterpret_cast<DWORD>(m_d3d8Hook.get()));

        /*
            Initialize ImGUI with D3D9
        */
        auto d3d9 = AngeloFramework::Core::ConvertD3D8ToD3D9(d3d8);
        auto screenSize = AngeloFramework::Core::GetScreenSize(d3d8);
        m_imGuiAdaptor.Initialize(d3d9,screenSize);

        // Test render
        m_imGuiAdaptor.m_contentRenderers.add(
            []
            {
                ImGui::ShowDemoWindow();
            }
        );
        return true;
    }
    private:
    std::unique_ptr<Direct8HookDevice> m_d3d8Hook;
    GameHooks m_hooks;

    CImGUIAdaptor m_imGuiAdaptor;
};
} // namespace AngeloFramework


#endif
