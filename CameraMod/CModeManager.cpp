#include "imgui.h"
#include "CModeManager.hpp"
#include "CModeController.hpp"
#include "CFreecamera.hpp"
#include <sstream>

CModeManager::CModeManager()
{
    this->m_currentMode = NULL;
    this->m_InputBlockingStatus = 0;
    this->m_isGUIVisible = false;
}

void CModeManager::setCoreController(CCoreController controller)
{
    this->m_coreController = controller;
}

CModeManager::~CModeManager()
{
    if(this->m_currentMode)
    {
        this->m_currentMode->deactivate();
    }
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
    controller.m_getScene = [&] ()->CScene& {
        return this->m_scene;
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
        case VK_F2:
            //this->m_isGUIVisible = !this->m_isGUIVisible;
            this->turnGUIstate(!this->m_isGUIVisible);
    }

    if(!this->m_isGUIVisible)
    {
        if(this->m_currentMode)
        return this->m_currentMode->onVKKey(key);
    }
}

void CModeManager::OnMouseMove(int x, int y)
{
    if(!this->m_isGUIVisible)
        if(this->m_currentMode)
            this->m_currentMode->onMouseMove(x,y);
}

void CModeManager::OnMouseButtons(unsigned short buttons)
{
    if(!this->m_isGUIVisible)
        if(this->m_currentMode)
            this->m_currentMode->onMouseButtons(buttons);
}

void CModeManager::Render()
{
    if(!this->m_isGUIVisible)
        return;

    if(this->m_currentMode)
        this->m_currentMode->onRender();
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

void CModeManager::switchToMode(size_t id)
{
    // deactivate current mode
    this->m_modes[m_currentModeIndex]->deactivate();
    this->m_currentModeIndex = id;
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

void CModeManager::turnGUIstate(bool shouldBeOn)
{
    this->updateBlocking(shouldBeOn, BlockStatus::BLOCK_GUI);
    this->m_isGUIVisible = shouldBeOn;
    this->m_coreController.m_blockGUIInput(shouldBeOn);
}
