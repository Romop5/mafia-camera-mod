#include "AngeloFramework/core/manager.hpp"

namespace AngeloFramework
{
    static Manager* hook_manager = nullptr;

    LRESULT CALLBACK WndProc(
        _In_ HWND   hwnd,
        _In_ UINT   uMsg,
        _In_ WPARAM wParam,
        _In_ LPARAM lParam
        )
    {
        bool shouldNotPassMessage = false;
        if (hook_manager)
        {
            switch (uMsg)
            {
                case WM_INPUT:
                {
                    MSG msg;
                    msg.hwnd = hwnd;
                    msg.wParam = wParam;
                    msg.lParam = lParam;
                    shouldNotPassMessage |= hook_manager->m_rawInput.ProcessMessage(&msg);
                }
                break;
                default:
                    break;
            }
            if (!shouldNotPassMessage)
            {
                return hook_manager->originalWindowProc(hwnd, uMsg, wParam, lParam);
            }
        }
        return DefWindowProcA(hwnd, uMsg, wParam, lParam);
    }

    Manager::Manager()
    {}
    bool Manager::Initialize()
    {
        /*
            Hook D3D8
        */
        auto originald3d8DevicePtr = reinterpret_cast<IDirect3DDevice8**>(GameOffsets::GetEngineAddress("D3D8"));
        auto d3d8 = *originald3d8DevicePtr;

        auto callbackFunc = [&](IDirect3DDevice8* device)->void {
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
        m_imGuiAdaptor.setMouseVisible(true);

        // Test render
        m_imGuiAdaptor.m_contentRenderers.add(
            []
            {
                ImGui::ShowDemoWindow();
            }
        );

        /*
            Hook input
        */
        hook_manager = this;
        auto directInputWindow = FindWindowW(L"DIEmWin", L"DIEmWin");
        if (directInputWindow)
        {
            auto result = SetWindowLongPtrA(directInputWindow, GWLP_WNDPROC, reinterpret_cast<LONG>(&WndProc));
            originalWindowProc = reinterpret_cast<WNDPROC>(result);
        }

        m_rawInput.m_onKeyPressedHandlers.add([&](unsigned short keycode, bool isDown)->bool
        {
            if (m_isOverlayControlled)
            {
                m_imGuiAdaptor.updateKey(keycode, isDown);
            }
            return false;
        });

        m_rawInput.m_onMouseMoveHandlers.add([&](long x, long y)->void
        {
            if (m_isOverlayControlled)
            {
                static Point2D point;
                point.x += x;
                point.y += y;
                m_imGuiAdaptor.updateMousePosition(point);
            }
        });

        m_rawInput.m_onMouseButtonsUpdateHandlers.add([&](unsigned short state, unsigned short delta)->void
        {
            m_imGuiAdaptor.updateButton(state, delta);
        });
        return true;
    }

    void Manager::ShouldOverlayAcceptUserInput(bool should)
    {
        m_isOverlayControlled = should;
        m_rawInput.shouldBlockInput(m_isOverlayControlled);
    }

    Manager::~Manager()
    {
        auto directInputWindow = FindWindowW(L"DIEmWin", L"DIEmWin");
        if (directInputWindow)
        {
            SetWindowLongPtrA(directInputWindow, GWLP_WNDPROC, reinterpret_cast<LONG>(&originalWindowProc));
        }
    }
}