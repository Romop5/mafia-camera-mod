#include "CGUI.h"
#ifndef C_GUIPOOL
#define C_GUIPOOL
//#include "CGUIElement.h"

struct CGUIElementsPoolStruct {
    CGUIElement* element;
    CGUIElementsPoolStruct* next;
};
class CGUIElementsPool {
private:
    CGUIElementsPoolStruct* pool;
    unsigned int size;
    CGUIElementsPoolStruct* pointer;

public:
    CGUIElementsPool();

    __declspec(noinline) void AddElement(CGUIElement* elem);
    void DeleteElement(CGUIElement* elem);
    CGUIElement* GetNext();
    void Reset();
    void ToTheTop(CGUIElement* elem);

    CGUIElementsPoolStruct* GetFirst();
};

#endif