#include "imgui.h"
#include "CModeManager.hpp"
#include "CModeController.hpp"
#include "CFreecamera.hpp"
#include <sstream>

CModeManager::CModeManager()
{
    this->m_currentMode = NULL;
    this->m_InputBlockingStatus = 0;
}

void CModeManager::setCoreController(CCoreController controller)
{
    this->m_coreController = controller;
}

void CModeManager::InitializeModes(CGame* game)
{
    utilslib::Logger::getInfo() << "CModeManager::InitializeModes" << std::endl;
    this->m_modes.push_back(new CFreecamera());
    this->m_modes.push_back(new CGenericMode()); // Regular 1st person gameplay

    // create controller
    CModeController controller;
    controller.m_blockInput = [&](bool should)->void {
        this->updateBlocking(should, BlockStatus::BLOCK_MODE);
    };
    
    // Set game to all of them
    for(auto mode: this->m_modes)
    {
        mode->setGameDriver(game);
        mode->setModeController(controller);
    }

    this->m_currentMode = m_modes[0];
    this->m_currentMode->activate();

    this->m_currentModeIndex = 0;

}
bool CModeManager::OnVKKey(USHORT key)
{
    switch(key)
    {
        case VK_F1:
            this->switchToNextMode();
    }
    if(this->m_currentMode)
    return this->m_currentMode->onVKKey(key);
}

void CModeManager::OnMouseMove(int x, int y)
{
    if(this->m_currentMode)
        this->m_currentMode->onMouseMove(x,y);
}

void CModeManager::OnMouseButtons(unsigned short buttons)
{
    if(this->m_currentMode)
        this->m_currentMode->onMouseButtons(buttons);
}

void CModeManager::Render()
{
    if(this->m_currentMode)
        this->m_currentMode->onRender();
    static size_t counter = 0;
    ImGui::Begin("Mod status!");                          // Create a window called "Hello, world!" and append into it.

    ImGui::Text("This is some text: %d", m_currentModeIndex);
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

void CModeManager::switchToNextMode()
{
    // deactivate current mode
    this->m_modes[m_currentModeIndex]->deactivate();
    this->m_currentModeIndex++;
    if(m_currentModeIndex >= this->m_modes.size())
        this->m_currentModeIndex = 0;
    // Activate the new one
    this->m_modes[m_currentModeIndex]->activate();
    this->m_currentMode = this->m_modes[this->m_currentModeIndex];
}

void CModeManager::updateBlocking(bool shouldBlock, BlockStatus type)
{
    auto oldStatus = this->m_InputBlockingStatus;
    if(shouldBlock)
    {
        this->m_InputBlockingStatus |= static_cast<size_t>(type);
    } else 
    {
        this->m_InputBlockingStatus &= ~(static_cast<size_t>(type));
    }
    
    if(this->m_InputBlockingStatus == oldStatus)
        return;
    // update blocking if neccessary
    this->m_coreController.m_blockGameInput(this->m_InputBlockingStatus > 0); 
    utilslib::Logger::getInfo() << "Input status: " << this->m_InputBlockingStatus << std::endl;
}
