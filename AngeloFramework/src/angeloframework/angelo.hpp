/**
 * @file angelo.hpp
 * @author Roman Dobias (rom.dobias@gmail.com)
 * @brief Definition of Framework's API
 * @version 1.0
 * @date 2020-03-22
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#include "angeloframework/game/offsets.hpp"
#include <functional>

struct IDirect3DDevice9;
namespace AngeloFramework
{
    class Manager;
}

namespace AngeloFramework
{
    /**
     * @brief Control set up of framework routines
     * 
     */
    class Framework
    {
    public:
        Framework();
        ~Framework();
        /*
            Controlling the framework/game
        */
        /// Inject game and set up routines
        bool Load();
        /// Detach game
        bool Unload();

        using EngineVersion = GameOffsets::EngineVersion;
        DWORD GetEngineVersion() const;
        bool IsInMafiaProcess() const;

        void AddRenderCallback(std::function<void(void)> callback);
        /*
            Framework features
        */
        bool IsImGuiAvailable() const;

        IDirect3DDevice9* GetDirect3D9Device() const;

    private:
        AngeloFramework::Manager* m_core;
    };
}