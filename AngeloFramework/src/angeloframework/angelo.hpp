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

#include <functional>

namespace AngeloFramework
{
    class FrameworkCallbacks
    {
        std::function<void(void)> m_onRender;
        std::function<void(void)> m_onKeyInput;
    };
    class Framework
    {
        public:
        /*
            Controlling the framework/game
        */
        bool Load();
        bool Unload();
        void SetCallbacks(FrameworkCallbacks callbacks);
        const FrameworkCallbacks& GetCallbacks(FrameworkCallbacks callbacks); 
    };
}