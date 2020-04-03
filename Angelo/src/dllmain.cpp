#include <Windows.h>
#include <utilslib/logger.hpp>
#include "imgui/imgui.h"
#include "angeloframework/angelo.hpp"

AngeloFramework::Framework framework;

BOOL APIENTRY DllMain(HMODULE hModule, DWORD ul_reason_for_call,
    LPVOID lpReserved)
{
    switch (ul_reason_for_call) {
    case DLL_PROCESS_ATTACH: {
        DisableThreadLibraryCalls(hModule);
        if (!framework.IsInMafiaProcess())
        {
            MessageBoxW(NULL, L"Angelo.dll hooked into wrong process. Please, choose one with Game.exe and ls3df.dll", L"Angelo failed", MB_OK | MB_ICONERROR);
            return false;
        }
        framework.Load();
        framework.AddRenderCallback(
        [&]
        {
            ImGui::SetNextWindowBgAlpha(0.5);
            ImGuiWindowFlags flags = 0;
            flags |= ImGuiWindowFlags_NoTitleBar;
            flags |= ImGuiWindowFlags_NoScrollbar;
            flags |= ImGuiWindowFlags_NoDecoration;
            flags |= ImGuiWindowFlags_NoSavedSettings;
            if (!ImGui::Begin("AngeloStatusBar", nullptr, flags))
            {
                ImGui::End();
                return;
            }
            ImGui::Text("Angelo Loaded");
            ImGui::End();

        }
        );
        break;
    }
    case DLL_THREAD_ATTACH:
    case DLL_THREAD_DETACH:
        break;
    case DLL_PROCESS_DETACH:
        break;
    }
    return TRUE;
}