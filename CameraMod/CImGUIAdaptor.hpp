#ifndef C_IMGUIADAPTOR
#define C_IMGUIADAPTOR
#include <utilslib/callbacks.hpp>

#include "structures.h"
struct IDirect3DDevice9;


class CImGUIAdaptor {
public:
    void Initialize(IDirect3DDevice9* device, Point2D size);
    void Render();
    bool SetUpIO();
    void Win32NewFrame();
    void CleanUP();
    void Invalidate();
    void updateMousePosition(Point2D position);
    void updateButton(unsigned short state);
    
    // Register for content generators
    utilslib::Callbacks<std::function<void()>> m_contentRenderers;
private:
    Point2D screenSize;
};
#endif
