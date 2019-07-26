#ifndef CFREECAMERA_HPP
#define CFREECAMERA_HPP
#include "common/structures.h"
#include "modes/CGenericMode.hpp"
#include <utilslib/logger.hpp>
#include <imgui.h>

class CFreecamera: public CGenericMode
{
    public:
        CFreecamera(): angleX(0), angleY(0), m_cameraFlyingSpeed(1.0), m_state(FREECAMERA_FREE) {}
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
            // Render free camera
            if(m_state == FREECAMERA_FREE)
            {
                renderFreecameraHelper();

                /* Camera points window */
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
                if (ImGui::Begin("Camera paths", &shouldShowCameraTracksManager))
                {
                    this->renderTrackManager();
                }
                ImGui::End();
            }
            if(m_state == FREECAMERA_REPLAYING)
            {
                // Render manager
                if(m_modeController.m_isGUIVisible())
                {
                    renderReplayingPanel();
                }
                auto &player = this->m_modeController.m_getScene().m_player;
                player.updateCamera(position,rotation);
                updateCamera();
            }
        };
    private:
        bool shouldShowPointsManager;
        bool shouldShowCameraTracksManager;

        enum State
        {
            FREECAMERA_FREE = 0,
            FREECAMERA_REPLAYING,
        } m_state;

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

        void renderFreecameraHelper();
        void renderReplayingPanel();

};
#endif
