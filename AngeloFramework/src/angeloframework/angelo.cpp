
#include "angelo.hpp"

#include "angeloframework/game/offsets.hpp"
#include "angeloframework/utils/game_hooks.hpp"
#include "angeloframework/core/manager.hpp"

#include <functional>


class AngeloFramework::Framework::Framework_impl
{
public:
    void Initialize()
    {
        m_manager.Initialize();
    }
    void Uninitialize()
    {
        m_manager.Initialize();
    }
    Manager m_manager;
};

namespace AngeloFramework
{
    
    Framework::Framework()
    {
        m_core = new Framework_impl();
    }
    Framework::~Framework()
    {
        delete m_core;
    }
    bool Framework::Load()
    {
        m_core->Initialize();
        return true;
    }
    bool Framework::Unload()
    {
        m_core->Uninitialize();
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
}