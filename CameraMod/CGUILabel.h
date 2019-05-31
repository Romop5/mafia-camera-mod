#include "CGUI.h"
#ifndef C_GUILABEL
#define C_GUILABEL

class CGUILabel : public CGUIElement {
private:
    char text[200];
    unsigned int color;

public:
    CGUILabel();
    char* GetText();
    void SetText(char*);

    void SetColor(unsigned int);
    unsigned int GetColor();

    void Render();
};

#endif