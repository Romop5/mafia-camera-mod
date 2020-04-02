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

namespace AngeloFramework
{
    class FrameworkCallbacks
    {
        std::function<void(void)> m_onRender;
        std::function<void(void)> m_onKeyInput;
    };

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

    private:
        class Framework_impl;
        Framework_impl* m_core;
    };
}