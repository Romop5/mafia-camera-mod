#ifndef ANGELOFRAMEWORK_GAME_OFFSETS_HPP
#define ANGELOFRAMEWORK_GAME_OFFSETS_HPP

#include <Windows.h>
#include <unordered_map>

namespace AngeloFramework
{
    struct Address
    {
        /// 1.0 game
        DWORD address10;
        /// 1.2 game
        DWORD address12;
        
        Address() = default;
        Address(DWORD a, DWORD b): address10(a),address12(b) {}
    };

    /*
        Addresses are relative to LSD3D's baseaddress
    */
    static std::unordered_map<std::string, Address> m_engineAddresses = 
    {
        /// ptr to IDirect8Device
        {std::string("D3D8"),Address(0x1C167C,0x0)},
        {std::string("MAINWINDOW"),Address(0x1C13F4,0)},
    };
    class GameOffsets
    {
        public:
        enum EngineVersion: DWORD
        {
            VERSION_10 = 0x180,
            VERSION_12 = 0x18B,
        };
        
        static HMODULE GetEngineModule()
        {
            return GetModuleHandleA("ls3df.dll");
        }
        /// Returns start of the module in game's addressspace
        static DWORD GetEngineModuleStartAddress()
        {
            auto module = GetEngineModule();
            // According to https://docs.microsoft.com/en-us/windows/win32/api/psapi/ns-psapi-moduleinfo,
            // module's HWND == address 
            return reinterpret_cast<DWORD>(module);
        }
        /**
         * @brief Get engine version (LS3D)
         * 
         * Returns 0x180 when engine is 1.0. 
         * Returns 0x18B when engine is 1.2
         * Returns else when unknown.
         * @return DWORD 
         */
        static DWORD GetEngineVersion()
        {
            auto engineDll = GetEngineModule();
            if(engineDll == NULL)
                return 0;
            using engineVersionFunc = DWORD(void);
            auto engineFunc = reinterpret_cast<engineVersionFunc*>(GetProcAddress(engineDll,"GetEngineVersion"));
            if(engineFunc)
            {
                return engineFunc();
            }
            return 0;
        }
        /**
         * @brief Get absolute address of structure/variable in engine
         * @returns NULL when not found, or when not defined for this version
         * 
         * @see 'm_engineAddresses' for defined symbols 
         */
        static DWORD GetEngineAddress(const std::string type)
        {
            DWORD result = GetEngineModuleStartAddress();
            auto it = m_engineAddresses.find(type);
            if(it != m_engineAddresses.end())
            {
                if(GetEngineVersion() == VERSION_10)
                    result+=(*it).second.address10;
                if(GetEngineVersion() == VERSION_12)
                    result+=(*it).second.address12;
                return result;
            }
            return 0x0;
        }

        /**
         * @brief Detects if current process in which the methos is called, is Mafia
         * @return true when it is
         */
        static bool IsProcessMafia()
        {
            // Set up score and decide if score is bigger than half of max score
            size_t maxScore = 0;
            size_t score = 0;
            // I. detect if process is called 'Game.exe'
            maxScore++;
            static char moduleName[256];
            if (GetModuleFileNameA(NULL, moduleName, 255))
            {
                std::string exeName = moduleName;
                if (exeName.find("Game.exe") != std::string::npos)
                    score++;
            }

            // II. detect if process contains 'ls3df.dll'
            maxScore++;
            //static char moduleName[256];
            if (GetEngineModule() != NULL)
            {
                score++;
            }
            return (score > (maxScore/2));
        }
    };
}
#endif