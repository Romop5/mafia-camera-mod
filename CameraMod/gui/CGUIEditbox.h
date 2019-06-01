#include "CGUI.h"
#ifndef C_GUIEDITBOX
#define C_GUIEDITBOX

class CGUIEditbox : public CGUIElement {
private:
    char input[255];
    unsigned char status;

public:
    CGUIEditbox();
    void SetInput(char* text);
    char* GetInput();

    void Render();

    void OnInput(unsigned short VK);
};

#endif