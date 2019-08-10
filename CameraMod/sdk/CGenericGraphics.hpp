#ifndef CAMERAMOD_SDK_GRAPHICS_HPP
#define CAMERAMOD_SDK_GRAPHICS_HPP

#include <functional>

class CGenericGraphics
{
    public:
    virtual bool registerOnRender(std::function<void()>& func);

    virtual bool saveScreenshot(const std::string path);
};

#endif