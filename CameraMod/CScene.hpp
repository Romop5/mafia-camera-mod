#ifndef CSCENE_HPP
#define CSCENE_HPP
#include <glm/glm.hpp>
#include <nlohmann/json.hpp>
#include <vector>
#include <sstream>
#include <algorithm>
#include "game/CGame.h"
#include <glm/gtx/compatibility.hpp>

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


class CTrackPlayer
{
    public:
    CCameraTrack* m_currentTrack;
    size_t m_replayingHead;
    bool m_isRewinding;
    float m_speed;
    float m_elapsedTime;
    bool m_isStoped;
    bool m_isCircular;

    public:
    CTrackPlayer(): m_isRewinding(false), m_speed(1.0), m_isStoped(true), m_currentTrack(nullptr),m_isCircular(true) {}

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

    void updateCamera(glm::vec3& cameraPosition, glm::vec3& cameraRotation)
    {
        if(!m_currentTrack)
            return;
        if(m_currentTrack->getPoints().size() == 0)
            return;
        if(this->isStoped())
            return;
        auto& currentFrame = m_currentTrack->getPoints()[m_replayingHead];
        
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
        }
    }
    private:
    size_t getNextHeadPosition() const { 
        auto length = m_currentTrack->getPoints().size();
        auto offset = (m_isRewinding ? -1 : 1);
        if(length == 0)
            return 0;
        if(m_isRewinding && m_replayingHead == 0)
            return length-1;
        return (m_replayingHead+offset) % length;
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
    void save(const std::string path);
    void load(const std::string path);

    CRecordingState m_recordingState;
    CCameraManager m_cameraManager;
    CTrackPlayer m_player;
};
#endif
