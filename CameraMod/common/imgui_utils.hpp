#ifndef CAMERAMOD_IMGUI_UTILS_HPP
#define CAMERAMOD_IMGUI_UTILS_HPP

#include <imgui.h>
#include <string>
#include <filesystem>
#include <cstdarg>

namespace ImGui {
namespace Utils {
    static ImVec4 COLOR_WHITE = ImVec4(1.0,1.0,1.0,1.0);
    static ImVec4 COLOR_RED = ImVec4(1.0,0.0,0.0,1.0);
    static ImVec4 COLOR_GREEN = ImVec4(0.0,1.0,0.0,1.0);
    static ImVec4 COLOR_BLUE = ImVec4(0.0,0.0,1.0,1.0);

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

    inline bool  InputTextMultiline(const char* label, std::string* str)
    {
        return ImGui::InputTextMultiline(label, (char*)str->c_str(), str->capacity() + 1);
    }

    /**
     * @brief Starts a window with side menu, showing 'items' in menu
     * 
     * @param items 
     * @return size_t ID of selected item, 0 when not selected
     */
    inline void BeginSelectorWithSideMenu(const std::vector<std::string> items,size_t* selectedID,bool allowFiltering = true)
    {
        ImGui::BeginChild("left pane", ImVec2(150, 0), true,ImGuiWindowFlags_HorizontalScrollbar);
        auto filterId = ImGui::GetID("filter");
        static std::unordered_map<ImGuiID,ImGuiTextFilter> filters;
        auto& filter = filters[filterId];

        if(ImGui::CollapsingHeader("Filter"))
        {
            filter.Draw("");
        }
        size_t i = 0;
        for(auto &item: items)
        {
            // Skip item if it doesn't match
            if(!filter.PassFilter(item.c_str()))
                continue;
            ImGui::PushID(i);
            if (ImGui::Selectable(item.c_str(), *selectedID == i))
            {
                *selectedID = i;
            }
            ImGui::PopID();
            i++;
        }
        if(*selectedID >= i)
            i = 0;
        ImGui::EndChild();
        ImGui::SameLine();

        // right
        ImGui::BeginGroup();
        ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));    
    }
    
    inline void EndSelectorWithSideMenu()
    {
        ImGui::EndChild();
        ImGui::EndGroup();
    }

    inline bool TextColoredButton(ImVec4 color, const std::string str)
    {
        ImGui::PushStyleColor(ImGuiCol_Button,ImVec4(0.0,0.0,0.0,0.0));
        ImGui::PushStyleColor(ImGuiCol_Text,color);
        //auto size = CalcTextSize(str.c_str());
        auto size = ImVec2(30.0,20.0);
        auto result = ImGui::SmallButton(str.c_str());
        ImGui::PopStyleColor(2);
        return result;
    }
}; // ImGUI::Utils
}; // ImGUI

#endif