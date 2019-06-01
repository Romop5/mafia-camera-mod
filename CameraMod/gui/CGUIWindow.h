#include "CGUI.h"
#ifndef C_GUIWINDOW
#define C_GUIWINDOW

enum clickState { CLICK_NONE,
    CLICK_UP,
    CLICK_DOWN,
    CLICK_LEFT,
    CLICK_RIGHT };
class CGUIWindow : public CGUIElement {
private:
    char title[200];
    CGUIElementsPool elements;
    Point2D lastMousePoint;
    int clickState;

    int minWidth;
    int minHeight;

public:
    CGUIWindow();

    void SetTitle(char*);
    char* GetTitle();

    bool AddElement(CGUIElement*);

    void Render();

    void OnClick();

    void GetAbsolutePos(Point2D& position);

    void OnMouseMove(int x, int y);

    __declspec(noinline) CGUIElement* OnHoverCheck(Point2D& point);
};

#endif