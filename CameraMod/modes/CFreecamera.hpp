#ifndef CFREECAMERA_HPP
#define CFREECAMERA_HPP
#include "common/structures.h"
#include "modes/CGenericMode.hpp"
#include <utilslib/logger.hpp>
#include <imgui.h>

class CFreecamera: public CGenericMode
{
    public:
        CFreecamera(): angleX(0), angleY(0), m_cameraFlyingSpeed(1.0) {}
        virtual void onTick() {}
        virtual bool onVKKey(USHORT key);
        virtual void onMouseMove(int x, int y);

        virtual void activate() override {
            this->position = glm::vec3(m_gameController->GetCameraTransform()[0]);
            this->rotation = glm::vec3(m_gameController->GetCameraTransform()[1]);
            updateCamera();
            utilslib::Logger::getInfo() << "activated FreeCamera" << std::endl;

            // Block ingame input
            this->m_modeController.m_blockInput(true);
            this->m_gameController->PrintDebugMessage("Activated camera");

            this->m_gameController->ToggleHUD(true);
        }
        virtual void deactivate() override {
            this->m_gameController->UnlockCameraTransform();
            // Unblock ingame input
            this->m_modeController.m_blockInput(false);
            this->m_gameController->ToggleHUD(false);
            this->m_gameController->PrintDebugMessage("Deactivated camera");
            
        }
        virtual void onRender() override {
            static size_t counter = 0;
            ImGui::Begin("Freecamera helper");                          
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

            ImGui::End();

            /*
            ImGui::Begin("Camera points");                          
            auto &points = this->m_modeController.m_getScene().getCameraPoints();
            size_t id = 0;
            for(auto &cameraPoint: points)
            {
                if(ImGui::TreeNode((void*) (intptr_t) id, "Point %d", id))
                {
                    auto &point = cameraPoint.m_point;
                    ImGui::Text("Point: %f %f %f", point.x, point.y, point.z);                          
                    if(ImGui::Button("Teleport camera"))
                    {
                        this->position = point;
                        updateCamera();
                    }
                    ImGui::TreePop();

                }
                id++;
            }
            ImGui::End();
            */

            ImGui::ShowDemoWindow();

            /* Camera points window */
            ImGui::SetNextWindowSize(ImVec2(300, 440), ImGuiCond_FirstUseEver);
            bool p_open = true;
            if (ImGui::Begin("Camera Points", &p_open, ImGuiWindowFlags_MenuBar))
            {
                // left
                static int selected = 0;
                ImGui::BeginChild("left pane", ImVec2(50, 0), true);

                size_t i = 0;
                auto &points = this->m_modeController.m_getScene().m_cameraManager.getCameraPoints();
                for(auto &cameraPoint: points)
                {
                    char label[128];
                    sprintf(label, "%d", i);
                    if (ImGui::Selectable(label, selected == i))
                        selected = i;
                    i++;
                }
                ImGui::EndChild();
                ImGui::SameLine();

                // right
                ImGui::BeginGroup();
                    ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
                        //ImGui::Text("MyObject: %d", selected);
                        //ImGui::Separator();
                        if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
                        {
                            if (ImGui::BeginTabItem("Description"))
                            {
                                if(points.size() > 0)
                                {
                                    auto &cameraPoint = points[selected];
                                    auto &point = cameraPoint.m_point;
                                    ImGui::Text("Point: %f %f %f", point.x, point.y, point.z);                          
                                    if(ImGui::Button("Teleport camera"))
                                    {
                                        this->position = point;
                                        updateCamera();
                                    }

                                    size_t selectedTrack = 0;
                                    auto &tracks = this->m_modeController.m_getScene().m_cameraManager.getCameraTracks();
                                     if (ImGui::BeginCombo("combo 1", "wat", 0)) // The second parameter is the label previewed before opening the combo.
                                     {
                                        size_t id = 0;
                                        for(auto &track: tracks)
                                        {
                                            bool is_selected = (id == selectedTrack);
                                            if (ImGui::Selectable(track.m_name.c_str(), is_selected))
                                                selectedTrack = id;
                                            if (is_selected)
                                                ImGui::SetItemDefaultFocus();   // Set the initial focus when opening the combo (scrolling + for keyboard navigation support in the upcoming navigation branch)
                                            id++;
                                        }
                                        ImGui::EndCombo();
                                     }
                                    if(ImGui::Button("Add point to track:"))
                                    {
                                        tracks[selectedTrack].addPoint(&cameraPoint);
                                    }
                                }

                                ImGui::EndTabItem();
                            }
                            if (ImGui::BeginTabItem("Details"))
                            {
                                ImGui::Text("ID: 0123456789");
                                ImGui::EndTabItem();
                            }
                            ImGui::EndTabBar();
                        }
                    ImGui::EndChild();
                ImGui::EndGroup();
            }
            ImGui::End();

            /* Camera paths */
            ImGui::SetNextWindowSize(ImVec2(300, 440), ImGuiCond_FirstUseEver);
            bool p_Pathopen = true;
            if (ImGui::Begin("Camera paths", &p_Pathopen, ImGuiWindowFlags_MenuBar))
            {
                // left
                static int selected = 0;
                ImGui::BeginChild("left pane", ImVec2(50, 0), true);

                size_t i = 0;
                auto &tracks = this->m_modeController.m_getScene().m_cameraManager.getCameraTracks();
                for(auto &track: tracks)
                {
                    char label[128];
                    sprintf(label, "%d", i);
                    if (ImGui::Selectable(label, selected == i))
                        selected = i;
                    i++;
                }
                ImGui::EndChild();
                ImGui::SameLine();

                // right
                ImGui::BeginGroup();
                    ImGui::BeginChild("item view", ImVec2(0, -ImGui::GetFrameHeightWithSpacing())); // Leave room for 1 line below us
                        //ImGui::Text("MyObject: %d", selected);
                        //ImGui::Separator();
                        if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None))
                        {
                            if (ImGui::BeginTabItem("Description"))
                            {
                                if(tracks.size() > 0)
                                {
                                    auto cameraTrack = tracks[selected];
                                    char cameraTrackName[255];
                                    ImGui::InputText("Name: ", cameraTrackName, IM_ARRAYSIZE(cameraTrackName));
                                    char numberOfPoints[255];
                                    sprintf(numberOfPoints, "Points: %d", cameraTrack.getPoints().size());
                                    ImGui::Text(numberOfPoints);
                                }

                                ImGui::EndTabItem();
                            }
                            if (ImGui::BeginTabItem("Details"))
                            {
                                ImGui::Text("ID: 0123456789");
                                ImGui::EndTabItem();
                            }
                            ImGui::EndTabBar();
                        }
                    ImGui::EndChild();
                ImGui::EndGroup();
            }
            ImGui::End();



        };
    private:
        float m_cameraFlyingSpeed;

        glm::vec3 position;
        glm::vec3 rotation;

        float angleX;
        float angleY;

        void updateCamera()
        {
            glm::mat4 transform; 
            transform[0] = glm::vec4(position,1.0);
            transform[1] = glm::vec4(rotation,1.0);
            m_gameController->SetCameraTransform(transform);
        }

};
#endif
