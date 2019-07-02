#ifndef CRECORDER_MODE_HPP
#define CRECORDER_MODE_HPP
#include "Windows.h"
#include "CGame.h"
#include "CModeController.hpp"
#include <utilslib/logger.hpp>


class CRecorderMode: public CGenericMode
{
    private:
        inline void recordMovementTick()
        {
            auto& globalRecordingState = this->m_modeController.m_getScene().m_recordingState;
            glm::vec3 position = toGlm(this->m_gameController->GetPlayerPosition());
            globalRecordingState.m_recordedPath.push_back(CPlayerMovementFrame(position));
        }
        inline void startReplaying()
        {

        }
    public:
        CRecorderMode(): {}
        void setGameDriver(CGame* game) { this->m_gameController = game;}
        void setModeController(CModeController controller) {this->m_modeController = controller; }

        virtual bool onVKKey(USHORT key) {
            auto& globalRecordingState = this->m_modeController.m_getScene().m_recordingState;
            switch(key)
            {
                // Enter
                case VK_RETURN:
                    if(globalRecordingState.isRunning())
                    {
                        globalRecordingState.setRecordingState(CRecordingStateEnum::IDLE);
                    } else {
                        globalRecordingState.setRecordingState(CRecordingStateEnum::RECORDING);
                    }
                case VK_BACK:
                    globalRecordingState.setRecordingState(CRecordingStateEnum::);
            }
            return false;
        }
        virtual void onMouseMove(int x, int y) {}
        virtual void onMouseButtons(unsigned short buttons) {}
        virtual void onTick() {}
        virtual void onRender() {
            auto& globalRecordingState = this->m_modeController.m_getScene().m_recordingState;
            // If recording is running, then record current position
            if(globalRecordingState.m_isRecordingRunning)
            {
                this->recordMovementTick();
            }
            static bool shouldRenderOverlay = true;
            bool shouldBeMoveable = true;
            static int corner = 0;
            float DISTANCE = 10;
            ImGuiIO& io = ImGui::GetIO();
            if (corner != -1)
            {
                ImVec2 window_pos = ImVec2((corner & 1) ? io.DisplaySize.x - DISTANCE : DISTANCE, (corner & 2) ? io.DisplaySize.y - DISTANCE : DISTANCE);
                ImVec2 window_pos_pivot = ImVec2((corner & 1) ? 1.0f : 0.0f, (corner & 2) ? 1.0f : 0.0f);
                ImGui::SetNextWindowPos(window_pos, ImGuiCond_Always, window_pos_pivot);
            }

            ImGui::SetNextWindowBgAlpha(0.35f); // Transparent background
            if (ImGui::Begin("Example: Simple overlay", &shouldRenderOverlay, (shouldBeMoveable != -1 ? ImGuiWindowFlags_NoMove : 0) | ImGuiWindowFlags_NoDecoration | ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoSavedSettings | ImGuiWindowFlags_NoFocusOnAppearing | ImGuiWindowFlags_NoNav))
            {
                if(globalRecordingState.m_isRecordingRunning)
                {
                    ImGui::Text("Recording is running");
                    ImGui::Text("Frames: %d",globalRecordingState.m_recordedPath.size());
                } else {
                    ImGui::Text("Recording is not running");
                }
                ImGui::Separator();
                if (ImGui::IsMousePosValid())
                    ImGui::Text("Mouse Position: (%.1f,%.1f)", io.MousePos.x, io.MousePos.y);
                else
                    ImGui::Text("Mouse Position: <invalid>");
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
            this->m_gameController->writeToConsole(CGame::COLOR_RED, "Activated recorder");
        }
        virtual void deactivate() {}
};
#endif
