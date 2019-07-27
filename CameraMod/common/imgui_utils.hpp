#ifndef CAMERAMOD_IMGUI_UTILS_HPP
#define CAMERAMOD_IMGUI_UTILS_HPP

#include <imgui.h>
#include <string>
#include <filesystem>
#include <cstdarg>

namespace ImGui {
namespace Utils {
    
    inline void TextColoredCentered(const ImVec4& col, const char* text,...)
    {
        auto width = ImGui::GetWindowWidth();
        auto box = ImGui::CalcTextSize(text);
        ImGui::Indent((width-box.x)/2);
        va_list list;
        va_start(list,text);
        ImGui::TextColoredV(col,text,list);
        va_end(list);
        ImGui::Unindent((width-box.x)/2);
    }
    inline void TextCentered(const char* text,...)
    {
        auto width = ImGui::GetWindowWidth();
        char buffer[1024];
        va_list list;
        va_start(list,text);
        vsprintf(buffer, text,list);
        auto box = ImGui::CalcTextSize(buffer);
        ImGui::Indent((width-box.x)/2);
        ImGui::TextV(text,list);
        va_end(list);
        ImGui::Unindent((width-box.x)/2);
    }
    inline bool FilePickerPlane(std::string& resultPath, bool isStoring, std::string directory = "")
    {
        static char currentPath[256] = "";
        ImGui::BeginChild("list",ImVec2(0,-30));
        for(auto& p: std::filesystem::directory_iterator(directory))
        {
            std::string& path = p.path().string();
            // Search for .txt files
            if(path.find(".txt") == path.npos)
                continue;
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
    inline bool  InputText(const char* label, std::string* str)
    {
        return ImGui::InputText(label, (char*)str->c_str(), str->capacity() + 1);
    }
 

}; // ImGUI::Utils
}; // ImGUI

#endif