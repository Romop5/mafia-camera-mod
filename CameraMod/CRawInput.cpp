#include "CCore.h"

extern CCore *core;
// RAW Input process taken from MSDN, just google it
void CRawInput::ProcessMessage(LPMSG message)
{
	UINT dwSize;

	GetRawInputData((HRAWINPUT)message->lParam, RID_INPUT, NULL, &dwSize,
		sizeof(RAWINPUTHEADER));
	LPBYTE lpb = new BYTE[dwSize];
	if (lpb)
	{

		if (GetRawInputData((HRAWINPUT)message->lParam, RID_INPUT, lpb, &dwSize,
			sizeof(RAWINPUTHEADER)) == dwSize)
		{

			RAWINPUT* raw = (RAWINPUT*)lpb;

			if (raw->header.dwType == RIM_TYPEKEYBOARD)
			{

				if (!(raw->data.keyboard.Flags & RI_KEY_BREAK))
					this->OnVKKey(raw->data.keyboard.VKey);
				
			}
			else if (raw->header.dwType == RIM_TYPEMOUSE)
			{
				//hResult = StringCchPrintf(szTempOutput, STRSAFE_MAX_CCH, TEXT("Mouse: usFlags=%04x ulButtons=%04x usButtonFlags=%04x usButtonData=%04x ulRawButtons=%04x lLastX=%04x lLastY=%04x ulExtraInformation=%04x\r\n"),
				/*	raw->data.mouse.usFlags,
					raw->data.mouse.ulButtons,
					raw->data.mouse.usButtonFlags,
					raw->data.mouse.usButtonData,
					raw->data.mouse.ulRawButtons,
					raw->data.mouse.lLastX,
					raw->data.mouse.lLastY,
					raw->data.mouse.ulExtraInformation);
					*/

				this->OnMouseTick(&raw->data.mouse);
			}
			delete[] lpb;
		}
	}
}


void CRawInput::OnVKKey(USHORT vk)
{
	core->getModControl()->OnVKKey(vk);
}


void CRawInput::OnMouseTick(RAWMOUSE* mouse)
{
	this->x = mouse->lLastX;
	this->y = mouse->lLastY;

	core->getModControl()->OnMouseMove(x, y);
	core->getModControl()->OnMouseButtons(mouse->usButtonFlags);

	if (core->getModControl()->GetState() == CMOD_PICKING)
	{
		core->getGUI()->OnMouseMove(x, y);
		core->getGUI()->OnMouseButtons(mouse->usButtonFlags);
	}

	sprintf(this->debuf, "OnMouseTick %x",mouse->usButtonFlags);
}