#ifndef CGENERIC_GAME_HPP
#define CGENERIC_GAME_HPP
#include <string>
#include <vector>
#include <glm/glm.hpp>

class CGenericObject
{
    public:
    virtual const std::string& getName() const = 0;
    virtual const std::string& getType() const = 0;

    virtual glm::mat4 getTransform() const = 0; 
    virtual void setTransform(const glm::mat4&) = 0;
    virtual const std::string dumpJSON() const = 0;
};

class CGenericObjectRecording
{
    public:
    virtual ~CGenericObjectRecording() = default;
    virtual const std::string dumpJSON() const = 0;
    virtual void loadFromJSON(const std::string& json) = 0;
};

class CGenericGameSettingBase
{
    protected:
    std::string m_name;
    public:
    const std::string getName() const { return this->m_name; }
    virtual std::string getType() const = 0;
};

#define CAMERAMOD_NUMERIC_GAME_SETTING(param)\
class CGenericGameSetting_##param: public CGenericGameSettingBase\
{\
    protected:\
    param m_minimal;\
    param m_maximal;\
    param m_value;\
    public:\
    param getValue() const { return m_value; }\
    bool setValue(const param val) {\
        if(val < m_minimal || val > m_maximal)\
            return false;\
        m_value = val;\
        return true;\
    }\
    virtual std::string getType() const override {\
        return "" #param;\
    }\
};

CAMERAMOD_NUMERIC_GAME_SETTING(int)
CAMERAMOD_NUMERIC_GAME_SETTING(float)
#undef CAMERAMOD_NUMERIC_GAME_SETTING

class CGenericGameSetting_String: public CGenericGameSettingBase
{
    protected:
    std::string m_value;
    public:
    const std::string getValue() const { return m_value; }
    bool setValue(const std::string val) {
        m_value = val;
        return true;
    }
    virtual std::string getType() const override  {
        return "string";
    }
};
/**
 * @brief Describes object's record
 * 
 * Allows to query recording status.
 */
class CGenericRecordingInfo
{
    public:
    virtual bool IsRecording() const = 0;
    virtual bool IsReplaying() const = 0;
    virtual size_t getFramesCount() const = 0;
    virtual size_t getEventsCount() const = 0;
    virtual size_t getCurrentFrameIndex() const = 0;
};

/**
 * @brief Describes a generic API that a game must implement
 * 
 * There are compulsory methods (defined as pure virtual) and 
 * optional methods, which may not be provided by game.
 */
class CGenericGame
{
    public:
    virtual ~CGenericGame() = default;
    virtual CGenericObject* getLocalPlayer() = 0;

    virtual void SetCameraTransform(const glm::mat4& transform) = 0;
    virtual void UnlockCameraTransform() = 0;
    virtual const glm::mat4 GetCameraTransform() const = 0;

    virtual void createObjectFromJSON(const std::string) = 0;
    virtual void LockControls(bool shouldBeLocked) = 0;

    virtual std::vector<std::unique_ptr<CGenericGameSettingBase>>& getSettings() = 0;

    ///////////////////////////////////////////////////////////////////////////
    // EVENTS
    ///////////////////////////////////////////////////////////////////////////
    virtual void onTick() = 0;

    ///////////////////////////////////////////////////////////////////////////
    // OPTIONAL METHODS (may be implemented by game)
    ///////////////////////////////////////////////////////////////////////////
    virtual void ToggleHUD(bool shouldBeVisible) {}
    virtual const std::string& GetVersionString() const {
        static const std::string versionString = "Not implemented";
        return versionString;
    }
    virtual void PrintDebugMessage(const std::string& message) {}

    /// Record object's state starting this moment
    virtual void startRecording(CGenericObject* object) = 0;
    virtual void clearRecording(CGenericObject* object) = 0;
    virtual CGenericObjectRecording* saveRecording(CGenericObject* object) = 0;
    virtual  void playRecording(CGenericObject* object, CGenericObjectRecording* record) = 0;
    virtual CGenericRecordingInfo* getRecordingInfo(CGenericObject* object) = 0;
};
#endif