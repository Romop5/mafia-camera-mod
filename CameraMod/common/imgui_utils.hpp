#ifndef CAMERAMOD_IMGUI_UTILS_HPP
#define CAMERAMOD_IMGUI_UTILS_HPP

#include <imgui.h>
#include <string>
#include <filesystem>

namespace ImGui {
namespace Utils {
    
    inline bool FilePickerPlane(std::string& resultPath, bool isStoring, std::string directory = "")
    {
        static char currentPath[256] = "";
        ImGui::BeginChild("list",ImVec2(0,-30));
        for(auto& p: std::filesystem::directory_iterator(directory))
        {
            std::string& path = p.path().string();
            if(ImGui::Selectable(path.c_str()))
            {
                strcpy(currentPath, path.c_str());
            }
        }
        ImGui::EndChild();

        ImGui::BeginChild("inputbar",ImVec2(0,30));
        auto buttonLabel = (isStoring ? "Store" : "Load");
        auto status = false;
        if(ImGui::Button(buttonLabel))
        {
            resultPath = currentPath;
            status = true;
        }
        ImGui::SameLine();
        ImGui::InputText("File",currentPath,255);
        ImGui::EndChild();
        return status;
    }
}; // ImGUI::Utils
}; // ImGUI

#endif