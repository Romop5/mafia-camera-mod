#ifndef CSCENE_HPP
#define CSCENE_HPP
#include <glm/glm.hpp>
#include <vector>

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
        const std::vector<CCameraPoint*>& getPoints() const { return this->m_trackPoints; }
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
    const std::vector<CCameraPoint>& getCameraPoints() { return this->m_points; }
};
#endif
