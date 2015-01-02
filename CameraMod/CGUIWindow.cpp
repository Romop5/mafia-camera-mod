#include "CCore.h"
#include "CGUI.h"

extern CCore *core;
CGUIWindow::CGUIWindow()
{
	CGUIElement::CGUIElement();
	this->SetType(GUITYPE_WINDOW);
	this->SetTitle("Window");
	this->clickState = 0;
}

void CGUIWindow::GetAbsolutePos(Point2D &position)
{
	position.x = this->GetRelativeX();
	position.y = this->GetRelativeY();

	minWidth = 100;
	minHeight = 50;
}

void		CGUIWindow::SetTitle(char* title)
{
	sprintf(this->title, "%s", title);
}
char*		CGUIWindow::GetTitle()
{
	return this->title;
}

void		CGUIWindow::Render()
{

	core->getGraphics()->Clear(this->GetRelativeX(), this->GetRelativeY()+20, this->GetWidth(), this->GetHeight()-20, 0xff000000);
	RenderClip clip;
	core->getGraphics()->SetViewport(&clip, this->GetRelativeX(), this->GetRelativeY() + 20, this->GetWidth(), this->GetHeight());

	CGUIElementsPoolStruct* pointer = this->elements.GetFirst();
	while (pointer != 0)
	{
		if (pointer->element->IsVisible())
			pointer->element->Render();
		pointer = pointer->next;
	}
	core->getGraphics()->RestoreRenderBackup(&clip);

	//core->getGraphics()->Clear(this->GetRelativeX(), this->GetRelativeY(), this->GetWidth(), 20, 0xff00adef);
	core->getGraphics()->FillARGBGradient(this->GetRelativeX(), this->GetRelativeY(), this->GetWidth(), 20, 0x7effFFff, 0x30000000);
	core->getGraphics()->TextDraw(this->GetRelativeX() + 10, this->GetRelativeY(), this->GetTitle(), 0xFFffFFff);


	Point2D ms = core->getGraphics()->GetMouseCoords();
	char buff[100];
	sprintf(buff, "State: %d Pos: %d %d Ms: %d %d", clickState, this->GetRelativeX(), this->GetRelativeY(), ms.x, ms.y);
	core->getGraphics()->TextDraw(this->GetRelativeX(), this->GetRelativeY()-30, buff, 0xFFffFFff);
}


void		CGUIWindow::OnMouseMove(int x, int y)
{

	Point2D posa = core->getGraphics()->GetMouseCoords();
	switch(this->clickState)
	{
	case CLICK_NONE:
		{
			this->SetX(this->GetRelativeX() + (-this->lastMousePoint.x + posa.x));
			this->SetY(this->GetRelativeY() + (-this->lastMousePoint.y + posa.y));
		}
		break;
	case CLICK_UP:
	{
		this->SetY(this->GetRelativeY() + (-this->lastMousePoint.y + posa.y));
		this->SetHeight(this->GetHeight() - (-this->lastMousePoint.y + posa.y));
	}
		break;
	case CLICK_DOWN:
	{
					 //this->SetY(this->GetRelativeY() - (-this->lastMousePoint.y + posa.y));
					   this->SetHeight(this->GetHeight() + (-this->lastMousePoint.y + posa.y));
	}
		break;
	case CLICK_LEFT:
	{
					 this->SetX(this->GetRelativeX() + (-this->lastMousePoint.x + posa.x));
					 this->SetWidth(this->GetWidth() - (-this->lastMousePoint.x + posa.x));
	}
		break;
	case CLICK_RIGHT:
	{
					   //this->SetY(this->GetRelativeY() - (-this->lastMousePoint.y + posa.y));
					   this->SetWidth(this->GetWidth() + (-this->lastMousePoint.x + posa.x));
	}
		break;
	}
	if (clickState != CLICK_NONE)
	{
		if (this->GetWidth() < minWidth)
			this->SetWidth(minWidth);
		if (this->GetHeight() < minHeight)
			this->SetHeight(minHeight);
	}
	this->lastMousePoint = posa;


}

CGUIElement* CGUIWindow::OnHoverCheck(Point2D &point)
{
	if (IsInRect(point.x, point.y, this->GetRelativeX(), this->GetRelativeY(), this->GetRelativeX() + this->GetWidth(), this->GetRelativeY() + this->GetHeight()))
	{
		CGUIElementsPoolStruct* pointer = this->elements.GetFirst();
		while (pointer != 0)
		{
			if (pointer->element->IsVisible())
			{
				CGUIElement* result = pointer->element->OnHoverCheck(point);
				if (result != NULL)
				{
					return result;
				}
			}
			pointer = pointer->next;
		}
		return this;
	}
	return 0;
}

void	CGUIWindow::OnClick()
{
	this->lastMousePoint = core->getGraphics()->GetMouseCoords();
	this->GetRoot()->ToTheTop(this);
	if (this->lastMousePoint.x == this->GetRelativeX())
		this->clickState = CLICK_LEFT;
	else if (this->lastMousePoint.x == this->GetRelativeX() + this->GetWidth()-1)
		this->clickState = CLICK_RIGHT;
	else if (this->lastMousePoint.y == this->GetRelativeY())
		this->clickState = CLICK_UP;
	else if (this->lastMousePoint.y == this->GetRelativeY() + this->GetHeight()-1)
		this->clickState = CLICK_DOWN;
	else
		this->clickState = CLICK_NONE;
}

bool	CGUIWindow::AddElement(CGUIElement* elem)
{
	this->elements.AddElement(elem);
	elem->SetOwner(this);
	return true;
}