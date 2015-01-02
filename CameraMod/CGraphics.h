#include "structures.h"
#include "../vendors/cd3dfont/d3dfont.h"

#ifndef C_GRAPHICS
#define C_GRAPHICS
//#include "../vendors/DirectX9SDK2007/Include/d3d8.h"
//#include "../vendors/dx8sdk/include/d3dx8.h"

#include "d3d8.h"
#include "d3dx8.h"

struct BasicVertex {
	float x, y, z, rhw;
	DWORD dwColor;
};

struct RenderClip
{
	D3DVIEWPORT8 viewport;
	D3DMATRIX	projection;
};

class CGraphics
{
private:
	IDirect3DDevice8*	device;
	void*	originalEndScene;
	LPD3DXFONT g_font,g_mono;
	HFONT				m_chatfontAPI;

	LPD3DXSPRITE Sprite;
	LPDIRECT3DTEXTURE8 Texture;
	bool TexturesLoaded = false;

	BasicVertex		vertexCube[18];
	CD3DFont*	betterFont;
public:
	CGraphics();
	void	Init();
	void	Unload();
	__declspec(noinline) void	onEndScene();
	void	SetDevice(IDirect3DDevice8* device);
	IDirect3DDevice8* GetDevice();
	void	SetOriginalEndScene(void* address);

	// callbacks

	void OnDeviceLost();
	void OnDeviceReset();
	// functions

	void TextDraw(int x, int y,char* text,DWORD color);
	int  TextDrawLength(char* text);
	void TextMonoDraw(int x, int y, char* text, DWORD color);

	void Clear(int x, int y, int width, int height, DWORD color);
	bool Get2DFrom3D(Vector3D world, Vector3D *screen);
	Point2D GetMouseCoords();

	RenderClip GetRenderBackup();
	void		RestoreRenderBackup(RenderClip*);
	//void		SetViewport(DWORD x, DWORD y, DWORD width, DWORD heigh);
	void		SetViewport(RenderClip*,DWORD x, DWORD y, DWORD width, DWORD heigh);

	DWORD		ColorPlusColor(DWORD, DWORD);
	DWORD		ColorMinusColor(DWORD,DWORD);

	void		DrawCubePoint(Vector3D position);

	void		FillARGB(int x, int y, int w, int h, D3DCOLOR color);
	void		FillARGBGradient(int x, int y, int w, int h, D3DCOLOR color, D3DCOLOR colorSec);

	Point2D		GetScreenSize();
};

#endif