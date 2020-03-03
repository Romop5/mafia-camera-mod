#ifndef CSCRIPTINSPECTOR_HPP
#define CSCRIPTINSPECTOR_HPP

#include <set>
#include <string>
#include "game/CGame.h"
#include "common/imgui_utils.hpp"

class ScriptInspector
{
    enum ScriptState
    {
        RUNNING,
        PAUSED,
        SINGLESTEP
    };
    
    /// Is breakpoint set for given address
    std::set<size_t> m_breakPoints;
    /// Currently edited text
    std::string m_editBox;
    /// Parent (script reference)
    Script* m_script;
    /// Should display source code or allow editing
    bool m_toggleSourceEditing = true;
    /// Is script's window visible for inspection
    bool m_isVisible = false;
    /// Control state for debugging, running, and pausing
    ScriptState m_state = RUNNING;
    public:
    ScriptInspector(): m_script(nullptr) {}
    ScriptInspector(Script* owner): m_script(owner) {
        m_editBox.reserve(1000);
        m_editBox = m_script->m_sourceCode;
    }
    Script* getScript() { return m_script; }
    bool shouldBePaused() const { return m_state == PAUSED; }
    void forceState(ScriptState s) { m_state = s; } 

    /**
     * @brief Return true when game can continue
     */
    bool requestUpdate() {
        if(m_state == SINGLESTEP)
        {
            forceState(PAUSED);
            return true;
        }
        if(m_breakPoints.count(m_script->m_currentOpcodeID) > 0)
        {
            // TODO: notify break point
            setVisible(true);
            forceState(PAUSED);
            return false;
        }
        if(m_state == PAUSED)
            return false;
        return true;
    }
    bool isVisible() const { return m_isVisible; }
    void setVisible(bool b) { m_isVisible = b; }
    void render();
};
#endif