
#include "angelo.hpp"

#include "angeloframework/game/offsets.hpp"
#include "angeloframework/utils/helpers.hpp"
#include "angeloframework/utils/game_hooks.hpp"
#include "angeloframework/core/manager.hpp"

#include <functional>

namespace AngeloFramework
{
    
    Framework::Framework()
    {
        m_core = new Manager();
    }
    Framework::~Framework()
    {
        if(m_core)
            delete m_core;
    }
    bool Framework::Load()
    {
        m_core->Initialize();
        return true;
    }
    bool Framework::Unload()
    {
        if (m_core)
            delete m_core;
        return true;
    }

    DWORD Framework::GetEngineVersion() const
    {
        return GameOffsets::GetEngineVersion();
    }

    bool Framework::IsInMafiaProcess() const 
    {
        return GameOffsets::IsProcessMafia();
    }
	void Framework::AddRenderCallback(std::function<void(void)> callback)
	{
        m_core->m_imGuiAdaptor.m_contentRenderers.add(callback);
	}
    bool Framework::IsImGuiAvailable() const
    {
        // In the future, ImGUI can be compile-defined and implemented as a submodule to decrease the size of repo and remove Dx8.1/Dx9 depdencies 
        return true;
    }
    IDirect3DDevice9* Framework::GetDirect3D9Device() const
    {
        auto d3d8wrapper = m_core->m_d3d8Hook.get();
        auto d3d8 = d3d8wrapper->getProxy();

        return Core::ConvertD3D8ToD3D9(d3d8);
    }
}