#include "CCore.h"
#include "CGUI.h"
//#include "tools.h"

extern CCore* core;

CGUIEditbox::CGUIEditbox()
{
    CGUIElement::CGUIElement();
    sprintf(this->input, "%s", "");
}

void CGUIEditbox::SetInput(char* text) { sprintf(this->input, "%s", text); }
char* CGUIEditbox::GetInput() { return this->input; }

void CGUIEditbox::OnInput(unsigned short VK)
{
    switch (VK) {
    case VK_BACK: {
        int len = strlen(input);
        if (len > 0) {
            input[len - 1] = 0x0;
        }
    } break;
    case VK_SPACE: {
        int len = strlen(input);
        if (len < 250) {
            input[len] = ' ';
            input[len + 1] = 0x0;
        }
    } break;
    default: {
        char inputChar = ConvertToASCII(VK);
        if (inputChar) {
            int len = strlen(input);
            if (len < 250) {
                input[len] = inputChar;
                input[len + 1] = 0x0;
            }
        }
    }
    }
    this->GetRoot()->OnElementChanged(this);
}

void CGUIEditbox::Render()
{
    Point2D position = this->GetPosition();

    core->getGraphics()->Clear(this->GetX(), this->GetY(), this->GetWidth(),
        this->GetHeight(), 0xffffFFFF);
    RenderClip clip;
    core->getGraphics()->SetViewport(&clip, position.x + 5, position.y,
        this->GetWidth() - 10, this->GetHeight());

    int newY = (this->GetHeight() / 2) - 8;
    int len = core->getGraphics()->TextDrawLength(this->GetInput());
    core->getGraphics()->TextDraw(this->GetX() + this->GetWidth() - len - 5,
        this->GetY() + newY, this->input, 0xff000000);

    // int len = strlen(this->input);

    // core->getGraphics()->TextMonoDraw((-len*9)+this->GetX()+this->GetWidth()-5,
    // this->GetY()+newY, this->input, 0xff000000);
    core->getGraphics()->RestoreRenderBackup(&clip);

    // if (status < 125 && this->HasInput())
    if (status < 125 && this->GetRoot()->GetInputElement() == this) {
        core->getGraphics()->Clear(this->GetX() + this->GetWidth() - 3,
            this->GetY() + 5, 1, this->GetHeight() - 10,
            0xff000000);
    }
    status += 5;
}