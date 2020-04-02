#ifndef ANGELOFRAMEWORK_UTILS_GAME_HOOKS_HPP
#define ANGELOFRAMEWORK_UTILS_GAME_HOOKS_HPP

#include <vector>
#include <Windows.h>
#include "angeloframework/utils/memory.hpp"

namespace AngeloFramework
{

struct HookPatch
{
    DWORD m_address;
    size_t m_sizeOfPatch;
    char m_originalData[5];
};

struct ObjectReplacement
{
    DWORD m_address;
    DWORD m_originalValue;
};

/**
 * @brief Manage installed function hooks
 * 
 * Allows to add hooks to functions, or patch pieces of memory, and automatically revert
 * changes when required.
 */
class GameHooks
{
private:
    std::vector<HookPatch> m_installedPatch;
    std::vector<ObjectReplacement> m_replacedObjects;
    void UnloadAll()
    {
        // Unload hooks
        for(auto& patch: m_installedPatch)
        {
            UninstallHook(patch.m_address,patch.m_originalData);
        }
        m_installedPatch.clear();

        // Replace back
        for(auto& object: m_replacedObjects)
        {
            SetValueAt(object.m_address, object.m_originalValue);
        }
        m_replacedObjects.clear();
    }
public:
    GameHooks() {}
    ~GameHooks()
    {
        UnloadAll();
    }
    bool AddPatch(DWORD address, DWORD functionAddress)
    {
        HookPatch newPatch;
        newPatch.m_address = address;
        InstallHook(address,functionAddress, newPatch.m_originalData);
    }

    bool AddObjectReplacement(DWORD address, DWORD newObject)
    {
        auto oldObject = GetValueAt(address);
        SetValueAt(address,newObject);
        m_replacedObjects.push_back(ObjectReplacement{address,newObject});
        return true;
    }
};

}

#endif