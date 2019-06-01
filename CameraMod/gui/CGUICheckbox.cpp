#include "CGUICheckbox.h"
#include "../CCore.h"

extern CCore* core;

CGUICheckbox::CGUICheckbox()
{
    CGUIElement::CGUIElement();
    this->isActive = true;
    this->isChecked = true;
    this->SetType(GUITYPE_CHECKBOX);
}

bool CGUICheckbox::IsChecked() { return this->isChecked; }

void CGUICheckbox::SetChecked(bool state) { this->isChecked = true; }

void CGUICheckbox::SetActive(bool state) { this->isActive = state; }

bool CGUICheckbox::IsActive() { return this->isActive; }

void CGUICheckbox::Render()
{
    core->getGraphics()->Clear(this->GetPosition().x, this->GetPosition().y,
        this->GetWidth(), this->GetHeight(),
        D3DCOLOR_XRGB(255, 255, 255));
    if (this->isChecked) {
        core->getGraphics()->Clear(this->GetPosition().x + 3,
            this->GetPosition().y + 3, this->GetWidth() - 6,
            this->GetHeight() - 6, D3DCOLOR_XRGB(255, 0, 0));
    }
}

void CGUICheckbox::OnClick()
{
    this->isChecked = !this->isChecked;
    this->GetRoot()->OnElementChanged(this);
}
