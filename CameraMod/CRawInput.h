#include "CCore.h"
#ifndef C_RAWINPUT
#define C_RAWINPUT

class CRawInput
{
private:
	bool isClickedLeft;
	bool isClickedRight;
public:
	void ProcessMessage(LPMSG message);
	void OnVKKey(USHORT vk);
	void OnMouseTick(RAWMOUSE* mouse);
	LONG x, y;
	char debuf[500];
};

#endif