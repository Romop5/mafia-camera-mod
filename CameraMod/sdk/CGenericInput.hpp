#ifndef CAMERAMOD_SDK_INPUT_HPP
#define CAMERAMOD_SDK_INPUT_HPP

#include <functional>

enum KeyStatus
{
    KEYDOWN,
    KEYUP
};

using onkey_callback_t = std::function<void(size_t keycode, KeyStatus state)>; 
using mousemove_callback_t = std::function<void(float x, float y)>; 
using mousemove_delta_callback_t = std::function<void(float x, float y)>; 

class CGenericInput
{
    public:
    virtual bool registerOnKey(onkey_callback_t& callback);
    virtual bool registerOnMouseMove(mousemove_callback_t& callback);
    virtual bool registerOnMouseMoveDelta(mousemove_delta_callback_t& callback);
};

#endif