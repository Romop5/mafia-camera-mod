//#include "CGUIElement.h"
#include "../CCore.h"
//#include "CGUI.h"

CGUIElement::CGUIElement()
{
    this->hasInput = false;
    this->isVisible = true;
    this->owner = 0;
    this->root = 0;
}

void CGUIElement::SetRoot(CGUI* parent) { this->root = parent; }
CGUI* CGUIElement::GetRoot() { return this->root; }

void CGUIElement::SetOwner(CGUIElement* parent) { this->owner = parent; }

CGUIElement* CGUIElement::GetOwner() { return this->owner; }

void CGUIElement::SetPosition(Point2D pos) { this->position = pos; }

Point2D CGUIElement::GetPosition()
{
    Point2D absolute;
    GetAbsolutePos(absolute);

    absolute.x += position.x;
    absolute.y += position.y;

    return absolute;
}

void CGUIElement::SetX(int value) { this->position.x = value; }

void CGUIElement::SetY(int value) { this->position.y = value; }

int CGUIElement::GetX()
{
    Point2D absolute;
    GetAbsolutePos(absolute);

    absolute.x += position.x;

    return absolute.x;
}

int CGUIElement::GetY()
{
    Point2D absolute;
    GetAbsolutePos(absolute);

    absolute.y += position.y;

    return absolute.y;
}

int CGUIElement::GetRelativeX() { return position.x; }

int CGUIElement::GetRelativeY() { return position.y; }

void CGUIElement::SetWidth(unsigned int width) { this->width = width; }

unsigned int CGUIElement::GetWidth() { return this->width; }

void CGUIElement::SetHeight(unsigned int height) { this->height = height; }

unsigned int CGUIElement::GetHeight() { return this->height; }

void CGUIElement::SetVisible(bool state) { this->isVisible = state; }

bool CGUIElement::IsVisible()
{
    bool value = true;
    if (this->owner != 0 && this->owner != this) {
        value = this->GetOwner()->IsVisible();
    }
    return (this->isVisible && value);
}

void CGUIElement::SetType(unsigned char type) { this->type = type; }

unsigned char CGUIElement::GetType() { return this->type; }

void CGUIElement::SetHasInput(bool state) { this->hasInput = state; }

bool CGUIElement::HasInput() { return this->hasInput; }

void CGUIElement::GetAbsolutePos(Point2D& position)
{
    if (this->GetOwner() != 0)
        this->GetOwner()->GetAbsolutePos(position);
    else {
        position.x = 0;
        position.y = 0;
    }
}

CGUIElement* CGUIElement::OnHoverCheck(Point2D& point)
{
    if (IsInRect(point.x, point.y, this->GetX(), this->GetY(),
            this->GetX() + this->GetWidth(),
            this->GetY() + this->GetHeight())) {
        return this;
    }
    return NULL;
}