#include "CCore.h"

bool CCore::Initialize()
{
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
