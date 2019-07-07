#include <string>
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

    virtual void SetCameraTransform(const glm::mat4& transform) = 0;
    virtual void UnlockCameraTransform() = 0;
    virtual const glm::mat4 GetCameraTransform() const = 0;

    virtual void createObjectFromJSON(const std::string) = 0;

    virtual void LockControls(bool shouldBeLocked) = 0;

    ///////////////////////////////////////////////////////////////////////////
    // OPTIONAL METHODS (may be implemented by game)
    ///////////////////////////////////////////////////////////////////////////
    virtual void ToggleHUD(bool shouldBeVisible) {}
    virtual const std::string& GetVersionString() const {return "Not implemented";}
    virtual void PrintDebugMessage(const std::string& message) {}

    /// Record object's state starting this moment
    virtual void startRecording(CGenericObject& object) = 0;
    virtual void clearRecording(CGenericObject& object) = 0;
    virtual CGenericObjectRecording& saveRecording(CGenericObject& object) = 0;
};