#include "CGUI.h"
#ifndef C_GUIBUTTON
#define C_GUIBUTTON
//#include "CGUIElement.h"

class CGUIButton : public CGUIElement
{
private:
	char		buttonText[100];

	bool		bIsActive;
	bool		onClick;

	int			textSize;
	unsigned int	backgroundColor;

	bool		backgroundState;
public:
	CGUIButton();
	char*		GetButtonText();
	void		SetButtonText(char*);

	void		SetBackgroundColor(unsigned int color);
	void		SetBackgroundState(bool);


	void		Render();

	void		OnClick();
};

#endif