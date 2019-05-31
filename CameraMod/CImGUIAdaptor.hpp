#ifndef C_IMGUIADAPTOR
#define C_IMGUIADAPTOR
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

private:
    Point2D screenSize;
};
#endif
