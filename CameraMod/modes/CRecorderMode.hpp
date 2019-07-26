#ifndef CRECORDER_MODE_HPP
#define CRECORDER_MODE_HPP
#include "Windows.h"
#include "game/CGame.h"
#include "CModeController.hpp"
#include <utilslib/logger.hpp>


class CRecorderMode: public CGenericMode
{
    private:
        inline void recordMovementTick()
        {
            auto& globalRecordingState = this->m_modeController.m_getScene().m_recordingState;
            //glm::vec3 position = toGlm(this->m_gameController->GetPlayerPosition());
            //glm::vec3 rotation = toGlm(this->m_gameController->GetPlayerRotation());

            //PED_State& state = this->m_gameController->GetState();

           // globalRecordingState.m_recordedPath.push_back(CPlayerMovementFrame(position,rotation,state));
        }

        inline void playPlayerMovementTick()
        {
            auto& globalRecordingState = this->m_modeController.m_getScene().m_recordingState;
            auto framePosition = globalRecordingState.getCurrentReplayFrame();
            //this->m_gameController->SetPlayerPosition(toVec3D(framePosition.position));
            //this->m_gameController->SetPlayerRotation(toVec3D(framePosition.rotation));
            //this->m_gameController->SetState(framePosition.objectState);
        }
       CGenericObjectRecording* m_recording;
    public:
        CRecorderMode(): m_recording(nullptr) {}
        void setGameDriver(CGame* game) { this->m_gameController = game;}
        void setModeController(CModeController controller) {this->m_modeController = controller; }

        virtual bool onVKKey(USHORT key) {
            auto& globalRecordingState = this->m_modeController.m_getScene().m_recordingState;
            auto player = this->m_gameController->getLocalPlayer();
            switch(key)
            {
                // Enter
                case VK_RETURN:
                    if(globalRecordingState.isRunning())
                    {
                        globalRecordingState.setRecordingState(CRecordingStateEnum::IDLE);
                        m_recording = this->m_gameController->saveRecording(player);
                    } else {
                        globalRecordingState.setRecordingState(CRecordingStateEnum::RECORDING);
                        this->m_gameController->startRecording(player);
                    }
                    this->m_gameController->LockControls(false);
                    break;
                case VK_BACK:
                    globalRecordingState.setRecordingState(CRecordingStateEnum::PLAYING);
                    this->m_gameController->playRecording(player,m_recording);
                    this->m_gameController->LockControls(true);
                    this->m_gameController->PrintDebugMessage("Replay pressed");
                    break;
            }
            return false;
        }
        virtual void onMouseMove(int x, int y) {}
        virtual void onMouseButtons(unsigned short buttons) {}
        virtual void onTick() {}
        virtual void onRender() {
            auto& globalRecordingState = this->m_modeController.m_getScene().m_recordingState;
            static bool shouldRenderOverlay = true;
            bool shouldBeMoveable = true;
            static int corner = 0;
            float DISTANCE = 50;
            ImGuiIO& io = ImGui::GetIO();
            if (corner != -1)
            {
                ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
                ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
                ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            }
            auto player = this->m_gameController->getLocalPlayer();
            auto info = this->m_gameController->getRecordingInfo(player);
            size_t framesCount = 0;
            if(info)
            {
                framesCount = info->getFramesCount();
            }
            ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
            if (ImGui::Begin("Example: Simple overlay", &shouldRenderOverlay, (shouldBeMoveable != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
            {
                switch(globalRecordingState.getState())
                {
                    case CRecordingStateEnum::RECORDING:
                        ImGui::Text("Recording is running");
                        ImGui::Text("Frames: %d",framesCount);
                    break;
                    case CRecordingStateEnum::PLAYING:
                        ImGui::Text("Replaying");
                    break;
                    default:
                        ImGui::Text("Recording is not running");
                    break;
                }
                if (ImGui::BeginPopupContextWindow())
                {
                    if (ImGui::MenuItem("Custom",       NULL, corner == -1)) corner = -1;
                    if (ImGui::MenuItem("Top-left",     NULL, corner == 0)) corner = 0;
                    if (ImGui::MenuItem("Top-right",    NULL, corner == 1)) corner = 1;
                    if (ImGui::MenuItem("Bottom-left",  NULL, corner == 2)) corner = 2;
                    if (ImGui::MenuItem("Bottom-right", NULL, corner == 3)) corner = 3;
                    if (&shouldRenderOverlay && ImGui::MenuItem("Close")) shouldRenderOverlay = false;
                    ImGui::EndPopup();
                }
            }
        ImGui::End();
        }

        virtual void activate() {
            utilslib::Logger::getInfo() << "activate() recorder mode" << std::endl;
            this->m_gameController->PrintDebugMessage("Activated recorder");
        }
        virtual void deactivate() {
            this->m_gameController->LockControls(false);
        }
};
#endif
