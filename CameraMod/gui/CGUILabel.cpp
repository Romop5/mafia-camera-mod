#include "CCore.h"
//#include "CGUI.h"

extern CCore* core;

CGUILabel::CGUILabel()
{
    color = 0xFFffFFff;
    this->SetType(GUITYPE_LABEL);
}
char* CGUILabel::GetText() { return this->text; }
void CGUILabel::SetText(char* text) { sprintf(this->text, "%s", text); }

void CGUILabel::SetColor(unsigned int color) { this->color = color; }
unsigned int CGUILabel::GetColor() { return this->color; }

void CGUILabel::Render()
{
    core->getGraphics()->TextDraw(this->GetX(), this->GetY(), this->text, color);
}