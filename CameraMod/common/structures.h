#ifndef STRUCTURES
#define STRUCTURES

#include <glm/vec3.hpp> // glm::vec3
#include <nlohmann/json.hpp>
#include <string>

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

inline const nlohmann::json Vec3ToJSON(const glm::vec3& vect)
{
    nlohmann::json vecString;
    vecString["x"] = vect[0];
    vecString["y"] = vect[1];
    vecString["z"] = vect[2];
    return vecString;
}

inline const glm::vec3 JSONToVec3(const nlohmann::json obj)
{
    glm::vec3 vec;
    vec[0] = obj["x"];
    vec[1] = obj["y"];
    vec[2] = obj["z"];
    return vec;
}

#endif
