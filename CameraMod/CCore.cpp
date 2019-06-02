#include "CCore.h"
#include <utilslib/logger.hpp>

bool CCore::Initialize()
{
    auto version = this->getGame()->GetGameVersion();
    if(version != 384)
    {
        utilslib::Logger::getError() << "Expecting game version 384 (1.0), found " << version << std::endl;
        return false;
    }
    this->getHook()->setInputMessageHandler(
            [&] (void* msg)->bool {
                // Process input by our handlers
                return this->getRawInput()->ProcessMessage(reinterpret_cast<LPMSG>(msg));   
            });
    // Apply keyboard hook etc.
    this->getHook()->ApplyThem();
    // Replace D3D8 driver with our proxy to hook EndScene()
    m_originalD3DDriver = this->getHook()->getD3D8Driver();

    this->getGraphics()->SetDevice(m_originalD3DDriver);

    // Replace game's driver with our proxy driver
    this->getHook()->replaceDirectXDriver(this->getGraphics());

    // Initialize graphics 
    this->getGraphics()->Init();

    this->getModControl()->InitializeModes(this->getGame());

    // Register onPressKey callback
    this->getRawInput()->m_onKeyPressedHandlers.add(
            [&] (USHORT pressedKey)->bool {
                return this->getModControl()->OnVKKey(pressedKey); 
            });
    // Register mouse move callback
    this->getRawInput()->m_onMouseMoveHandlers.add(
            [&] (LONG x, LONG y)->void {
                this->getModControl()->OnMouseMove(x, y);
            });
    // Register mouse button callback
    this->getRawInput()->m_onMouseButtonsUpdateHandlers.add(
            [&] (unsigned short state)->void {
                this->getModControl()->OnMouseButtons(state);
            });
    // Register mouse button callback for ImGUI
    this->getRawInput()->m_onMouseButtonsUpdateHandlers.add(
            [&] (unsigned short state)->void {
                this->getGraphics()->getImGUIAdaptor().updateButton(state);
            });

    // Register GUI content generator callback
    this->getGraphics()->getImGUIAdaptor().m_contentRenderers.add(
            [&] ()->void
            {
                this->getModControl()->Render();
            });
    return true;
}
bool CCore::Unload()
{
    // Restore original D3D8 driver
    this->getHook()->replaceDirectXDriver(m_originalD3DDriver);

    this->getHook()->UnloadThem();

    this->getGraphics()->Unload();
    this->getGame()->CameraUnlock();
    return true;
}
