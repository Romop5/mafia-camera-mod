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
            static bool shouldShowPointsManager = true;
            static bool shouldShowCameraTracksManager = true;

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

            ImGui::ShowDemoWindow();

            /* Camera points window */
            //ImGui::SetNextWindowSize(ImVec2(300, 440), ImGuiCond_FirstUseEver);
            if (ImGui::Begin("Camera Points", &shouldShowPointsManager))
            {
                auto &points = this->m_modeController.m_getScene().m_cameraManager.getCameraPoints();
                static size_t selectedPoint = 0;
                if(selectedPoint >= points.size())
                    selectedPoint = 0;
                this->renderPointsManager(points,selectedPoint);
            }
            ImGui::End();

            /* Camera paths */
            //ImGui::SetNextWindowSize(ImVec2(300, 440), ImGuiCond_FirstUseEver);
            if (ImGui::Begin("Camera paths", &shouldShowCameraTracksManager))
            {
                this->renderTrackManager();
            }
            ImGui::End();
        };
    private:
        void renderPointsManager(CameraPointVector_t& points,size_t& index);
        void renderTrackManager();

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
