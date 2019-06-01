//#pragma once
// core of GUI subsystem
#ifndef C_GUI
#define C_GUI

class CGUI;
#include <stdio.h>

#include "CGUIElement.h"
#include "CGUIElementsPool.h"
#include "CGUIScrollbox.h"
// objects

#include "CGUIButton.h"
#include "CGUICheckbox.h"
#include "CGUIEditbox.h"
#include "CGUIGroup.h"
#include "CGUILabel.h"
#include "CGUITableList.h"
#include "CGUIWindow.h"

class CGUI {
private:
    CGUIElementsPool* elementsPool;
    CGUIElementsPool* windowsPool;

    CGUIElement* elementHover;
    CGUIElement* elementInput;

    bool isLeftClick;

    bool isInputActive;

public:
    CGUI();

    CGUIButton* CreateButton(int x, int y, unsigned int width,
        unsigned int height, char* title);
    CGUICheckbox* CreateCheckbox(int x, int y, unsigned int width,
        unsigned int height, bool state);
    CGUITableList* CreateTableList(int x, int y, unsigned int width,
        unsigned int height);
    CGUIEditbox* CreateEditbox(int x, int y, unsigned int width,
        unsigned int height, char* text);
    CGUIGroup* CreateGroup(bool visible);
    CGUILabel* CreateLabel(int x, int y, char* text);

    CGUIWindow* GUICreateWindow(int x, int y, unsigned int width,
        unsigned int height, char* text);

    void DeleteElement(CGUIElement* elem);

    CGUIElement* GetInputElement();
    CGUIElement* GetHoverElement();

    bool IsInputActive();
    bool SetInputActive(bool state);
    // Windows mng
    void RenderWindows(CGUIElementsPoolStruct* win);
    void ToTheTop(CGUIElement* element);
    // callbacks

    void OnClick(CGUIElement* element);
    void OnElementSelect(CGUIElement* element, int ID);
    void OnElementChanged(CGUIElement* element);

    // extern callbacks
    void OnMouseMove(int x, int y);
    void OnMouseButtons(unsigned short buttons);
    void OnInput(unsigned short VK);
    void OnRender();

    char debug[500];
};

#endif