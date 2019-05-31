#include "CGUI.h"
#include "CCore.h"

extern CCore* core;

CGUI::CGUI()
{
    this->elementsPool = new CGUIElementsPool();
    this->windowsPool = new CGUIElementsPool();

    this->elementHover = 0;
    this->elementInput = 0;

    this->isLeftClick = false;
}

CGUIButton* CGUI::CreateButton(int x, int y, unsigned int width,
    unsigned int height, char* title)
{
    CGUIButton* button = new CGUIButton();
    button->SetRoot(this);
    //	button->SetActive(true);
    button->SetX(x);
    button->SetY(y);
    button->SetWidth(width);
    button->SetHeight(height);
    button->SetButtonText(title);

    if (this->elementsPool == 0)
        throw 20;
    this->elementsPool->AddElement(button);

    return button;
}

CGUICheckbox* CGUI::CreateCheckbox(int x, int y, unsigned int width,
    unsigned int height, bool state)
{
    CGUICheckbox* element = new CGUICheckbox();
    element->SetRoot(this);
    element->SetActive(true);
    element->SetX(x);
    element->SetY(y);
    element->SetWidth(width);
    element->SetHeight(height);
    element->SetChecked(state);

    if (this->elementsPool == 0)
        throw 20;
    this->elementsPool->AddElement(element);

    return element;
}

CGUITableList* CGUI::CreateTableList(int x, int y, unsigned int width,
    unsigned int height)
{
    CGUITableList* element = new CGUITableList();
    element->SetRoot(this);
    element->SetX(x);
    element->SetY(y);
    element->SetWidth(width);
    element->SetHeight(height);

    if (this->elementsPool == 0)
        throw 20;
    this->elementsPool->AddElement(element);

    return element;
}

CGUIEditbox* CGUI::CreateEditbox(int x, int y, unsigned int width,
    unsigned int height, char* text)
{
    CGUIEditbox* element = new CGUIEditbox();
    element->SetRoot(this);
    element->SetX(x);
    element->SetY(y);
    element->SetWidth(width);
    element->SetHeight(height);
    element->SetInput(text);

    if (this->elementsPool == 0)
        throw 20;
    this->elementsPool->AddElement(element);

    return element;
}

CGUIGroup* CGUI::CreateGroup(bool visible)
{
    CGUIGroup* element = new CGUIGroup();
    element->SetRoot(this);
    element->SetVisible(visible);

    if (this->elementsPool == 0)
        throw 20;
    this->elementsPool->AddElement(element);

    return element;
}

CGUILabel* CGUI::CreateLabel(int x, int y, char* text)
{
    CGUILabel* element = new CGUILabel();
    element->SetRoot(this);
    element->SetX(x);
    element->SetY(y);
    element->SetText(text);

    if (this->elementsPool == 0)
        throw 20;
    this->elementsPool->AddElement(element);

    return element;
}

CGUIWindow* CGUI::GUICreateWindow(int x, int y, unsigned int width,
    unsigned int height, char* text)
{
    CGUIWindow* element = new CGUIWindow();
    element->SetRoot(this);
    element->SetX(x);
    element->SetY(y);
    element->SetWidth(width);
    element->SetHeight(height);
    element->SetTitle(text);

    if (this->windowsPool == 0)
        throw 20;
    this->windowsPool->AddElement(element);

    return element;
}

void CGUI::DeleteElement(CGUIElement* elem) {}

CGUIElement* CGUI::GetInputElement() { return this->elementInput; }

CGUIElement* CGUI::GetHoverElement() { return this->elementHover; }

bool CGUI::IsInputActive() { return this->isInputActive; }
bool CGUI::SetInputActive(bool state)
{
    this->isInputActive = state;
    return true;
}

// extern callbacks

void CGUI::OnMouseMove(int x, int y)
{
    if (this->IsInputActive() == false)
        return;
    if (this->isLeftClick && this->elementInput)
        this->elementInput->OnMouseMove(x, y);

    Point2D point = core->getGraphics()->GetMouseCoords(), position;

    CGUIElementsPoolStruct* pointer = this->windowsPool->GetFirst();
    while (pointer != 0) {
        if (pointer->element->IsVisible() && pointer->element->GetOwner() == false) {
            CGUIElement* result = pointer->element->OnHoverCheck(point);
            if (result) {
                this->elementHover = result;
                return;
            }
        }
        pointer = pointer->next;
    }

    pointer = this->elementsPool->GetFirst();
    while (pointer != 0) {
        if (pointer->element->IsVisible() && pointer->element->GetOwner() == false) {
            CGUIElement* result = pointer->element->OnHoverCheck(point);
            if (result) {
                this->elementHover = result;
                return;
            }
        }
        pointer = pointer->next;
    }
    this->elementHover = 0;
}

void CGUI::OnMouseButtons(unsigned short buttons)
{
    if (buttons & RI_MOUSE_LEFT_BUTTON_DOWN) {
        if (this->GetHoverElement() != NULL) {
            this->isLeftClick = true;
            this->OnClick(this->GetHoverElement());
            this->elementInput = this->GetHoverElement();
            this->elementInput->SetHasInput(true);
        } else {
            if (this->elementInput)
                this->elementInput->SetHasInput(false);
            this->elementInput = NULL;
        }
    } else if (buttons & RI_MOUSE_LEFT_BUTTON_UP) {
        if (this->isLeftClick) {
            this->isLeftClick = false;
            if (this->elementInput)
                this->elementInput->OnClickEnd();
        }
    }
}

void CGUI::OnInput(unsigned short VK)
{
    if (elementInput != NULL) {
        this->elementInput->OnInput(VK);
    }
}

void CGUI::OnRender()
{
    // this->elementsPool->Reset();
    CGUIElementsPoolStruct* pointer = this->elementsPool->GetFirst();
    while (pointer != 0) {
        if (pointer->element->IsVisible() && pointer->element->GetOwner() == false)
            pointer->element->Render();
        pointer = pointer->next;
    }

    pointer = this->windowsPool->GetFirst();
    this->RenderWindows(pointer);
    /*while (pointer != 0)
  {
          //pointer->element->IsVisible()
          if (pointer->element->IsVisible() && pointer->element->GetOwner() ==
  false) pointer->element->Render(); pointer = pointer->next;
  }*/

    /*char buff[500];
  sprintf(buff, "Elem %p", this->GetHover());
  core->getGraphics()->TextDraw(500,10,buff,D3DCOLOR_XRGB(255,255,255));
  */
    // core->getGraphics()->TextDraw(500, 300, debug, D3DCOLOR_XRGB(255, 255,
    // 255));
}

void CGUI::OnClick(CGUIElement* elem)
{
    elem->OnClick();
    core->getModControl()->onGUIClick(elem);
}

void CGUI::OnElementSelect(CGUIElement* element, int ID)
{
    core->getModControl()->onGUISelectElement(element, ID);
}

void CGUI::OnElementChanged(CGUIElement* element)
{
    core->getModControl()->onGUIElementChanged(element);
}

void CGUI::ToTheTop(CGUIElement* element)
{
    this->windowsPool->ToTheTop(element);
}

void CGUI::RenderWindows(CGUIElementsPoolStruct* win)
{
    if (win != NULL) {
        if (win->next != NULL) {
            this->RenderWindows(win->next);
        }
        if (win->element->IsVisible() && win->element->GetOwner() == false)
            win->element->Render();
    }
}
