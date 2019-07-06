#ifndef CSCENE_HPP
#define CSCENE_HPP
#include <glm/glm.hpp>
#include <vector>
#include <sstream>
#include "game/CGame.h"

///////////////////////////////////////////////////////////////////////////////
// Camera points + camera paths
///////////////////////////////////////////////////////////////////////////////
class CCameraPoint
{
    public:
        CCameraPoint(glm::vec3 p, glm::vec3 r): m_point(p), m_rotation(r), m_name("CamPoint") {}
        glm::vec3 m_point;
        glm::vec3 m_rotation;
        std::string m_name;

};

class CCameraTrack
{
    public:
        CCameraTrack(): m_name("CamTrack") {}
        std::string m_name;
        std::vector<CCameraPoint*> m_trackPoints; 
        void addPoint(CCameraPoint* m_point) { this->m_trackPoints.push_back(m_point); }
        std::vector<CCameraPoint*>& getPoints() { return this->m_trackPoints; }
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

class CScene
{
    private:
        std::vector<CCameraPoint> m_points;
        std::vector<CCameraTrack> m_tracks;
    public:
    void save(const std::string path) {}
    void load(const std::string path) {}

    void addCameraPoint(glm::vec3 position, glm::vec3 m_rotation);
    std::vector<CCameraPoint>& getCameraPoints() { return this->m_points; }
    std::vector<CCameraTrack>& getCameraTracks() { return this->m_tracks; }

    CRecordingState m_recordingState;
};
#endif
