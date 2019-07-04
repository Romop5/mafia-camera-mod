#ifndef STRUCTURES
#define STRUCTURES

#include <glm/vec3.hpp> // glm::vec3

struct Vector3D {
    float x, y, z;
};

struct Vector2D {
    float x, y;
};

struct Point2D {
    int x, y;
};

static inline glm::vec3 toGlm(Vector3D& vec)
{
    return glm::vec3(vec.x,vec.y,vec.z);
}

static inline Vector3D toVec3D(glm::vec3& vec)
{
    Vector3D vector;
    vector.x = vec[0];
    vector.y = vec[1];
    vector.z = vec[2];
    return vector;
}

#endif
