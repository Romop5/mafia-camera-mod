#ifndef CTIMELINEMANAGER_MODE_HPP
#define CTIMELINEMANAGER_MODE_HPP
#include "modes/CGenericMode.hpp"

class CTimelineManager: public CGenericMode
{
    public:
        CTimelineManager() {}
       
        virtual void onRender() {
            // render timeline
        }

        virtual void activate() {

        }
        virtual void deactivate() {}
};
#endif
