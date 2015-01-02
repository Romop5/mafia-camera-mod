#include "CGUIGroup.h"

CGUIGroup::CGUIGroup()
{
	CGUIElement::CGUIElement();
	this->SetType(GUITYPE_GROUP);
}

bool	CGUIGroup::AddElement(CGUIElement* elem)
{
	this->elements.AddElement(elem);
	elem->SetOwner(this);
	return true;
}


void		CGUIGroup::Render()
{
	CGUIElementsPoolStruct* pointer = this->elements.GetFirst();
	while (pointer != 0)
	{
		if (pointer->element->IsVisible())
			pointer->element->Render();
		pointer = pointer->next;
	}

}

CGUIElement* CGUIGroup::OnHoverCheck(Point2D &point)
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
	return 0;
}


CGUIElementsPool*	CGUIGroup::GetElements()
{
	return &this->elements;
}