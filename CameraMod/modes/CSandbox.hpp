#ifndef CSANDBOX_MODE_HPP
#define CSANDBOX_MODE_HPP
#include "Windows.h"
#include "game/CGenericGame.hpp"
#include "modes/CModeController.hpp"
#include "modes/CGenericMode.hpp"
#include "modes/sandbox/CScriptInspector.hpp"
#include <utilslib/logger.hpp>
#include "version.hpp"
#include "common/imgui_utils.hpp"
#include <sstream>
#include <set>
#include <unordered_map>

using originalDelete_t = void __fastcall (Script* script);
using originalUpdate_t = void __fastcall (Script* script,DWORD EDX, DWORD deltaTime);

originalDelete_t* g_originalDeleter = nullptr;
originalUpdate_t* g_originalUpdater = nullptr;

originalDelete_t* g_originalDeleter1 = nullptr;
originalUpdate_t* g_originalUpdater1 = nullptr;

std::set<Script*> g_perFrameScripts;

class ScriptHandleMap
{
    using map_t = std::unordered_map<Script*, ScriptInspector>;
    map_t m_map;
    public:
    void update(Script* script)
    {
        if(m_map.count(script) == 0)
        {
            m_map[script] = ScriptInspector(script);
        }
    }
    ScriptInspector& getAtIndex(const size_t id) {
        size_t i = 0;
        for(auto& pair: m_map)
        {
            if(i == id)
                return pair.second;
            i++;
        }
        return (*m_map.begin()).second;
    }
    map_t& getMap() { return m_map; }
};

static ScriptHandleMap g_ScriptMap;

CGame* g_game = nullptr;

class CSandbox: public CGenericMode
{
    protected:
        ImFont* m_font;
    public:
        CSandbox(){
            ImGuiIO& io = ImGui::GetIO();
            m_font = io.Fonts->AddFontDefault();
            m_font = io.Fonts->AddFontFromFileTTF("Roboto-Medium.ttf", 50);
        }

        virtual bool onVKKey(USHORT key) {return false;}
        virtual void onMouseMove(int x, int y) {}
        virtual void onMouseButtons(unsigned short buttons) {}
        virtual void onTick() {}
        virtual void renderScriptMenuWidget()
        {
            ImGui::Begin("List of ingame scripts", nullptr,0);
            {
                static std::vector<std::string> menuItems;
                menuItems.clear();
                for(auto pair: g_ScriptMap.getMap())
                {
                    menuItems.push_back(pair.first->getName());
                }
                static size_t selectedID = 0;
                std::stringstream ss; 
                ss << "Num. of scripts: " << g_ScriptMap.getMap().size();
                ImGui::Text(ss.str().c_str());
                ImGui::Utils::BeginSelectorWithSideMenu(menuItems,&selectedID);
                auto& selectedWindow = g_ScriptMap.getAtIndex(selectedID);
                auto selectedScript = selectedWindow.getScript();
                if(ImGui::Button("Open manipulator"))
                {
                    selectedWindow.setVisible(!selectedWindow.isVisible());
                }
                std::stringstream scriptPreview;
                scriptPreview << selectedScript->getName() << "\n"
                    << selectedScript->getSource();
                ImGui::Text(scriptPreview.str().c_str());
                ImGui::Utils::EndSelectorWithSideMenu();
            }
            ImGui::End();
        }
        virtual void renderObjectsMenuWidget()
        {
            ImGui::Begin("List of objects", nullptr,0);
            {
                auto scene = CMafiaContext::getInstance()->getScene();
                static std::vector<std::string> menuItems;
                menuItems.clear();
                //auto objectPool = scene->m_scriptedObjects;
                auto& objectPool = CMafiaContext::getObjectPool();
                auto poolSize = objectPool.getLength();

                for(auto i = 0; i < poolSize; i++)
                {
                    auto object = objectPool[i];
                    if(object->frame)
                    {
                        menuItems.push_back(object->frame->getName());
                    } else {
                        menuItems.push_back(std::string("Object ID: ")+std::to_string(i));
                    }
                }
                static size_t selectedID = 0;
                
                std::stringstream objectsInfo;
                objectsInfo << objectPool.getLength();

                ImGui::Text(objectsInfo.str().c_str());

                ImGui::Utils::BeginSelectorWithSideMenu(menuItems,&selectedID);
                auto selectedObject = objectPool[selectedID];
                auto selectedFrame = selectedObject->frame;
                
                std::stringstream info;
                if(selectedFrame)
                    info << "Name: " << selectedObject->frame->getName()<< "\n";
                info << "Type: " << selectedObject->getObjectType() << "\n";
                auto pos = selectedObject->position;
                info << "Pos: " << "[" << pos.x << "," << pos.y << "," << pos.z << "]\n";
                auto rot = selectedObject->rotation; 
                info << "Rot: " << "[" << rot.x << "," << rot.y << "," << rot.z << "]\n";
                info << "IsActive:" << selectedObject->isActive << "\n";
                
                switch(selectedObject->objectType)
                {
                    case OBJECT_ENEMY:
                    case OBJECT_LOCALPLAYER:
                    {
                        auto ped = reinterpret_cast<PED*>(selectedObject);
                        info << "Health:" << ped->health << "\n";
                        info << "Is reloading:" << ped->isReloading << "\n";
                    }
                    break;
                    case OBJECT_VEHICLE:
                    {
                        auto veh = reinterpret_cast<VEHICLE*>(selectedObject);
                        info << "Petrol level:" << veh->petrolLevel << "\n";
                        info << "Engine damage:" << veh->engineDamage << "\n";
                        info << "Is horn activated:" << veh->hornState << "\n";
                        info << "Is siren activated:" << veh->sirenState << "\n";
                    }
                    break;
                }

                ImGui::Text(info.str().c_str());

                if(ImGui::Button("Teleport local player to object"))
                {
                    auto mafiaGame = reinterpret_cast<CGame*>(this->m_gameController);
                    auto player = static_cast<CLocalPlayer*>(mafiaGame->getLocalPlayer());
                    auto ped = reinterpret_cast<PED*>(player->getMafiaAddress());
                    ped->object.position = pos;
                }
                ImGui::Utils::EndSelectorWithSideMenu();
            }
            ImGui::End();
        }
        virtual void onRender() {

            ImGui::ShowDemoWindow();
            
            auto mafiaGame = reinterpret_cast<CGame*>(this->m_gameController);
            auto machine = mafiaGame->getScriptMachine();
            // Add scripts to list of all scripts
            for(auto script = machine->m_scriptsPoolStart; script != machine->m_scriptsPoolEnd; script++)
            {
                g_ScriptMap.update(*script);
            }
    
            for(auto& scriptWindow: g_ScriptMap.getMap())
            {
                auto& window = scriptWindow.second;
                window.render();
            }

            // Render menu widgets
            renderScriptMenuWidget();
            renderObjectsMenuWidget();
        }

        virtual void activate() override {
            utilslib::Logger::getInfo() << "activate() ABOUT" << std::endl;

            //using onRenderCallGame_t =  void (__stdcall *)(Script* this, DWORD deltaTime);

            g_game = reinterpret_cast<CGame*>(this->m_gameController);
            g_originalDeleter = reinterpret_cast<originalDelete_t*>(Script::getVtable()->swapFunction(0,(DWORD*) scriptDelete));
            g_originalUpdater = reinterpret_cast<originalUpdate_t*>(Script::getVtable()->swapFunction(0x1C/4,(DWORD*) scriptUpdate));
            g_originalDeleter1 = reinterpret_cast<originalDelete_t*>(Script::getVtable1()->swapFunction(0,(DWORD*) scriptDelete1));
            g_originalUpdater1 = reinterpret_cast<originalUpdate_t*>(Script::getVtable1()->swapFunction(0x1C/4,(DWORD*) scriptUpdate1));
                
        }

        static void __thiscall scriptUpdate(Script* script, DWORD deltaTime)
        {
            auto obj = reinterpret_cast<OBJECT*>(reinterpret_cast<UCHAR*>(script)-0xA9C);
            script->m_name = obj->frame->frameName;

            g_perFrameScripts.insert(script);
            g_ScriptMap.update(script);
            if(g_ScriptMap.getMap()[script].requestUpdate())
                g_originalUpdater(script,0,deltaTime);
        }

        static void __thiscall scriptDelete(Script* script)
        {
            g_perFrameScripts.erase(script);
            g_originalDeleter(script);
        }

         static void __thiscall scriptUpdate1(Script* script, DWORD deltaTime)
        {
            // Hack: set script name to frame name
            auto obj = reinterpret_cast<OBJECT*>(reinterpret_cast<UCHAR*>(script)-0x70);
            script->m_name = obj->frame->frameName;

            g_perFrameScripts.insert(script);
            g_ScriptMap.update(script);
            if(g_ScriptMap.getMap()[script].requestUpdate())
                g_originalUpdater1(script,0,deltaTime);
        }

        static void __thiscall scriptDelete1(Script* script)
        {
            g_perFrameScripts.erase(script);
            g_originalDeleter1(script);
        }

        virtual void deactivate() override {
            reinterpret_cast<originalDelete_t*>(Script::getVtable()->swapFunction(0,(DWORD*) g_originalDeleter));
            reinterpret_cast<originalUpdate_t*>(Script::getVtable()->swapFunction(0x1C/4,(DWORD*) g_originalUpdater));

            reinterpret_cast<originalDelete_t*>(Script::getVtable1()->swapFunction(0,(DWORD*) g_originalDeleter1));
            reinterpret_cast<originalUpdate_t*>(Script::getVtable1()->swapFunction(0x1C/4,(DWORD*) g_originalUpdater1));
            
            g_perFrameScripts.clear();
        }
};
#endif
