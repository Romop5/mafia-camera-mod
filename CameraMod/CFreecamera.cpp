#include "CFreecamera.hpp"
#include <glm/glm.hpp> 
#include <glm/gtx/transform.hpp>

bool CFreecamera::onVKKey(USHORT key) {
    float frontVectorMagnitude = 0.0;
    float leftVectorMagnitude = 0.0;
    glm::vec3 upVector = glm::vec3(0.0,1.0,0.0);

    enum 
    {
        VK_A = 0x41,
        VK_S = 0x53,
        VK_W = 0x57,
        VK_D = 0x44,
    };
    switch(key)
    {
        case VK_W:
        case VK_UP: frontVectorMagnitude += m_cameraFlyingSpeed; break;
        case VK_S:
        case VK_DOWN: frontVectorMagnitude -= m_cameraFlyingSpeed; break;
        case VK_A:
        case VK_LEFT: leftVectorMagnitude += m_cameraFlyingSpeed; break;
        case VK_D:
        case VK_RIGHT: leftVectorMagnitude -= m_cameraFlyingSpeed; break;
    }

    glm::vec3 leftVector = glm::cross(rotation, upVector);

    this->position += leftVector*leftVectorMagnitude + rotation*frontVectorMagnitude;
    this->updateCamera();
    return true;
}


void CFreecamera::onMouseMove(int x, int y)
{
    this->angleX += x*0.01;
    this->angleY += y*0.01;

    //auto verticalMatrix = glm::rotate(this->angleY, glm::vec3(-1.0,0.0,0.0)); 
    //auto horizontalMatrix = glm::rotate(this->angleX, glm::vec3(0.0,1.0,0.0)); 
    //this->rotation = glm::vec3(horizontalMatrix*verticalMatrix*glm::vec4(1.0,0.0,0.0,0.0));
    this->rotation = glm::vec3(cos(this->angleY)*sin(this->angleX), sin(this->angleY), cos(this->angleX)*cos(this->angleY));
    updateCamera();
}
