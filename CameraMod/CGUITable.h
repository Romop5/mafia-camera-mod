#include "CGUI.h"
#ifndef C_GUITABLE
#define C_GUITABLE

struct Column
{
	char text[200];
	Column*	next;
	Column()
	{
		text[0] = 0x0;
	}
};

struct Row
{
	int rowID;
	Column*	columns;
	Row* next;
	Row(int ID, int columnsCount)
	{
		rowID = ID;
		columns = new Column();
		Column* pointer = columns;
		for (int i = 0; i < columnsCount-1; i++)
		{
			pointer->next = new Column();
			pointer = pointer->next;
		}
	}
};


class CGUITable : public CGUIElement
{
private:
	
public:
	CGUITable(int columnCount);
	

	void		Render();

	void		OnClick();
};

#endif