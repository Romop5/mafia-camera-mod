#ifndef CSCENE_HPP
#define CSCENE_HPP
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <sstream>
#include <algorithm>
#include <array>
#include "game/CGame.h"
#include <glm/gtx/compatibility.hpp>
#include <glm/gtx/spline.hpp>

///////////////////////////////////////////////////////////////////////////////
// Camera points + camera paths
///////////////////////////////////////////////////////////////////////////////
class CCameraPoint
{
    public:
        CCameraPoint(glm::vec3 p, glm::vec3 r): m_point(p), m_rotation(r), m_name("CamPoint"), duration(1000.0) {}
        explicit CCameraPoint(const nlohmann::json obj) { deserialize(obj); }
        glm::vec3 m_point;
        glm::vec3 m_rotation;
        std::string m_name;
        float duration;
        const nlohmann::json serialize() const 
        {
            nlohmann::json jsonDump;
            jsonDump["position"] = Vec3ToJSON(m_point);
            jsonDump["rotation"] = Vec3ToJSON(m_rotation);
            jsonDump["name"] = m_name;
            return jsonDump;
        }

        bool deserialize(const nlohmann::json obj) 
        {
            m_point = JSONToVec3(obj["position"]);
            m_rotation = JSONToVec3(obj["rotation"]);
            m_name = obj["name"].get<std::string>();
            return true;
        }
};

using CameraPointVector_t = std::vector<CCameraPoint>;

class CCameraTrack
{
    public:
        CCameraTrack(): m_name("CamTrack") {}
        std::string m_name;
        std::vector<CCameraPoint> m_trackPoints; 
        void addPoint(CCameraPoint* m_point) { this->m_trackPoints.push_back(*m_point); }
        std::vector<CCameraPoint>& getPoints() { return this->m_trackPoints; }
        const nlohmann::json serialize() const 
        {
            nlohmann::json jsonDump;
            nlohmann::json jsonPoints;
            for(auto& point: m_trackPoints)
            {
                jsonPoints.push_back(point.serialize());
            }
            jsonDump["name"] = m_name;
            jsonDump["points"] = jsonPoints;
            return jsonDump;
        }
        bool deserialize(const nlohmann::json obj)
        {
            bool resultStatus = true;
            m_trackPoints.clear();
            for(auto& point: obj["points"])
            {
                auto newPoint = CCameraPoint(glm::vec3(),glm::vec3(0));
                resultStatus &= newPoint.deserialize(point);
                m_trackPoints.push_back(newPoint);
            }
            m_name = obj["name"].get<std::string>();
            return resultStatus;
        }
};
class CTrackPlayerController
{
    public:
    /// Returns point for given ID
    std::function<CCameraPoint&(size_t)> m_getPoint;
    ///Returns the next point ID for given ID
    std::function<size_t(size_t)> m_getNextID;
    ///Move track's head by N points
    std::function<void(size_t)> m_moveHead;
    ///Get current point
    std::function<size_t()> m_getCurrentPoint;
};
/**
 * @brief Component which moves camera along the path
 * 
 * The component is reponsible for calculating camera's properties for each frame.
 * For example, CPointTrackInterpolator moves camera in descrete steps whereas
 * CLinearTrackInterpolator moves camera in lines.
 * 
 * When updateCamera is called, the component can use passed controller to query
 * current track's head position and underlying camera points, and 
 * also triggers moving of the head.
 */
class CGenericTrackInterpolator
{
    protected:
    CTrackPlayerController* m_controller;
    float m_elapsedTime;
    public:
    CGenericTrackInterpolator(): m_elapsedTime(0) {}
    virtual ~CGenericTrackInterpolator() = default;
    void setController(CTrackPlayerController* controller) { m_controller = controller; }
    virtual void updateCamera(glm::vec3& cameraPosition, glm::vec3& cameraRotation, float deltams) = 0;

};

class CPointTrackInterpolator: public CGenericTrackInterpolator
{
    public:
    virtual void updateCamera(glm::vec3& cameraPosition, glm::vec3& cameraRotation, float deltams) override
    {
        auto tmpCurrentID = m_controller->m_getCurrentPoint();
        auto tmpCurrentPoint = m_controller->m_getPoint(tmpCurrentID);
        m_elapsedTime += deltams;
        if(tmpCurrentPoint.duration < m_elapsedTime)
        {
            m_elapsedTime = 0;
            // Update camera head position
            m_controller->m_moveHead(1);
        }
        auto currentID = m_controller->m_getCurrentPoint();
        auto currentPoint = m_controller->m_getPoint(currentID);

        auto nextID = m_controller->m_getNextID(currentID);
        auto nextPoint = m_controller->m_getPoint(nextID);

        // Turns off interpolation => moves in discrete steps
        float t = 0;

        cameraPosition = glm::lerp(currentPoint.m_point,nextPoint.m_point, t);
        auto newQuat = glm::slerp(glm::quat(currentPoint.m_rotation),glm::quat(nextPoint.m_rotation), t);
        cameraRotation = glm::vec3(newQuat.x, newQuat.y, newQuat.z);
    }
};

class CLinearTrackInterpolator: public CGenericTrackInterpolator
{
    public:
    virtual void updateCamera(glm::vec3& cameraPosition, glm::vec3& cameraRotation, float deltams) override
    {
        auto tmpCurrentID = m_controller->m_getCurrentPoint();
        auto tmpCurrentPoint = m_controller->m_getPoint(tmpCurrentID);
        m_elapsedTime += deltams;
        if(tmpCurrentPoint.duration < m_elapsedTime)
        {
            m_elapsedTime = 0;
            m_controller->m_moveHead(1);
        }
        auto currentID = m_controller->m_getCurrentPoint();
        auto currentPoint = m_controller->m_getPoint(currentID);

        auto nextID = m_controller->m_getNextID(currentID);
        auto nextPoint = m_controller->m_getPoint(nextID);

        float t = m_elapsedTime / currentPoint.duration;

        cameraPosition = glm::lerp(currentPoint.m_point,nextPoint.m_point, t);
        auto newQuat = glm::slerp(glm::quat(currentPoint.m_rotation),glm::quat(nextPoint.m_rotation), t);
        cameraRotation = glm::vec3(newQuat.x, newQuat.y, newQuat.z);
    }
};

class CCatmulRomTrackInterpolator: public CGenericTrackInterpolator
{
    public:
    virtual void updateCamera(glm::vec3& cameraPosition, glm::vec3& cameraRotation, float deltams) override
    {
        auto id = m_controller->m_getCurrentPoint();
        std::array<CCameraPoint*,4> points;
        // Get points
        for(auto& ptr: points)
        {
            ptr = &m_controller->m_getPoint(id);
            id = m_controller->m_getNextID(id);
        }

        m_elapsedTime += deltams;
        float t = m_elapsedTime / points[0]->duration;
        if(t > 1.0f)
        {
            m_controller->m_moveHead(1);
            t = 1.0;
            m_elapsedTime = 0;
        }

        cameraPosition = glm::catmullRom(points[0]->m_point,points[1]->m_point,points[2]->m_point,points[3]->m_point,t);
        cameraRotation = glm::catmullRom(points[0]->m_rotation,points[1]->m_rotation,points[2]->m_rotation,points[3]->m_rotation,t);
    }
};

class CCubicTrackInterpolator: public CGenericTrackInterpolator
{
    public:
    virtual void updateCamera(glm::vec3& cameraPosition, glm::vec3& cameraRotation, float deltams) override
    {
        auto id = m_controller->m_getCurrentPoint();
        std::array<CCameraPoint*,2> points;
        // Get points
        for(auto& ptr: points)
        {
            ptr = &m_controller->m_getPoint(id);
            id = m_controller->m_getNextID(id);
        }

        m_elapsedTime += deltams;
        float t = m_elapsedTime / points[0]->duration;
        if(t > 1.0f)
        {
            m_controller->m_moveHead(1);
            t = 1.0;
            m_elapsedTime = 0;
        }

        static glm::vec3 coef = glm::vec3(0.0,0.1,0.0);
        cameraPosition = glm::hermite(points[0]->m_point,coef,points[1]->m_point,coef,t);
        cameraRotation = glm::hermite(points[0]->m_rotation,coef,points[1]->m_rotation,coef,t);
    }
};

class CGenericInterpolatorFactory
{
    using return_type = std::unique_ptr<CGenericTrackInterpolator>;
    public:
    inline static std::unique_ptr<CGenericTrackInterpolator> createInterpolator(const std::string name)
    {
        for(const auto& pair: getNames())
        {
            if(pair.first == name)
            return pair.second();
        }
        return static_cast<return_type>(std::make_unique<CLinearTrackInterpolator>());
    }

    inline static const std::vector<std::pair<std::string,std::function<return_type()>>> getNames() 
    {
        static const std::vector<std::pair<std::string,std::function<return_type()>>> interpolatorNames = {
            std::make_pair("CatmulRom", [] { return static_cast<return_type>(std::make_unique<CCatmulRomTrackInterpolator>());}),
            std::make_pair("Point", [] { return static_cast<return_type>(std::make_unique<CPointTrackInterpolator>());}),
            std::make_pair("Cubic", [] { return static_cast<return_type>(std::make_unique<CCubicTrackInterpolator>());}),
            std::make_pair("Linear", [] { return static_cast<return_type>(std::make_unique<CLinearTrackInterpolator>());})
        }; 
        return interpolatorNames;
    }
};

class CTrackPlayer
{
    public:
    CCameraTrack* m_currentTrack;
    std::unique_ptr<CGenericTrackInterpolator> m_interpolator;
    CTrackPlayerController m_ourController;
    size_t m_replayingHead;
    bool m_isRewinding;
    float m_speed;
    float m_elapsedTime;
    bool m_isStoped;
    bool m_isCircular;

    public:
    CTrackPlayer(): m_isRewinding(false), m_speed(1.0), m_isStoped(true), m_currentTrack(nullptr),m_isCircular(true) {

        m_ourController.m_getPoint = [&] (size_t id)->CCameraPoint& 
        {
            return m_currentTrack->getPoints()[id];
        };
        m_ourController.m_getNextID = [&] (size_t id)->size_t 
        {
            return getNextHeadPosition(id);
        }; 
        m_ourController.m_moveHead = [&] (size_t positions)->void 
        {
            for(size_t i = 0; i < positions; i++)
            {
                m_replayingHead = getNextHeadPosition();
            }
        };
        m_ourController.m_getCurrentPoint = [&] (void)->size_t
        {
            return m_replayingHead;
        };

        this->setInterpolator(static_cast<std::unique_ptr<CGenericTrackInterpolator>>(std::make_unique<CCatmulRomTrackInterpolator>()));
    }

    void moveForward() {
        m_replayingHead = getNextHeadPosition();
    }
    void toggleStop(bool shouldBeStoped) { m_isStoped = shouldBeStoped; }
    bool isStoped() const { return m_isStoped; } 

    void setSpeed(float speed) { m_speed = speed; }
    float getSpeed() const { return m_speed; }

    void toggleCircular(bool shouldRewind) { m_isCircular = shouldRewind; }
    bool isCircular() const { return m_isCircular; }

    void toggleRewinding(bool shouldRewind) { m_isRewinding = shouldRewind; }
    bool isRewinding() const { return m_isRewinding; }
    void reset() 
    {
        m_elapsedTime = 0;
        m_replayingHead = 0;
    }
    void setTrack(CCameraTrack* track) { m_currentTrack = track; }

    void setInterpolator(std::unique_ptr<CGenericTrackInterpolator>& interpolator)
    {
        m_interpolator = std::move(interpolator);
        m_interpolator->setController(&m_ourController);
    }

    void updateCamera(glm::vec3& cameraPosition, glm::vec3& cameraRotation)
    {
        if(!m_currentTrack)
            return;
        if(m_currentTrack->getPoints().size() == 0)
            return;
        if(this->isStoped())
            return;
        if(!m_interpolator)
            return;
        m_interpolator->updateCamera(cameraPosition, cameraRotation, 30.0f*m_speed);

        /*auto& currentFrame = m_currentTrack->getPoints()[m_replayingHead];
        
        // interpolate between successive frames
        float t = m_elapsedTime/currentFrame.duration;
        auto& nextFrame = m_currentTrack->getPoints()[getNextHeadPosition()];

        cameraPosition = glm::lerp(currentFrame.m_point,nextFrame.m_point, t);
        auto newQuat = glm::slerp(glm::quat(currentFrame.m_rotation),glm::quat(nextFrame.m_rotation), t);
        cameraRotation = glm::vec3(newQuat.x, newQuat.y, newQuat.z);
        // fixed-time loop update
        m_elapsedTime += 30*m_speed;
        if(m_elapsedTime > currentFrame.duration)
        {
            m_elapsedTime = 0;
            m_replayingHead = getNextHeadPosition();
            
            if(!m_isCircular && (m_replayingHead == m_currentTrack->getPoints().size()-1))
            {   // if it's not circular, then skip the last point
                m_replayingHead = getNextHeadPosition();
            }
        }*/
    }
    private:
    size_t getNextHeadPosition(size_t position = -1) const { 
        if(position == -1)
            position = m_replayingHead;
        auto length = m_currentTrack->getPoints().size();
        auto offset = (m_isRewinding ? -1 : 1);
        if(length == 0)
            return 0;
        if(m_isRewinding && position == 0)
        {
            if(m_isCircular)
                return length-1;
            return 0;
        }
        auto newPosition = (position+offset) % length;
        if(!m_isCircular)
        {
            if(newPosition < position && m_isRewinding)
                return position;
        }
        return newPosition;
    }
};

class CCameraManager
{
    private:
        std::vector<CCameraPoint> m_points;
        std::vector<CCameraTrack> m_tracks;
    public:

    void addCameraPoint(glm::vec3 position, glm::vec3 m_rotation);
    void addNewTrack();
    std::vector<CCameraPoint>& getCameraPoints() { return this->m_points; }
    std::vector<CCameraTrack>& getCameraTracks() { return this->m_tracks; }
    const nlohmann::json serialize() const 
    {
        nlohmann::json jsonDump;
        nlohmann::json points; 
        nlohmann::json tracks; 

        // create array of points
        for(auto& point: m_points)
        {
            points.push_back(point.serialize());
        }  
        // create array of tracks
        for(auto& track: m_tracks)
        {
            tracks.push_back(track.serialize());
        }
        jsonDump["points"] = points;
        jsonDump["tracks"] = tracks;
        return jsonDump;
    }
    bool deserialize(const nlohmann::json obj)
    {
        bool status = true;
        m_tracks.clear();
        for(auto& track: obj["tracks"])
        {
            auto newTrack = CCameraTrack();
            status &= newTrack.deserialize(track);
            m_tracks.push_back(newTrack);
        }
        m_points.clear();
        for(auto& point: obj["points"])
        {
            auto newPoint = CCameraPoint(glm::vec3(0),glm::vec3(0));
            status &= newPoint.deserialize(point);
            m_points.push_back(newPoint);
        }
        return status;
    }
};

///////////////////////////////////////////////////////////////////////////////
// Recording player movement
///////////////////////////////////////////////////////////////////////////////

class CPlayerMovementFrame
{
    public:
    glm::vec3 position;
    glm::vec3 rotation;
    PED_State objectState;
    CPlayerMovementFrame(const glm::vec3 pos,const glm::vec3 rot, PED_State state): position(pos), rotation(rot), objectState(state) {}
    inline std::string serialize() const 
    {
        std::stringstream ss;
        ss << position.x << "," << position.y << "," << position.z;
        return ss.str();
    }
};

enum class CRecordingStateEnum
{
    IDLE,
    RECORDING,
    PAUSED,
    PLAYING,
};

class CRecordingState
{
    private:
        size_t m_currentReplayPosition;
    public:
        CRecordingState(): m_state(CRecordingStateEnum::IDLE), m_currentReplayPosition(0) {}
        CRecordingStateEnum m_state;    
        std::vector<CPlayerMovementFrame> m_recordedPath;

        inline std::string serialize() const
        {
            std::stringstream ss;
            for(auto& frame: m_recordedPath)
            {
                ss << "{ " << frame.serialize() << " } ,";
            }
            return ss.str();
        }

        inline void setRecordingState(CRecordingStateEnum newState)
        {
            if(newState == CRecordingStateEnum::RECORDING && m_state != CRecordingStateEnum::PAUSED)
            {
                m_recordedPath.clear();
            }
            m_state = newState;
        }
        inline bool isRunning() const
        {
            return m_state == CRecordingStateEnum::RECORDING;
        }
        
        inline CRecordingStateEnum getState() const
        {
            return m_state;
        }
        
        inline void startReplaying()
        {
            m_currentReplayPosition = 0;
        }

        inline CPlayerMovementFrame& getCurrentReplayFrame()
        {
            // Wrap over
            if(++m_currentReplayPosition >= m_recordedPath.size())
                m_currentReplayPosition = 0;
            return m_recordedPath[m_currentReplayPosition];
        }
};


///////////////////////////////////////////////////////////////////////////////
// CScene interface
///////////////////////////////////////////////////////////////////////////////

class CScene
{
    public:
    bool save(const std::string path);
    bool load(const std::string path);

    CRecordingState m_recordingState;
    CCameraManager m_cameraManager;
    CTrackPlayer m_player;
};
#endif
