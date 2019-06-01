#include "CGUI.h"

#ifndef C_GUIGROUP
#define C_GUIGROUP

class CGUIGroup : public CGUIElement {
private:
    bool bIsActive;
    CGUIElementsPool elements;

public:
    CGUIGroup();

    bool AddElement(CGUIElement*);
    void Render();
    // void		OnHover();
    CGUIElementsPool* GetElements();
    __declspec(noinline) CGUIElement* OnHoverCheck(Point2D& point);
};

#endif