#include "CGUIElementsPool.h"
#define NULL 0

CGUIElementsPool::CGUIElementsPool()
{
	this->pool = NULL;
	this->pointer = NULL;
	this->size = 0;
}

void				CGUIElementsPool::AddElement(CGUIElement* elem)
{
	CGUIElementsPoolStruct* newStruct = new CGUIElementsPoolStruct();
	newStruct->element = elem;
	newStruct->next = this->pool;
	this->pool = newStruct;
}

void				CGUIElementsPool::DeleteElement(CGUIElement* elem)
{
	CGUIElementsPoolStruct *now = this->pool, *parent = NULL;

	while (now != NULL)
	{
		if (now->element == elem)
		{
			if (parent == NULL)
				this->pool = now->next;
			else
				parent = now->next;

			delete now;
			return;
		}
		parent = now;
		now = now->next;
	}

}

CGUIElement*		CGUIElementsPool::GetNext()
{
	if (this->pointer)
	{
		CGUIElement* pt = this->pointer->element;
		this->pointer = this->pointer->next;
		return pt;
	}
	return false;
}

void				CGUIElementsPool::Reset()
{
	this->pointer = this->pool;
}


CGUIElementsPoolStruct*		CGUIElementsPool::GetFirst()
{
	return this->pool;
}


void	CGUIElementsPool::ToTheTop(CGUIElement* elem)
{
	CGUIElementsPoolStruct *now = this->pool, *parent = NULL;

	while (now != NULL)
	{
		if (now->element == elem)
		{
			if (parent != NULL)
			{
				parent->next = now->next;
				now->next = this->pool;
				this->pool = now;
			}
			return;
		}
		parent = now;
		now = now->next;
	}
}