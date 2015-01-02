#include "CGUITableList.h"
#include "CCore.h"

extern CCore* core;

CGUITableList::CGUITableList()
{
	CGUIElement::CGUIElement();
	this->SetType(GUITYPE_TABLELIST);
	this->elements = NULL;
	this->elementsCount = 0;
	this->shift = 0;
	this->needScrollbox = false;
	this->selectedID = -1;
}

void CGUITableList::AddElement(int ID, char* text)
{
	TableListElement* newPointer = new TableListElement(ID,text);

	TableListElement* pointer = this->elements;
	if (pointer != NULL)
	{
		while (pointer->next != NULL)
		{
			pointer = pointer->next;
		}
		pointer->next = newPointer;
	}
	else {
		this->elements = newPointer;
		this->scrollbox.SetUp(this->GetX(), this->GetY(), this->GetWidth(), this->GetHeight());
	}
	elementsCount++;

	if ((unsigned int) (this->elementsCount) * 20 > this->GetHeight())
		this->needScrollbox = true;

	//this->scrollbox.SetUp(this->GetX(), this->GetY(), this->GetWidth(), this->GetHeight());
	this->scrollbox.Update(this->elementsCount * 20);
}

void CGUITableList::DeleteElement(int ID)
{
	this->selectedID = 0;
	TableListElement* pointer = this->elements;
	if (pointer)
	{
		if (pointer->ID == ID)
		{
			this->elements = pointer->next;

			this->elementsCount--;
			delete pointer;
			return;
		}
		else {
			while (pointer->next != NULL)
			{
				if (pointer->next->ID == ID)
				{
					TableListElement* deletePointer = pointer->next;
					pointer->next = deletePointer->next;
					this->elementsCount--;
					delete deletePointer;
					return;
				}
				pointer = pointer->next;
			}
		}
	}
}

void CGUITableList::DeleteElementInOrder(int ID)
{
	int counter = 0;
	this->selectedID = 0;
	TableListElement* pointer = this->elements;
	if (pointer)
	{
		if (counter == ID)
		{
			this->elements = pointer->next;

			this->elementsCount--;
			delete pointer;
			return;
		}
		else {
			while (pointer->next != NULL)
			{
				if (counter+1 == ID)
				{
					TableListElement* deletePointer = pointer->next;
					pointer->next = deletePointer->next;
					this->elementsCount--;
					delete deletePointer;
					return;
				}
				pointer = pointer->next;
				counter++;
			}
		}
	}
}

void CGUITableList::Reset()
{
	TableListElement* pointer = this->elements, *deletePointer = 0;
	while (pointer != NULL)
	{
		deletePointer = pointer;
		pointer = pointer->next;
		delete deletePointer;
	}

	this->elementsCount = 0;
	this->elements = 0;
}

int CGUITableList::GetElementsCount()
{
	return this->elementsCount;
}

void CGUITableList::Render()
{

	this->scrollbox.Render();

	Point2D position = this->GetPosition();
	RenderClip clip = core->getGraphics()->GetRenderBackup();

	if (needScrollbox)
	{
		RenderClip clip;
		core->getGraphics()->SetViewport(&clip,position.x, position.y, this->GetWidth(), this->GetHeight());
	}

	int counter = 0;
	Point2D pos = this->GetPosition();
	TableListElement* pointer = this->elements;
	while (pointer != NULL)
	{
		if (counter == selectedID)
		{
			core->getGraphics()->TextDraw(pos.x, pos.y + (counter * 20) - this->scrollbox.GetShift(), pointer->text, 0xff00adef);
		}
		else
		{
			core->getGraphics()->TextDraw(pos.x, pos.y + (counter * 20) - this->scrollbox.GetShift(), pointer->text, 0xffffFFFF);
		}
		counter++;
		pointer = pointer->next;
	}
	if (needScrollbox)
		core->getGraphics()->RestoreRenderBackup(&clip);

	// DEBUG
	/*char buff[50];
	sprintf(buff, "%d", selectedID);
	core->getGraphics()->TextDraw(pos.x, pos.y-30, buff, 0xff00adef);*/
}


void CGUITableList::OnInput(unsigned short VK)
{
	this->scrollbox.OnKey(VK);
}

void CGUITableList::OnClick()
{
	
	Point2D	pos = this->GetPosition();
	Point2D mouse = core->getGraphics()->GetMouseCoords();
	if ((pos.x + this->GetWidth()-mouse.x) > 10)
	{
		//char buff[200];
		selectedID = (this->scrollbox.GetShift() + mouse.y - pos.y) / 20;
		if (selectedID > this->elementsCount + 1)
			selectedID = this->elementsCount + 1;
		this->GetRoot()->OnElementSelect(this,selectedID);
	}

}

void		CGUITableList::OnMouseMove(int x, int y)
{
	this->scrollbox.OnMouseMove(x, y);
}

int			CGUITableList::GetElementID(int ID)
{
	TableListElement* pointer = this->elements;
	if (pointer != NULL)
	{
		int counter = 0;
		while (pointer->next != NULL)
		{
			if (counter == ID)
			{

				return pointer->ID;
			}
			counter++;
			pointer = pointer->next;
		}
		if (counter == ID)
		{
			return pointer->ID;
		}
	}
	return -1;
}

char*		CGUITableList::GetElementText(int ID)
{
	TableListElement* pointer = this->elements;
	if (pointer != NULL)
	{
		int counter = 0;
		while (pointer->next != NULL)
		{
			if (counter == ID)
			{

				return pointer->text;
			}
			counter++;
			pointer = pointer->next;
		}
		if (counter == ID)
		{
			return pointer->text;
		}
	}
	return "";
}

int			CGUITableList::GetSelectedID()
{
	return this->selectedID;
}