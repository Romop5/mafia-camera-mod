#include "CCore.h"
#include "imgui.h"

extern CCore* core;

CModControl::CModControl()
{
    this->p_bIsActive = false;
    this->p_bBlockInput = false;
    this->p_bControlState = CMOD_FREECAM;
    this->pickedPoint = -1;
    this->pickedPointSecond = -1;

    // player
    this->playingID = 0;
    this->playingTime = 1;
    this->playintSpeed = 100.0f;

}

bool CModControl::IsActive() { return this->p_bIsActive; }

void CModControl::OnVKKey(USHORT key)
{
}

void CModControl::OnMouseMove(int x, int y)
{
}

void CModControl::OnMouseButtons(unsigned short buttons)
{
}

void CModControl::ActivateFreeCam(bool state)
{
}

void CModControl::UpdateCam()
{
    core->getGame()->SetCameraPos(this->p_vectCamera, this->p_vectCameraRot);
}

bool CModControl::isFreeCam()
{
    return (this->p_bControlState == CMOD_FREECAM);
}

void CModControl::BlockInput(bool should) { this->p_bBlockInput = should; }
bool CModControl::IsInputBlocked() { return this->p_bBlockInput; }

unsigned char CModControl::GetState() { return this->p_bControlState; }

void CModControl::Render()
{
    static size_t counter = 0;
    ImGui::Begin("Hello, world!");                          // Create a window called "Hello, world!" and append into it.

            float f;
            ImGui::Text("This is some useful text.");               // Display some text (you can use a format strings too)

            ImGui::SliderFloat("float", &f, 0.0f, 1.0f);            // Edit 1 float using a slider from 0.0f to 1.0f

            if (ImGui::Button("Button"))                            // Buttons return true when clicked (most widgets return true when edited/activated)
                counter++;
            ImGui::SameLine();
            ImGui::Text("counter = %d", counter);

            ImGui::Text("Application average %.3f ms/frame (%.1f FPS)", 1000.0f / ImGui::GetIO().Framerate, ImGui::GetIO().Framerate);
            ImGui::End();

}

void CModControl::ToggleMod()
{
    if (this->p_bIsActive == true) {
        this->ActivateFreeCam(false);
        core->getGame()->CameraUnlock();
    } else {
        this->ActivateFreeCam(true);
    }
    this->p_bIsActive = !this->p_bIsActive;
}

void CModControl::Init()
{
}

CamPoint* CModControl::GetPointID(int ID)
{
    int size = points.size();
    for (int i = 0; i < size; i++) {
        if (points[i].uniqueID == ID)
            return &points[i];
    }
    return NULL;
}
