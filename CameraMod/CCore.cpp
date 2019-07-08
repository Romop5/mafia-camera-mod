#include "CCore.h"
#include "CCoreController.hpp"
#include <utilslib/logger.hpp>

bool CCore::Initialize()
{
    /*auto version = this->getGame()->GetGameVersion();
    if(version != 384)
    {
        utilslib::Logger::getError() << "Expecting game version 384 (1.0), found " << version << std::endl;
        return false;
    }*/
    this->getHook()->setInputMessageHandler(
            [&] (void* msg)->bool {
                // Process input by our handlers
                return this->getRawInput()->ProcessMessage(reinterpret_cast<LPMSG>(msg));   
            });
    // Apply keyboard hook etc.
    this->getHook()->ApplyThem();
    // Replace D3D8 driver with our proxy to hook EndScene()
    m_originalD3DDriver = this->getHook()->getD3D8Driver();
    utilslib::Logger::getInfo() << "[CCore] Original Driver: " << std::hex << m_originalD3DDriver << std::dec << std::endl;

    this->getGraphics()->SetDevice(m_originalD3DDriver);

    // Replace game's driver with our proxy driver
    this->getHook()->replaceDirectXDriver(this->getGraphics());

    // Initialize graphics 
    this->getGraphics()->Init();

    // Create proxy object for manipulating with parts of mod
    CCoreController controller;
    controller.m_blockGameInput = [&] (bool shouldWe)->void { this->getRawInput()->shouldBlockInput(shouldWe); };
    controller.m_blockGUIInput = [&] (bool shouldWe)->void { this->m_isGUIacceptingInput = shouldWe; };
    // TODO
    //controller.m_hideGUI = NULL;
    // Set controller to mod control
    this->getModControl()->setCoreController(controller);


    this->getModControl()->InitializeModes(this->getGame());

    // Register onPressKey callback
    this->getRawInput()->m_onKeyPressedHandlers.add(
            [&] (USHORT pressedKey)->bool {
                // Shutdown on ESC
                if(pressedKey == VK_ESCAPE)
                {
                    this->getGame()->PrintDebugMessage("Shutting down LOL");
                    this->ModDetach();
                    }
                return this->getModControl()->OnVKKey(pressedKey); 
            });
    // Register mouse move callback
    this->getRawInput()->m_onMouseMoveHandlers.add(
            [&] (LONG x, LONG y)->void {
                this->getModControl()->OnMouseMove(x, y);

                if(this->m_isGUIacceptingInput)
                {
                    Point2D point = this->getGraphics()->GetMouseCoords();
                    this->getGraphics()->getImGUIAdaptor().updateMousePosition(point);
                }
            });
    // Register mouse button callback
    this->getRawInput()->m_onMouseButtonsUpdateHandlers.add(
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

    // Register GUI content generator callback
    this->getGraphics()->getImGUIAdaptor().m_contentRenderers.add(
            [&] ()->void
            {
                this->getModControl()->Render();
            });

    // Register DearImGUI render
    this->getGraphics()->m_onRenderHandler.add(
            [&] (void)->void  {
	    	// Render in any case
                //if(this->m_isGUIacceptingInput)
		this->getGraphics()->getImGUIAdaptor().setMouseVisible(this->m_isGUIacceptingInput);
                this->getGraphics()->getImGUIAdaptor().Render();
            });
    // Tick on game
    this->getGraphics()->m_onRenderHandler.add(
        [&] (void)->void {
            this->getGame()->onTick();
        }
    );
    return true;
}
bool CCore::Unload()
{
    this->getGraphics()->Unload();
    // Restore original D3D8 driver
    this->getHook()->replaceDirectXDriver(m_originalD3DDriver);

    this->getHook()->UnloadThem();
    this->getGame()->ToggleHUD(false);
    this->getGame()->UnlockCameraTransform();
    return true;
}
