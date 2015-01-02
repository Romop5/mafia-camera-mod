#include "CGUI.h"
#ifndef C_GUIELEMENT
#define	C_GUIELEMENT
#include "structures.h"

enum GUIElementType
{
	GUITYPE_DEFAULT,
	GUITYPE_GROUP,
	GUITYPE_BUTTON,
	GUITYPE_CHECKBOX,
	GUITYPE_TABLELIST,
	GUITYPE_LABEL,
	GUITYPE_WINDOW
};
class CGUIElement
{
private:
	CGUI*			root;
	CGUIElement*	owner;
	Point2D			position;
	unsigned int	width, height;
	bool			isVisible;
	unsigned char	type;
	bool			hasInput;

public:
	CGUIElement();

	void			SetRoot(CGUI*);
	CGUI*			GetRoot();

	void			SetOwner(CGUIElement*);
	CGUIElement*	GetOwner();

	void			SetPosition(Point2D pos);
	Point2D			GetPosition();

	void			SetX(int);
	void			SetY(int);
	int				GetX();
	int				GetY();

	int				GetRelativeX();
	int				GetRelativeY();



	void			SetWidth(unsigned int);
	unsigned int	GetWidth();

	void			SetHeight(unsigned int);
	unsigned int	GetHeight();

	void			SetVisible(bool);
	bool			IsVisible();

	void			SetType(unsigned char);
	unsigned char	GetType();

	void			SetHasInput(bool state);
	bool			HasInput();

	virtual void	Render() {};

	virtual void	OnHover() {};
	virtual CGUIElement*	OnHoverCheck(Point2D &point);
	virtual void	OnClick() {};
	virtual void	OnClickEnd() {};
	virtual void	OnMouseMove(int x, int y) {};
	virtual void	OnInput(unsigned short VK) {};

	virtual void	GetAbsolutePos(Point2D &position);

};

#endif