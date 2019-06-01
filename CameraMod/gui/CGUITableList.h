#include "CGUI.h"
#ifndef C_GUITABLELIST
#define C_GUITABLELIST

struct TableListElement {
    int ID;
    char text[100];
    TableListElement* next;
    TableListElement(int ID, char* text)
    {
        this->ID = ID;
        sprintf(this->text, "%s", text);
        this->next = 0;
    }
};

class CGUITableList : public CGUIElement {
private:
    int elementsCount;
    TableListElement* elements;
    int shift;

    bool needScrollbox;

    int selectedID;

    CGUIScrollbox scrollbox;

public:
    CGUITableList();

    int GetElementsCount();

    // delete all elements
    void Reset();
    void AddElement(int ID, char* text);
    void DeleteElement(int uniqueID);
    void DeleteElementInOrder(int ID);

    int GetElementID(int ID);
    char* GetElementText(int ID);

    int GetSelectedID();

    void Render();

    void OnInput(unsigned short VK);
    void OnClick();
    void OnMouseMove(int x, int y);
};

#endif