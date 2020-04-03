#ifndef ANGELOFRAMEWORK_CORE_MANAGER_HPP
#define ANGELOFRAMEWORK_CORE_MANAGER_HPP

#include "AngeloFramework/utils/game_hooks.hpp"
#include "AngeloFramework/game/offsets.hpp"
#include "AngeloFramework/core/direct8hookdevice.hpp"

#include "angeloframework/core/imgui/CImGUIAdaptor.hpp"
#include "angeloframework/core/CRawInput.h"

#include "angeloframework/core/utils.hpp"

#include "imgui.h"
namespace AngeloFramework
{
    class Framework;
}
namespace AngeloFramework
{
class Manager
{
    public:
        Manager();
        ~Manager();
        bool Initialize();

        void ShouldOverlayAcceptUserInput(bool should);

        friend LRESULT CALLBACK WndProc(
        _In_ HWND   hwnd,
        _In_ UINT   uMsg,
        _In_ WPARAM wParam,
        _In_ LPARAM lParam
        );
        friend class AngeloFramework::Framework;
    private:
        bool m_isOverlayControlled = false;
        std::unique_ptr<Direct8HookDevice> m_d3d8Hook;
        GameHooks m_hooks;

        CImGUIAdaptor m_imGuiAdaptor;
        CRawInput m_rawInput;

        WNDPROC  originalWindowProc = nullptr;

        std::function<void(void)> m_userOnRenderCallback;
};
} // namespace AngeloFramework


#endif
