#include "CCore.h"
#include "CCoreController.hpp"
#include <utilslib/logger.hpp>
#include <Windows.h>

bool CCore::Initialize()
{
    auto result = getWrapper()->load("cameramod-wrapper.dll");
    if(!result)
    {
        // TODO: report fail
        return false;
    }

    auto initFunction = getWrapper()->getExternFunction<std::function<bool(void)>>("cameramod_game_initialize");
    if(!initFunction)
    {
        // TODO: report fail
        return false;
    }

    (*initFunction)();

    auto gameCreateFunction = getWrapper()->getExternFunction<std::function<CGenericGame*(void)>>("cameramod_game_get_module");
    if(!gameCreateFunction)
    {
        // TODO: report fail
        return false;
    }

    auto game = (*gameCreateFunction)();

    //game->getInput()->registerOnKey();

    // Replace D3D8 driver with our proxy to hook EndScene()
    //m_originalD3DDriver = this->getHook()->getD3D8Driver();
    //utilslib::Logger::getInfo() << "[CCore] Original Driver: " << std::hex << m_originalD3DDriver << std::dec << std::endl;
    //this->getGraphics()->SetDevice(m_originalD3DDriver);

    // Replace game's driver with our proxy driver
    //this->getHook()->replaceDirectXDriver(this->getGraphics());

    // Initialize graphics 
    //this->getGraphics()->Init();

    // Create proxy object for manipulating with parts of mod
    CCoreController controller;
    controller.m_blockGameInput = [&] (bool shouldWe)->void {};
    controller.m_blockGUIInput = [&] (bool shouldWe)->void { this->m_isGUIacceptingInput = shouldWe; };
    controller.m_exitMod = [&] ()->void { this->ModDetach(); };
    // TODO
    //controller.m_hideGUI = NULL;
    // Set controller to mod control
    this->getModControl()->setCoreController(controller);


    this->getModControl()->InitializeModes(this->getGame());

    game->getInput()->registerOnKey(
         [&] (USHORT pressedKey, KeyStatus status)->bool {
                if(pressedKey == VK_F12 && status == KEYDOWN)
                {
                    if(game->getGraphics()->saveScreenshot("C:\\screenshot.bmp"))
                        this->getGame()->PrintDebugMessage("Screenshot taken");
                    else
                        this->getGame()->PrintDebugMessage("Screenshot.bmp failed");
                }
                //if(this->m_isGUIacceptingInput)
                //{
                    //this->getGraphics()->getImGUIAdaptor().updateKey(pressedKey, isKeyDown);
                //}
                if(status == KEYDOWN)
                    return this->getModControl()->OnVKKey(pressedKey); 
                return true;
            });
    // Register mouse move callback
    game->getInput()->registerOnMouseMoveDelta(
            [&] (LONG x, LONG y)->void {
                this->getModControl()->OnMouseMove(x, y);

                if(this->m_isGUIacceptingInput)
                {
                    //Point2D point = this->getGraphics()->GetMouseCoords();
                    //this->getGraphics()->getImGUIAdaptor().updateMousePosition(point);
                    // TODO: 
                }
            });
    // Register mouse button callback
    /*this->getRawInput()->m_onMouseButtonsUpdateHandlers.add(
            [&] (unsigned short state)->void {
                this->getModControl()->OnMouseButtons(state);
            });
    // Register mouse button callback for ImGUI
    this->getRawInput()->m_onMouseButtonsUpdateHandlers.add(
            [&] (unsigned short state)->void {
                if(this->m_isGUIacceptingInput)
                {
                    this->getGraphics()->getImGUIAdaptor().updateButton(state);
                }
            });
    */
    // Register GUI content generator callback
    game->getGraphics()->registerOnRender(
            [&] ()->void
            {
                this->getModControl()->Render();
            });

    // Register DearImGUI render
    /*this->getGraphics()->m_onRenderHandler.add(
            [&] (void)->void  {
	    	// Render in any case
                //if(this->m_isGUIacceptingInput)
		this->getGraphics()->getImGUIAdaptor().setMouseVisible(this->m_isGUIacceptingInput);
                this->getGraphics()->getImGUIAdaptor().Render();
            });
    */
    // Tick on game
    /*this->getGraphics()->m_onRenderHandler.add(
        [&] (void)->void {
            this->getGame()->onTick();
        }
    );
    */
    return true;
}
bool CCore::Unload()
{
    //this->getGraphics()->Unload();
    // Restore original D3D8 driver
    //this->getHook()->replaceDirectXDriver(m_originalD3DDriver);

    //this->getHook()->UnloadThem();
    this->getGame()->ToggleHUD(false);
    this->getGame()->UnlockCameraTransform();
    return true;
}
