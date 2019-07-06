#include <string>
#include <glm.hpp>

class CGenericObject
{
    public:
    virtual const std::string& getName() const = 0;
    virtual const std::string& getType() const = 0;

    virtual glm::vec3 getPosition() const = 0; 
    virtual void setPosition(const glm::vec3&) = 0;
    virtual const std::string dumpJSON() const = 0;
};

class CGenericObjectRecording
{
    public:
    virtual const std::string dumpJSON() const = 0;
    virtual void loadFromJSON(const std::string& json) = 0;
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
    virtual CGenericObject* getLocalPlayer() = 0;

    virtual void SetCameraPosition(const glm::vec3& position) = 0;
    virtual void UnlockCameraPosition() = 0;
    virtual const glm::vec3 GetCameraPosition() const = 0;

    virtual void createObjectFromJSON(const std::string) = 0;

    ///////////////////////////////////////////////////////////////////////////
    // OPTIONAL METHODS (may be implemented by game)
    ///////////////////////////////////////////////////////////////////////////
    virtual void ToggleHUD(bool shouldBeVisible) {}
    virtual const std::string& GetVersionString() const {return "Not implemented";}
    virtual void PrintDebugMessage(std::string& message) {}

    /// Record object's state starting this moment
    virtual void startRecording(CGenericObject& object);
    virtual void clearRecording(CGenericObject& object);
    virtual CGenericObjectRecording& saveRecording(CGenericObject& object);
};