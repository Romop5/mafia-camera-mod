#include "CScene.hpp"

void CCameraManager::addCameraPoint(glm::vec3 position, glm::vec3 m_rotation)
{
    auto point = CCameraPoint(position, m_rotation);
    this->m_points.push_back(point);
}
