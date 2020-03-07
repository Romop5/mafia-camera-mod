#include "imgui.h"
#include "modes/CModeManager.hpp"
#include "modes/CModeController.hpp"
#include "modes/CFreecamera.hpp"
#include "modes/CRecorderMode.hpp"
#include "modes/CAbout.hpp"
#include "modes/CSandbox.hpp"
#include "common/imgui_utils.hpp"
#include <sstream>
#include <filesystem>

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
    AddMode(new CAbout(), "About"); // About
    AddMode(new CFreecamera(), "Free camera");
    AddMode(new CGenericMode(), "Regular game"); // Regular 1st person gameplay
    AddMode(new CRecorderMode(), "Recorder"); // Regular 1st person gameplay
    AddMode(new CSandbox(), "Sandbox"); // Regular 1st person gameplay

    // create controller
    CModeController controller;
    controller.m_blockInput = [&](bool should)->void {
        this->updateBlocking(should, BlockStatus::BLOCK_MODE);
    };
    controller.m_getScene = [&] ()->CScene& {
        return this->m_scene;
    };
    controller.m_isGUIVisible = [&] ()->bool {
        return this->m_isGUIVisible;
    };
    controller.m_getGraphics = [&] ()->CGraphics* {
        return this->m_coreController.m_getGraphics();
    };
    
    // Set game to all of them
    for(auto mode: this->m_modes)
    {
        mode.first->setGameDriver(game);
        mode.first->setModeController(controller);
    }

    this->m_currentMode = m_modes[0].first;
    this->m_currentMode->activate();

    this->m_currentModeIndex = 0;

    this->m_gameController = game;
}

void CModeManager::AddMode(CGenericMode* mode, const std::string name)
{
     this->m_modes.push_back(std::make_pair(mode,name));
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
    return true;
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

void CModeManager::RenderMainMenu()
{
    ImGui::BeginMainMenuBar();
    if (ImGui::BeginMenu("Modes"))
    {
        for(size_t i = 0; i < m_modes.size(); i++)
        {
            auto& item = m_modes[i];
            if (ImGui::MenuItem(item.second.c_str(), "")) 
            {
                this->switchToMode(i);
            }
        }
        if (ImGui::MenuItem("Exit mod"))
        {
            this->m_coreController.m_exitMod();
        }
        ImGui::EndMenu();
    }
    bool openDialog = false;
    static bool isSaveDialog = false;
    if (ImGui::BeginMenu("Storage"))
    {
        if (ImGui::MenuItem("Save")) 
        {
            openDialog = true;
            isSaveDialog = true;
        }
        if (ImGui::MenuItem("Load")) 
        {
            openDialog = true;
            isSaveDialog = false;
        }
        ImGui::EndMenu();
    }
    ImGui::EndMainMenuBar();

    if(openDialog)
        ImGui::OpenPopup("savedialog");
    ImGui::SetNextWindowSize(ImVec2(400,200));
    if(ImGui::BeginPopup("savedialog",ImGuiWindowFlags_AlwaysAutoResize | ImGuiWindowFlags_NoScrollbar))
    {
        ImGui::Text("Choose file destination");
        std::string path;
        if(ImGui::Utils::FilePickerPlane(path,isSaveDialog))
        {
            std::string directory = std::filesystem::current_path().string();
            if(isSaveDialog)
            {
                if(this->m_scene.save(directory+"/"+path))
                {
                    m_gameController->PrintDebugMessage("Saved successfully");
                }
            } else {
                if(this->m_scene.load(directory+"/"+path))
                    m_gameController->PrintDebugMessage("Loaded successfully");
            }
            ImGui::CloseCurrentPopup();
        }
        ImGui::EndPopup(); 
    }
}
void CModeManager::Render()
{
    if(this->m_isGUIVisible)
    {
        this->RenderMainMenu();
    }

    if(this->m_currentMode)
        this->m_currentMode->onRender();
}

void CModeManager::switchToNextMode()
{
    // deactivate current mode
    this->m_modes[m_currentModeIndex].first->deactivate();
    this->m_currentModeIndex++;
    if(m_currentModeIndex >= this->m_modes.size())
        this->m_currentModeIndex = 0;
    // Activate the new one
    this->m_modes[m_currentModeIndex].first->activate();
    this->m_currentMode = this->m_modes[this->m_currentModeIndex].first;
}

void CModeManager::switchToMode(size_t id)
{
    // deactivate current mode
    this->m_modes[m_currentModeIndex].first->deactivate();
    this->m_currentModeIndex = id;
    if(m_currentModeIndex >= this->m_modes.size())
        this->m_currentModeIndex = 0;
    // Activate the new one
    this->m_modes[m_currentModeIndex].first->activate();
    this->m_currentMode = this->m_modes[this->m_currentModeIndex].first;
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
