#ifndef CFREECAMERA_HPP
#define CFREECAMERA_HPP
#include "structures.h"
#include "CGenericMode.hpp"
class CFreecamera: public CGenericMode
{
    public:
        virtual void onTick() {}
        virtual bool onVKKey(USHORT key) {
            return false;
        }
        virtual void onMouseMove(int x, int y) {
        }
    private:
        Vector3D position;
        Vector3D rotation;
};
#endif
