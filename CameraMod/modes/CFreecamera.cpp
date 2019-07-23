#include "modes/CFreecamera.hpp"
#include <glm/glm.hpp> 
#include <glm/gtx/transform.hpp>

bool CFreecamera::onVKKey(USHORT key) {
    float frontVectorMagnitude = 0.0;
    float leftVectorMagnitude = 0.0;
    glm::vec3 upVector = glm::vec3(0.0,1.0,0.0);

    enum 
    {
        VK_A = 0x41,
        VK_S = 0x53,
        VK_W = 0x57,
        VK_D = 0x44,
    };
    switch(key)
    {
        case VK_W:
        case VK_UP: frontVectorMagnitude += m_cameraFlyingSpeed; break;
        case VK_S:
        case VK_DOWN: frontVectorMagnitude -= m_cameraFlyingSpeed; break;
        case VK_A:
        case VK_LEFT: leftVectorMagnitude += m_cameraFlyingSpeed; break;
        case VK_D:
        case VK_RIGHT: leftVectorMagnitude -= m_cameraFlyingSpeed; break;
    }

    glm::vec3 leftVector = glm::cross(rotation, upVector);

    this->position += leftVector*leftVectorMagnitude + rotation*frontVectorMagnitude;
    this->updateCamera();
    return true;
}


void CFreecamera::onMouseMove(int x, int y)
{
    this->angleX += x*0.01;
    this->angleY += y*0.01;

    //auto verticalMatrix = glm::rotate(this->angleY, glm::vec3(-1.0,0.0,0.0)); 
    //auto horizontalMatrix = glm::rotate(this->angleX, glm::vec3(0.0,1.0,0.0)); 
    //this->rotation = glm::vec3(horizontalMatrix*verticalMatrix*glm::vec4(1.0,0.0,0.0,0.0));
    this->rotation = glm::vec3(cos(this->angleY)*sin(this->angleX), sin(this->angleY), cos(this->angleX)*cos(this->angleY));
    updateCamera();
}

void CFreecamera::renderPointsManager(CameraPointVector_t& points,size_t& index)
{
    if(points.size() == 0)
    {
        ImGui::Text("No camera points available");
        return;
    }
    ImGui::BeginChild("left pane", ImVec2(100, 0), true);
    if (ImGui::ArrowButton("Up", ImGuiDir_Up)) {
        if(index > 0)
            std::swap(points[index], points[index-1]);
            index--;
    }
    ImGui::SameLine();
    if (ImGui::ArrowButton("Down", ImGuiDir_Down)) {
        if(index < points.size()-1)
            std::swap(points[index], points[index+1]);
            index++;
    }
    size_t i = 0;
    for(auto &cameraPoint: points)
    {
        char label[128];
        sprintf(label, "ID %d", i);
        if (ImGui::Selectable(label, index == i))
            index = i;
        i++;
    }
    ImGui::EndChild();
    ImGui::SameLine();

    // right
    ImGui::BeginGroup();
        ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            if(points.size() > 0)
            {
                auto &cameraPoint = points[index];
                auto &point = cameraPoint.m_point;
                auto &rotation = cameraPoint.m_rotation;
                ImGui::Text("Point: %f %f %f", point.x, point.y, point.z);                          
                if(ImGui::Button("Teleport camera to point"))
                {
                    this->position = point;
                    this->rotation = rotation;
                    updateCamera();
                }
                static size_t indexTrack = 0;
                auto &tracks = this->m_modeController.m_getScene().m_cameraManager.getCameraTracks();
                if(tracks.size() > 0)
                {
                    auto &selectedTrack = tracks[indexTrack];
                    if (ImGui::BeginCombo("combo 1", selectedTrack.m_name.c_str(), 0)) // The second parameter is the label previewed before opening the combo.
                    {
                        size_t id = 0;
                        for(auto &track: tracks)
                        {
                            bool is_index = (id == indexTrack);
                            if (ImGui::Selectable(track.m_name.c_str(), is_index))
                                indexTrack = id;
                            if (is_index)
                                ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
                            id++;
                        }
                        ImGui::EndCombo();
                    }
                
                    if(ImGui::Button("Add point to track:"))
                    {
                        tracks[indexTrack].addPoint(&cameraPoint);
                    }
                } else {
                    ImGui::Text("No tracks available - add track at first.");
                }
                ImGui::PushStyleColor(ImGuiCol_Button, (ImVec4)ImColor::HSV(0.0f,1.0f,1.0f));
                if(ImGui::Button("Delete point"))
                {
                    points.erase(points.begin()+index);
                    index = 0;
                }
                ImGui::PopStyleColor();
            }
        ImGui::EndChild();
    ImGui::EndGroup();
}


void CFreecamera::renderTrackManager()
{
    auto &tracks = this->m_modeController.m_getScene().m_cameraManager.getCameraTracks();
    if(tracks.size() == 0)
    {
        ImGui::Text("No camera tracks available");
        return;
    }

    // Input strings
    static char inputCameraTrackName[255];
    static int index = 0;
    ImGui::BeginChild("left pane", ImVec2(80, 0), true,ImGuiWindowFlags_HorizontalScrollbar);

    size_t i = 0;
    for(auto &track: tracks)
    {
        char label[128];
        sprintf(label, "%s", track.m_name.c_str());
        if (ImGui::Selectable(label, index == i))
        {
            index = i;
            sprintf(inputCameraTrackName, track.m_name.c_str());
        }
        i++;
    }
    ImGui::EndChild();
    ImGui::SameLine();

    // right
    ImGui::BeginGroup();
        ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
            if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
            {
                if (ImGui::BeginTabItem("Description"))
                {
                    if(tracks.size() > 0)
                    {
                        auto& cameraTrack = tracks[index];
                        
                        if(ImGui::InputText("Name", inputCameraTrackName, IM_ARRAYSIZE(inputCameraTrackName)))
                        {
                            tracks[index].m_name = inputCameraTrackName;
                        }
                        ImGui::Text("Points: %d", cameraTrack.getPoints().size());
                    }

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Points"))
                {
                    ImGui::Text("ID: 0123456789");
                    auto& cameraTrack = tracks[index];
                    static size_t pointIndex = 0;
                    if(cameraTrack.getPoints().size() <= pointIndex)
                        pointIndex = 0;
                    this->renderPointsManager(cameraTrack.getPoints(),pointIndex);
                    ImGui::EndTabItem();
                }
                ImGui::EndTabBar();
            }
        ImGui::EndChild();
    ImGui::EndGroup();
}