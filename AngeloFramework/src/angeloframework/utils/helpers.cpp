#ifndef ANGELOFRAMEWORK_UTILS_HELPERS
#define ANGELOFRAMEWORK_UTILS_HELPERS
#include <cmath>
#include "helpers.hpp"

// using arc tangens for 2 variables
float RotationTo180(float x, float y) { return atan2(x, y); }

float RotationTo360(float x, float y)
{
    float result = atan2(x, y) * RADIAN; // actually, this converts to degrees
    if (result < 0) {
        result = 360.0f - result * (-1);
    }
    return result;
}

float ASinTo180(float x) { return asin(x) * RADIAN; }

bool IsInRadius2D(int pointX, int pointY, int a, int b, int radius)
{
    if ((pointX >= a - radius && pointX <= a + radius) && (pointY >= b - radius && pointY <= b + radius))
        return true;
    return false;
}

bool IsInRect(int pointX, int pointY, int Rx, int Ry, int Rx2, int Ry2)
{
    if ((pointX >= Rx && pointX <= Rx2) && (pointY >= Ry && pointY <= Ry2))
        return true;
    return false;
}

#endif