#include "game/CImGUIAdaptor.hpp"
#include "imgui.h"
#include "imgui_impl_dx9.h"
#include <windows.h>
#include <vector>
#include <tuple>
#include "utilslib/logger.hpp"

void CImGUIAdaptor::Initialize(IDirect3DDevice9* device, Point2D size)
{
    this->screenSize = size;

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    (void)io;
    // io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;  // Enable Keyboard
    // Controls io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;   // Enable
    // Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    // ImGui::StyleColorsClassic();

    ImGui_ImplDX9_Init(device);
    this->SetUpIO();
}

void CImGUIAdaptor::Render()
{
    ImGui_ImplDX9_NewFrame();
    this->Win32NewFrame();
    ImGui::NewFrame();
    // Rendering
    for(auto callback: m_contentRenderers)
        callback();
    ImGui::EndFrame();

    ImGui::Render();
    ImGui_ImplDX9_RenderDrawData(ImGui::GetDrawData());
}

bool CImGUIAdaptor::SetUpIO()
{
    // if (!::QueryPerformanceFrequency((LARGE_INTEGER *)&g_TicksPerSecond))
    //    return false;
    // if (!::QueryPerformanceCounter((LARGE_INTEGER *)&g_Time))
    //    return false;

    // Setup back-end capabilities flags
    // g_hWnd = (HWND)hwnd;
    ImGuiIO& io = ImGui::GetIO();
    io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors; // We can honor GetMouseCursor() values
        // (optional)
    io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos; // We can honor io.WantSetMousePos
        // requests (optional, rarely used)
    io.BackendPlatformName = "imgui_impl_win32";
    // io.ImeWindowHandle = hwnd;

    // Keyboard mapping. ImGui will use those indices to peek into the
    // io.KeysDown[] array that we will update during the application lifetime.
    io.KeyMap[ImGuiKey_Tab] = VK_TAB;
    io.KeyMap[ImGuiKey_LeftArrow] = VK_LEFT;
    io.KeyMap[ImGuiKey_RightArrow] = VK_RIGHT;
    io.KeyMap[ImGuiKey_UpArrow] = VK_UP;
    io.KeyMap[ImGuiKey_DownArrow] = VK_DOWN;
    io.KeyMap[ImGuiKey_PageUp] = VK_PRIOR;
    io.KeyMap[ImGuiKey_PageDown] = VK_NEXT;
    io.KeyMap[ImGuiKey_Home] = VK_HOME;
    io.KeyMap[ImGuiKey_End] = VK_END;
    io.KeyMap[ImGuiKey_Insert] = VK_INSERT;
    io.KeyMap[ImGuiKey_Delete] = VK_DELETE;
    io.KeyMap[ImGuiKey_Backspace] = VK_BACK;
    io.KeyMap[ImGuiKey_Space] = VK_SPACE;
    io.KeyMap[ImGuiKey_Enter] = VK_RETURN;
    io.KeyMap[ImGuiKey_Escape] = VK_ESCAPE;
    
    io.KeyMap[ImGuiKey_A] = 'A';
    io.KeyMap[ImGuiKey_C] = 'C';
    io.KeyMap[ImGuiKey_V] = 'V';
    io.KeyMap[ImGuiKey_X] = 'X';
    io.KeyMap[ImGuiKey_Y] = 'Y';
    io.KeyMap[ImGuiKey_Z] = 'Z';

    return true;
}

void CImGUIAdaptor::Win32NewFrame()
{
    ImGuiIO& io = ImGui::GetIO();
    IM_ASSERT(io.Fonts->IsBuilt() && "Font atlas not built! It is generally built by the renderer "
                                     "back-end. Missing call to renderer _NewFrame() function? e.g. "
                                     "ImGui_ImplOpenGL3_NewFrame().");

    // Setup display size (every frame to accommodate for window resizing)
    RECT rect;
    //::GetClientRect(g_hWnd, &rect);
    io.DisplaySize = ImVec2(this->screenSize.x, this->screenSize.y);
 //   io.MouseDrawCursor = true;

    /*
  // Setup time step
  INT64 current_time;
  ::QueryPerformanceCounter((LARGE_INTEGER *)&current_time);
  io.DeltaTime = (float)(current_time - g_Time) / g_TicksPerSecond;
  g_Time = current_time;

  // Read keyboard modifiers inputs
  io.KeyCtrl = (::GetKeyState(VK_CONTROL) & 0x8000) != 0;
  io.KeyShift = (::GetKeyState(VK_SHIFT) & 0x8000) != 0;
  io.KeyAlt = (::GetKeyState(VK_MENU) & 0x8000) != 0;
  io.KeySuper = false;
  // io.KeysDown[], io.MousePos, io.MouseDown[], io.MouseWheel: filled by the
  WndProc handler below.

  // Update OS mouse position
  ImGui_ImplWin32_UpdateMousePos();

  // Update OS mouse cursor with the cursor requested by imgui
  ImGuiMouseCursor mouse_cursor = io.MouseDrawCursor ? ImGuiMouseCursor_None :
  ImGui::GetMouseCursor(); if (g_LastMouseCursor != mouse_cursor)
  {
      g_LastMouseCursor = mouse_cursor;
      ImGui_ImplWin32_UpdateMouseCursor();
  }

  // Update game controllers (if enabled and available)
  ImGui_ImplWin32_UpdateGamepads();
  */
}

void CImGUIAdaptor::CleanUP() {}

void CImGUIAdaptor::Invalidate() { ImGui_ImplDX9_InvalidateDeviceObjects(); }

void CImGUIAdaptor::updateMousePosition(Point2D position)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MousePos = ImVec2((float)position.x, (float)position.y);
}

void CImGUIAdaptor::updateKey(size_t keycode, bool isDown)
{
    ImGuiIO& io = ImGui::GetIO();
    io.KeysDown[keycode] = isDown;
    if(isDown)
    {
        // is a character code
        if(keycode >= 0x41 && keycode <= 0x5A)
        {
            // Append character
            auto baseCharacter = (io.KeyShift ? 'A':'a');
            io.AddInputCharacter(keycode-0x41+baseCharacter);
        }
        static std::unordered_map<size_t, size_t> specialKeyMapping = {
            {VK_OEM_COMMA, ','},
            {VK_OEM_MINUS, '-'},
            {VK_OEM_PERIOD, '.'},
            {VK_OEM_PERIOD, '.'}
        };

        if(specialKeyMapping.count(keycode) > 0)
        {
            io.AddInputCharacter(specialKeyMapping[keycode]);
        }
    }

    switch(keycode)
    {
        case VK_CONTROL:    io.KeyCtrl = isDown; break;
        case VK_SHIFT:      io.KeyShift = isDown; break;
        case VK_MENU:       io.KeyAlt = isDown; break;
    }
    //utilslib::Logger::getInfo() << "[ImGUI Hook] key: " << std::hex << keycode << " status: " << isDown << std::endl;
}
void CImGUIAdaptor::updateButton(unsigned short state)
{
   std::vector<std::tuple<unsigned short,unsigned short,unsigned short>> buttons = { {0, RI_MOUSE_LEFT_BUTTON_DOWN,RI_MOUSE_LEFT_BUTTON_UP}, {1, RI_MOUSE_RIGHT_BUTTON_DOWN, RI_MOUSE_RIGHT_BUTTON_UP}};
   ImGuiIO& io = ImGui::GetIO();
   for(auto &pair: buttons)
   {
       auto id = std::get<0>(pair);
       auto flagDown = std::get<1>(pair);
       auto flagUp = std::get<2>(pair);
       if(state & flagDown)
           io.MouseDown[id] = true;

       if(state & flagUp)
           io.MouseDown[id] = false;
   }
}

void CImGUIAdaptor::setMouseVisible(bool state)
{
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDrawCursor = state;
}
