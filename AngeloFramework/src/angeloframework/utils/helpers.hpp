#ifndef ANGELOFRAMEWORK_UTILS_HELPERS_HPP
#define ANGELOFRAMEWORK_UTILS_HELPERS_HPP

#define RADIAN (180.0f / 3.14159f)
#define TORADIAN (3.14159f / 180.0f)

namespace AngeloFramework
{
// using arc tangens for 2 variables
float RotationTo180(float x, float y);

float RotationTo360(float x, float y);
float ASinTo180(float x);

bool IsInRadius2D(int pointX, int pointY, int a, int b, int radius);

bool IsInRect(int pointX, int pointY, int Rx, int Ry, int Rx2, int Ry2);

}
#endif