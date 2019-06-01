#include "CGUI.h"
#ifndef C_GUICHECKBOX
#define C_GUICHECKBOX

class CGUICheckbox : public CGUIElement {
private:
    bool isChecked;
    bool isActive;

public:
    CGUICheckbox();
    void SetChecked(bool state);
    bool IsChecked();

    void SetActive(bool);
    bool IsActive();

    void Render();

    void OnClick();
};

#endif