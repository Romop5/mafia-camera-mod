#include "modes/CFreecamera.hpp"
#include <glm/glm.hpp> 
#include <glm/gtx/transform.hpp>

bool CFreecamera::onVKKey(USHORT key) {
    switch(m_state)
    {
        case FREECAMERA_FREE:
        {
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
        }
        break;
        case FREECAMERA_REPLAYING:
        {
            switch(key)
            {
                case VK_ESCAPE:
                    auto &player = m_modeController.m_getScene().m_player;
                break;
            }
        }
        break;
    }
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
    if(ImGui::Button("Add current camera as a point"))
    {
        points.push_back(CCameraPoint(this->position, this->rotation));
    }
    if(points.size() == 0)
    {
        ImGui::Text("No camera points available");
        return;
    }
    ImGui::BeginChild("left pane", ImVec2(100, 0), true);
    if (ImGui::ArrowButton("Up", ImGuiDir_Up)) {
        if(index > 0)
        {
            std::swap(points[index], points[index-1]);
            index--;
        }
    }
    ImGui::SameLine();
    if (ImGui::ArrowButton("Down", ImGuiDir_Down)) {
        if(index < points.size()-1)
        {
            std::swap(points[index], points[index+1]);
            index++;
        }
    }
    size_t i = 0;
    for(auto &cameraPoint: points)
    {
        char label[128];
        sprintf(label, "ID %d", i);
        if (ImGui::Selectable(label, index == i))
        {
            // teleport camera
            this->position = cameraPoint.m_point;
            this->rotation = cameraPoint.m_rotation;
            updateCamera();
            index = i;
        }
        i++;
    }
    ImGui::EndChild();
    ImGui::SameLine();

    // right
    ImGui::BeginGroup();
        ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing()));
            if(points.size() > 0)
            {
                auto &cameraPoint = points[index];
                auto &point = cameraPoint.m_point;
                auto &rotation = cameraPoint.m_rotation;
                ImGui::Text("Point: %f %f %f", point.x, point.y, point.z);                          
    
                static size_t indexTrack = 0;
                auto &tracks = this->m_modeController.m_getScene().m_cameraManager.getCameraTracks();
                if(tracks.size() > 0)
                {
                    auto &selectedTrack = tracks[indexTrack];
                    if (ImGui::BeginCombo("Track", selectedTrack.m_name.c_str(), 0)) 
                    {
                        size_t id = 0;
                        for(auto &track: tracks)
                        {
                            bool is_index = (id == indexTrack);
                            if (ImGui::Selectable(track.m_name.c_str(), is_index))
                                indexTrack = id;
                            if (is_index)
                                ImGui::SetItemDefaultFocus();
                            id++;
                        }
                        ImGui::EndCombo();
                    }
                
                    if(ImGui::Button("Add point to track"))
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
    
    if(ImGui::Button("Add new track"))
    {
        this->m_modeController.m_getScene().m_cameraManager.addNewTrack();
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
                        ImGui::Text("Count of points: %d", cameraTrack.getPoints().size());
                        if(ImGui::Button("Play track"))
                        {
                            auto &player = this->m_modeController.m_getScene().m_player;
                            player.setTrack(&cameraTrack);
                            player.reset();
                            this->m_state = FREECAMERA_REPLAYING;
                        }
                    }

                    if(ImGui::Button("Delete track"))
                    {
                        tracks.erase(tracks.begin()+index);
                    }

                    ImGui::EndTabItem();
                }
                if (ImGui::BeginTabItem("Points"))
                {
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

void CFreecamera::renderFreecameraHelper()
{
    if(ImGui::Begin("Freecamera helper"))
    {                          
        ImGui::Text("Position: %f %f %f", this->position.x, this->position.y, this->position.z);                          
        ImGui::Text("Rotation: %f %f %f", this->rotation.x, this->rotation.y, this->rotation.z);                          
        ImGui::SliderFloat("Flying speed", &m_cameraFlyingSpeed, 1.0f, 10.0f, "ratio = %.3f");
        if(ImGui::Button("Teleport camera to player"))
        {
            auto player = m_gameController->getLocalPlayer();
            auto transform = player->getTransform();
            this->position = glm::vec3(transform[0]);
            this->rotation = glm::vec3(transform[1]);
            this->updateCamera();
        }

        if(ImGui::Button("Add camera point"))
        {
            this->m_modeController.m_getScene().m_cameraManager.addCameraPoint(this->position, this->rotation);
        }
        ImGui::SameLine();
        if(ImGui::Button("Add track"))
        {
            this->m_modeController.m_getScene().m_cameraManager.addNewTrack();
        }
        if(ImGui::Button("Show points"))
        {
            shouldShowPointsManager = true;
        }
        ImGui::SameLine();
        if(ImGui::Button("Show tracks"))
        {
            shouldShowCameraTracksManager = true;
        } 
        ImGui::SameLine();
        if(ImGui::Button("Hide all"))
        {
            shouldShowCameraTracksManager = false;
            shouldShowPointsManager = false;
        }
    }
    ImGui::End();
}

void CFreecamera::renderReplayingPanel()
{
    auto &player = this->m_modeController.m_getScene().m_player;
    auto track = player.m_currentTrack;
    if(ImGui::Begin("Replaying controller",nullptr,ImGuiWindowFlags_AlwaysAutoResize))
    {  
        if(track) 
        {
            ImGui::Text("Replaying track: %s", track->m_name.c_str());
            if(ImGui::Button("Restart"))
            {
                player.reset();
            }
            auto stopActionText = (player.isStoped() ? "Play" : "Stop");
            ImGui::SameLine();
            if(ImGui::Button(stopActionText))
            {
                player.toggleStop(!player.isStoped());
            }

            float speed = player.getSpeed();
            if(ImGui::SliderFloat("Replay speed",&speed,0.25,4.0))
            {
                player.setSpeed(speed);
            }

            bool isRewinding = player.isRewinding();
            if(ImGui::Checkbox("Is rewinding",&isRewinding))
            {
                player.toggleRewinding(isRewinding);
            } 
            if(ImGui::Button("Next step"))
            {
                player.moveForward();
            }
        } 
        
        if(ImGui::Button("Exit replaying"))
        {
            player.setTrack(nullptr);
            this->m_state = FREECAMERA_FREE;
        }
    }
    ImGui::End();
}