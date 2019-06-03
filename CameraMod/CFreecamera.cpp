#include "CFreecamera.hpp"
#include <glm/glm.hpp> 
#include <glm/gtx/transform.hpp>

bool CFreecamera::onVKKey(USHORT key) {
    float frontVectorMagnitude = 0.0;
    float leftVectorMagnitude = 0.0;
    glm::vec3 upVector = glm::vec3(0.0,1.0,0.0);
    switch(key)
    {
        case VK_UP: frontVectorMagnitude += 1.0; break;
        case VK_DOWN: frontVectorMagnitude -= 1.0; break;
        case VK_LEFT: leftVectorMagnitude += 1.0; break;
        case VK_RIGHT: leftVectorMagnitude -= 1.0; break;
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

    auto verticalMatrix = glm::rotate(this->angleY, glm::vec3(-1.0,0.0,0.0)); 
    auto horizontalMatrix = glm::rotate(this->angleX, glm::vec3(0.0,1.0,0.0)); 
    this->rotation = glm::vec3(verticalMatrix*horizontalMatrix*glm::vec4(1.0,0.0,0.0,0.0));
    updateCamera();
}
