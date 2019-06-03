#include "CCAMERA_PLAYER_HPP"
#define "CCAMERA_PLAYER_HPP"

#include "CScene.hpp"
#include "CGenericMode.hpp"

class CCameraPlayer: public CGenericMode
{
    private:
        CCameraTrack* m_currentTrack;
        
    public:
        virtual bool onVKKey(USHORT key) {return false;}
        virtual void onMouseMove(int x, int y) {}
        virtual void onMouseButtons(unsigned short buttons) {}
        virtual void onTick() {
            //TODO: update camera according to position at track 
        }
        virtual void onRender() {}

        virtual void activate() {
            utilslib::Logger::getInfo() << "activate() generic" << std::endl;
        }
        virtual void deactivate() {}
 
};

