#include "CCore.h"

bool CCore::Initialize()
{
    this->getHook()->setInputMessageHandler(
            [&] (void* msg)->bool {
                // block whole game input when mod is active
                if(this->getModControl()->IsActive())		
                    return false;
                // Otherwise, delegate input to RawInput class
                this->getRawInput()->ProcessMessage(reinterpret_cast<LPMSG>(msg));   
                return true;
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
    
    //this->SetModule(hModule);
    this->getModControl()->Init();

    // Register onPressKey callback
    this->getRawInput()->m_onKeyPressedHandlers.add(
            [&] (USHORT pressedKey)->void {
                this->getModControl()->OnVKKey(pressedKey); 
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
