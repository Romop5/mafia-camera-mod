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
            if(ImGui::Button("Add track"))
            {
                this->m_modeController.m_getScene().m_cameraManager.addNewTrack();
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
                auto &points = this->m_modeController.m_getScene().m_cameraManager.getCameraPoints();
                static size_t selectedPoint = 0;
                if(selectedPoint >= points.size())
                    selectedPoint = 0;
                this->renderPointsManager(points,selectedPoint);
            }
            ImGui::End();

            /* Camera paths */
            ImGui::SetNextWindowSize(ImVec2(300, 440), ImGuiCond_FirstUseEver);
            bool p_Pathopen = true;
            if (ImGui::Begin("Camera paths", &p_Pathopen, ImGuiWindowFlags_MenuBar))
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
