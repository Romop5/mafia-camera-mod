#include "CGUIButton.h"
#include <stdio.h>
#include "CCore.h"

extern CCore* core;

CGUIButton::CGUIButton()
{
	CGUIElement::CGUIElement();
	this->bIsActive = true;
	sprintf(this->buttonText, "%s", "Button");
	this->onClick = false;

	this->backgroundColor = D3DCOLOR_XRGB(250, 0, 0);
	this->backgroundState = true;
}

char*		CGUIButton::GetButtonText()
{
	return this->buttonText;
}

void		CGUIButton::SetButtonText(char* text)
{
	sprintf(this->buttonText, "%s", text);
	this->textSize = core->getGraphics()->TextDrawLength(text);
}

void		CGUIButton::SetBackgroundColor(unsigned int color)
{
	this->backgroundColor = color;
}

void	CGUIButton::Render()
{
	DWORD color = 0x0,textColor = 0xffFFffFF;
	if (this->onClick)
	{
		this->onClick = false;
		color = D3DCOLOR_XRGB(255, 255, 255);
		textColor = D3DCOLOR_XRGB(99, 99, 99);
	}
	else {
		if (this->GetRoot()->GetHoverElement() == this)
		{
			color = core->getGraphics()->ColorMinusColor(this->backgroundColor, 0x707070);
			textColor = D3DCOLOR_XRGB(200,200,200);
		} else
			color = this->backgroundColor;
		//color = D3DCOLOR_XRGB(250, 0, 0);	
	}
	if (this->backgroundState)
	{
		core->getGraphics()->Clear(this->GetPosition().x, this->GetPosition().y, this->GetWidth(), this->GetHeight(), color);
		core->getGraphics()->Clear(this->GetPosition().x, this->GetPosition().y + this->GetHeight() - 3, this->GetWidth(), 3, core->getGraphics()->ColorMinusColor(color, 0x00333333));
	}
	RenderClip clip;
	core->getGraphics()->SetViewport(&clip,this->GetX() + 5, this->GetY(), this->GetWidth() - 10, this->GetHeight());

	int newX = this->GetWidth()/2 - this->textSize / 2;
	if (this->backgroundState)
		core->getGraphics()->TextDraw(this->GetPosition().x+newX, this->GetPosition().y, this->GetButtonText(), 0xffffffff);
	else
		core->getGraphics()->TextDraw(this->GetPosition().x + newX, this->GetPosition().y, this->GetButtonText(), textColor);

	//core->getGraphics()->GetDevice()->Clear(0, NULL, D3DCLEAR_TARGET, D3DCOLOR_XRGB(0, 40, 100), 1.0f, 0);
	//core->getGraphics()->Clear(this->GetX() + newX, this->GetY(), 30, 10, 0xff00adef);
	core->getGraphics()->RestoreRenderBackup(&clip);
}

void CGUIButton::OnClick()
{
	this->onClick = true;
}

void CGUIButton::SetBackgroundState(bool state)
{
	this->backgroundState = state;
}