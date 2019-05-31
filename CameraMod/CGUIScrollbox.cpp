#include "CGUIScrollbox.h"
#include "CCore.h"
#include "CGUI.h"

extern CCore* core;

CGUIScrollbox::CGUIScrollbox()
{
    CGUIElement::CGUIElement();
    this->x = 0;
    this->y = 0;
    this->width = 0;
    this->height = 0;

    this->shift = 0;
    this->size = 0;
    this->sizeOverall = 0;
}

void CGUIScrollbox::SetUp(int x, int y, int width, int height)
{
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
}

void CGUIScrollbox::Update(int overallSize)
{
    this->sizeOverall = overallSize;
    this->size = (float)this->height / (float)overallSize;
}

void CGUIScrollbox::Render()
{
    if (this->size < 1) {
        int pice = (int)(this->size * this->height);
        int scrollPos = ((float)this->shift / this->sizeOverall) * this->height;
        core->getGraphics()->Clear(this->x + this->width - 10, this->y + scrollPos,
            10, pice, 0xff00adef);

        // sprintf(test, "%d %f %d", pice, this->size, this->height);
        // core->getGraphics()->TextDraw(70, 20, test, 0xffff0000);
    }
}

int CGUIScrollbox::GetShift() { return this->shift; }

void CGUIScrollbox::OnKey(int VK)
{
    if (this->size < 1) {
        switch (VK) {
        case VK_UP:
            shift -= 5;
            break;
        case VK_DOWN:
            shift += 5;
        default:
            break;
        }

        if (shift < 0)
            shift = 0;

        if (shift > (this->sizeOverall - this->height))
            shift = (this->sizeOverall - this->height);
    }
}

void CGUIScrollbox::OnMouseMove(int x, int y)
{
    if (this->size < 1) {
        shift += y;

        if (shift < 0)
            shift = 0;

        if (shift > (this->sizeOverall - this->height))
            shift = (this->sizeOverall - this->height);
    }
}