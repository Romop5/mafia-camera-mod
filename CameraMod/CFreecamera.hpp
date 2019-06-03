#ifndef CFREECAMERA_HPP
#define CFREECAMERA_HPP
#include "structures.h"
#include "CGenericMode.hpp"
#include <utilslib/logger.hpp>
#include <glm/vec3.hpp> // glm::vec3
#include <imgui.h>

class CFreecamera: public CGenericMode
{
    public:
        CFreecamera(): angleX(0), angleY(0) {}
        virtual void onTick() {}
        virtual bool onVKKey(USHORT key);
        virtual void onMouseMove(int x, int y);

        virtual void activate() override {
            this->position = this->toGlm(m_gameController->GetPlayerPosition());
            this->rotation = this->toGlm(m_gameController->GetPlayerRotation());
            updateCamera();
            utilslib::Logger::getInfo() << "activated FreeCamera" << std::endl;

            // Block ingame input
            this->m_modeController.m_blockInput(true);

            this->m_gameController->writeToConsole(CGame::COLOR_RED, "Activated camera");
        }
        virtual void deactivate() override {
            this->m_gameController->CameraUnlock();
            // Unblock ingame input
            this->m_modeController.m_blockInput(false);
        }
        virtual void onRender() override {
            static size_t counter = 0;
            ImGui::Begin("Freecamera helper");                          
            ImGui::Button("Teleport camera to player");
            ImGui::End();

        };
    private:
        glm::vec3 position;
        glm::vec3 rotation;

        float angleX;
        float angleY;

        void updateCamera()
        {
            m_gameController->SetCameraPos(this->toVec3D(position), rotation.x,rotation.y,rotation.z,0.0f);
        }

        glm::vec3 toGlm(Vector3D& vec)
        {
            return glm::vec3(vec.x,vec.y,vec.z);
        }

        Vector3D toVec3D(glm::vec3& vec)
        {
            Vector3D vector;
            vector.x = vec[0];
            vector.y = vec[1];
            vector.z = vec[2];
            return vector;
        }

};
#endif
