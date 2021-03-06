#include "CCore.h"
#include "game/CD3D9Accessor.h"
#include "game/CDirect3DDevice8Proxy.h"
#include "game/CImGUIAdaptor.hpp"
//#include "CGraphics.h"
#include <utilslib/logger.hpp>

extern CCore* core;
#pragma comment(lib, "d3dx8.lib")

CGraphics::CGraphics()
{
    g_font = NULL;
    g_mono = NULL;
    betterFont = NULL;
    Texture = NULL;
}

IDirect3DDevice9* CGraphics::getD3D9() const
{
   auto d3d9Object = CD3D9Accessor::castFromD3D8(this->device);
   return d3d9Object;
} 

void CGraphics::Init()
{
    // Note: this is a hack and it requires d3d8to9 to be compiled using the same
    // compilator as CameraMode is due to the fact that C++ may not be binary
    // compatible when compiling classes

    utilslib::Logger::getInfo() << "D3D8 Device: " << std::hex << this->device << std::dec
                      << std::endl;
    auto d3d9Object = CD3D9Accessor::castFromD3D8(this->device);
    utilslib::Logger::getInfo() << "D3D9 Device: " << std::hex << d3d9Object << std::dec
                      << std::endl;

    // auto size = this->GetScreenSize();
    // CLog::getStream() << "Size: " << size.x << " - " << size.y << std::endl;
    utilslib::Logger::getInfo() << "Initializing ImGUI" << std::endl;

    D3DDEVICE_CREATION_PARAMETERS cparams;
    RECT rect;

    device->GetCreationParameters(&cparams);
    GetWindowRect(cparams.hFocusWindow, &rect);
    Point2D point = {rect.right-rect.left, rect.bottom-rect.top};
    //Point2D point = { core->getGame()->getScreenWidth(), core->getGame()->getScreenHeight()};
    adaptor.Initialize(d3d9Object, point);
}

void CGraphics::Unload()
{
    adaptor.CleanUP();
}
void CGraphics::Lost()
{
    adaptor.Invalidate();
}

bool CGraphics::saveScreenshot(const std::string path)
{
    auto d3d8device = this->device;
    IDirect3DSurface8* backbuffer;
    auto result = d3d8device->GetBackBuffer(0,D3DBACKBUFFER_TYPE_MONO, &backbuffer);
    if(D3DERR_INVALIDCALL == result)
    {
        core->getGame()->PrintDebugMessage("get back buffer");
        return false;
    }
    // copy
    IDirect3DSurface8* copySurface;
    auto screenSize = GetScreenSize();
    d3d8device->CreateImageSurface(screenSize.x,screenSize.y,D3DFMT_R8G8B8,&copySurface);
    result = d3d8device->CopyRects(backbuffer,NULL,0,copySurface,NULL);
    if(D3DERR_INVALIDCALL == result)
    {
        core->getGame()->PrintDebugMessage("UpdateSurface fail");
        return false;
    }
    result = D3DXSaveSurfaceToFileA(path.c_str(),D3DXIFF_BMP,copySurface,NULL,NULL);
    copySurface->Release();
    return (result == D3D_OK);
}

void CGraphics::onEndScene()
{
    // Update mouse in ImGui
    //Point2D mouse = this->GetMouseCoords();
    //adaptor.updateMousePosition(mouse);
    //adaptor.Render();

    // call callbacks
    for(auto callback: m_onRenderHandler)
    {
        callback();
    }
}

void CGraphics::SetDevice(IDirect3DDevice8* device) { this->device = device; this->p_Dev = device; }
IDirect3DDevice8* CGraphics::GetDevice() { return this->device; }

void CGraphics::TextDraw(int x, int y, char* text, DWORD color)
{
    if (betterFont)
        betterFont->DrawTextA(static_cast<float>(x), static_cast<float>(y), color, text, D3DFONT_FILTERED);
    // g_font->Begin();
}

int CGraphics::TextDrawLength(char* text)
{
    /*RECT recta = { 0, 0, 0 + 1000, 0 + 100 };
  g_font->DrawTextA(text, strlen(text), &recta, DT_LEFT | DT_SINGLELINE |
  DT_CALCRECT, 0x0); return recta.right-recta.left;
  */
    SIZE fontSize;
    if (betterFont)
        betterFont->GetTextExtent(text, &fontSize);
    else
        return 0;
    return fontSize.cx;
}

void CGraphics::TextMonoDraw(int x, int y, char* text, DWORD color)
{
    RECT recta = { x, y, x + 1000, y + 100 };
    if (g_mono)
        g_mono->DrawTextA(text, strlen(text), &recta, DT_LEFT | DT_SINGLELINE,
            color);
}

void CGraphics::Clear(int x, int y, int width, int height, DWORD color)
{
    D3DRECT rect2 = { x, y, x + width, y + height };
    this->GetDevice()->Clear(1, &rect2, D3DCLEAR_TARGET, color, 0, 0);
}

bool CGraphics::Get2DFrom3D(Vector3D world, Vector3D* screen)
{
    // funguje dobre
    D3DXVECTOR3 out;
    D3DVIEWPORT8 viewPort;
    this->GetDevice()->GetViewport(&viewPort);
    D3DXVECTOR3 input = D3DXVECTOR3(world.x, world.y, world.z);
    D3DXMATRIX mProjection, mView, mWorld;
    this->GetDevice()->GetTransform(D3DTS_VIEW, &mView);
    D3DXMatrixIdentity(&mWorld);
    // m_DirectDevice->GetTransform(D3DTS_WORLD,&mWorld);
    this->GetDevice()->GetTransform(D3DTS_PROJECTION, &mProjection);

    D3DXVec3Project(&out, &input, &viewPort, &mProjection, &mView, &mWorld);
    screen->x = out.x;
    screen->y = out.y;
    screen->z = out.z;

    if (screen->z > 1)
        return false;
    return true;
}

Point2D CGraphics::GetMouseCoords()
{
    POINT p;
    GetCursorPos(&p);

    D3DDEVICE_CREATION_PARAMETERS params;
    this->GetDevice()->GetCreationParameters(&params);
    ScreenToClient(params.hFocusWindow, &p);

    Point2D point;
    point.x = p.x;
    point.y = p.y;
    return point;
}

void CGraphics::OnDeviceLost()
{
    betterFont->InvalidateDeviceObjects();
    if (g_font)
        this->g_font->OnLostDevice();
    if (g_mono)
        this->g_mono->OnLostDevice();
    this->Sprite->OnLostDevice();

    if (this->Texture)
        this->Texture->Release();

    adaptor.Invalidate();
}

void CGraphics::OnDeviceReset()
{
    betterFont->RestoreDeviceObjects();

    if (g_font)
        this->g_font->OnResetDevice();
    if (g_mono)
        this->g_mono->OnResetDevice();
    this->Sprite->OnResetDevice();

    /*HRESULT DTEX = D3DXCreateTextureFromResourceEx(
        this->GetDevice(), GetModuleHandleA("CameraMod.dll"),
        MAKEINTRESOURCEA(IDB_BITMAP1), D3DX_DEFAULT, D3DX_DEFAULT, 0,
        D3DUSAGE_DYNAMIC, D3DFMT_UNKNOWN, D3DPOOL_DEFAULT, D3DX_FILTER_LINEAR,
        D3DX_FILTER_LINEAR, D3DCOLOR_XRGB(0, 0, 0), NULL, NULL, &Texture);
        */
}

RenderClip CGraphics::GetRenderBackup()
{
    RenderClip clip;
    this->GetDevice()->GetTransform(D3DTS_PROJECTION, &clip.projection);
    this->GetDevice()->GetViewport(&clip.viewport);
    return clip;
}
void CGraphics::RestoreRenderBackup(RenderClip* clip)
{
    this->GetDevice()->SetTransform(D3DTS_PROJECTION, &clip->projection);
    this->GetDevice()->SetViewport(&clip->viewport);
}

void CGraphics::SetViewport(RenderClip* backup, DWORD x, DWORD y, DWORD width,
    DWORD heigh)
{
    this->GetDevice()->GetTransform(D3DTS_PROJECTION, &backup->projection);
    this->GetDevice()->GetViewport(&backup->viewport);
    // int x, y;
    D3DVIEWPORT8 port;
    /*if (backup->viewport.X <= x && backup->viewport.X + backup->viewport.Width
  >= x) port.X = x; else port.X = backup->viewport.X; if (backup->viewport.Y <=
  y && backup->viewport.Y + backup->viewport.Height >= y) port.Y = y; else
          port.Y = backup->viewport.Y;

  int sizeX = x+width, sizeY = y+heigh;

  if (backup->viewport.X <= sizeX && backup->viewport.X + backup->viewport.Width
  >= sizeX) port.Width = sizeX-x; else port.Width = backup->viewport.X +
  backup->viewport.Width - x; if (backup->viewport.Y <= sizeY &&
  backup->viewport.Y + backup->viewport.Height >= sizeY) port.Height = sizeY -
  y; else port.Height = backup->viewport.Y + backup->viewport.Height - y;
*/

    if (IsInRect(x, y, backup->viewport.X, backup->viewport.Y,
            backup->viewport.X + backup->viewport.Width,
            backup->viewport.Y + backup->viewport.Height)) {
        if (IsInRect(x + width, y + heigh, backup->viewport.X, backup->viewport.Y,
                backup->viewport.X + backup->viewport.Width,
                backup->viewport.Y + backup->viewport.Height)) {
            port.X = x;
            port.Y = y;
            port.Width = width;
            port.Height = heigh;
        } else {
            port.X = x;
            port.Y = y;
            port.Width = (backup->viewport.X + backup->viewport.Width - x);
            port.Height = (backup->viewport.Y + backup->viewport.Height - y);
        }
    } else if (IsInRect(x + width, y + heigh, backup->viewport.X,
                   backup->viewport.Y,
                   backup->viewport.X + backup->viewport.Width,
                   backup->viewport.Y + backup->viewport.Height)) {
        port.X = backup->viewport.X;
        port.Y = backup->viewport.Y;
        port.Width = (x + width) - port.X;
        port.Height = (y + heigh) - port.Y;
    } else {
        return;
    }

    D3DXMATRIX projection;

    /*port.X = x;
  port.Y = y;
  port.Width = width;
  port.Height = heigh;
  */
    port.MinZ = 0.0f;
    port.MaxZ = 1.0f;

    this->GetDevice()->SetViewport(&port);
}


void CGraphics::DrawCubePoint(Vector3D position)
{
    DWORD color = 0xFF00adef;
    int x = 200;
    int y = 100;
    int w = 50;
    int h = 100;

    struct QuadVertex {
        float x, y, z, rhw;
        DWORD dwColor;
    };
    QuadVertex qV[4];

    qV[0].dwColor = qV[1].dwColor = qV[2].dwColor = qV[3].dwColor = color;
    qV[0].z = qV[1].z = qV[2].z = qV[3].z = 0.0f;
    qV[0].rhw = qV[1].rhw = qV[2].rhw = qV[3].rhw = 0.0f;

    qV[0].x = (float)x;
    qV[0].y = (float)(y + h);
    qV[1].x = (float)x;
    qV[1].y = (float)y;
    qV[2].x = (float)(x + w);
    qV[2].y = (float)(y + h);
    qV[3].x = (float)(x + w);
    qV[3].y = (float)y;

    /*m_DirectDevice->SetRenderState( D3DRS_ALPHABLENDENABLE,true );
  m_DirectDevice->SetRenderState(D3DRS_ZENABLE, D3DZB_FALSE);
  m_DirectDevice->SetRenderState( D3DRS_SRCBLEND,  D3DBLEND_SRCALPHA );
  m_DirectDevice->SetRenderState( D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA );*/

    D3DXMATRIX trans_matrix, view, pro;
    this->GetDevice()->GetTransform(D3DTS_VIEW, &view);
    this->GetDevice()->GetTransform(D3DTS_PROJECTION, &pro);
    D3DXMatrixTranslation(&trans_matrix, -1.5, 0.0f, 50.0f);
    this->GetDevice()->SetTransform(D3DTS_WORLD, &trans_matrix);

    this->GetDevice()->SetTexture(0, NULL);

    this->GetDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);

    this->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    this->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    this->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    this->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    this->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0x08);
    this->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    this->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    this->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    this->GetDevice()->SetRenderState(D3DRS_STENCILENABLE, FALSE);
    this->GetDevice()->SetRenderState(D3DRS_CLIPPING, TRUE);
    this->GetDevice()->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
    this->GetDevice()->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
    this->GetDevice()->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
    this->GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);
    this->GetDevice()->SetRenderState(
        D3DRS_COLORWRITEENABLE,
        D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS,
        D3DTTFF_DISABLE);
    this->GetDevice()->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    this->GetDevice()->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

    this->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, qV,
        sizeof(QuadVertex));
}

void CGraphics::FillARGB(int x, int y, int w, int h, D3DCOLOR color)
{
    // const DWORD D3D_FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

    struct QuadVertex {
        float x, y, z, rhw;
        DWORD dwColor;
    };
    QuadVertex qV[4];

    qV[0].dwColor = qV[1].dwColor = qV[2].dwColor = qV[3].dwColor = color;
    qV[0].z = qV[1].z = qV[2].z = qV[3].z = 0.0f;
    qV[0].rhw = qV[1].rhw = qV[2].rhw = qV[3].rhw = 0.0f;

    qV[0].x = (float)x;
    qV[0].y = (float)(y + h);
    qV[1].x = (float)x;
    qV[1].y = (float)y;
    qV[2].x = (float)(x + w);
    qV[2].y = (float)(y + h);
    qV[3].x = (float)(x + w);
    qV[3].y = (float)y;

    this->GetDevice()->SetTexture(0, NULL);

    this->GetDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);

    this->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    this->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    this->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    this->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    this->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0x00);
    this->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    this->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    this->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    this->GetDevice()->SetRenderState(D3DRS_STENCILENABLE, FALSE);
    this->GetDevice()->SetRenderState(D3DRS_CLIPPING, TRUE);
    this->GetDevice()->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
    this->GetDevice()->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
    this->GetDevice()->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
    this->GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);
    this->GetDevice()->SetRenderState(
        D3DRS_COLORWRITEENABLE,
        D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS,
        D3DTTFF_DISABLE);
    this->GetDevice()->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    this->GetDevice()->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

    this->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, qV,
        sizeof(QuadVertex));
}

void CGraphics::FillARGBGradient(int x, int y, int w, int h, D3DCOLOR color,
    D3DCOLOR colorSec)
{
    // const DWORD D3D_FVF = D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1;

    struct QuadVertex {
        float x, y, z, rhw;
        DWORD dwColor;
    };
    QuadVertex qV[4];

    qV[0].dwColor = qV[2].dwColor = colorSec;
    qV[1].dwColor = qV[3].dwColor = color;

    // qV[0].dwColor = qV[1].dwColor = qV[2].dwColor = qV[3].dwColor = color;
    qV[0].z = qV[1].z = qV[2].z = qV[3].z = 0.0f;
    qV[0].rhw = qV[1].rhw = qV[2].rhw = qV[3].rhw = 0.0f;

    qV[0].x = (float)x;
    qV[0].y = (float)(y + h);
    qV[1].x = (float)x;
    qV[1].y = (float)y;
    qV[2].x = (float)(x + w);
    qV[2].y = (float)(y + h);
    qV[3].x = (float)(x + w);
    qV[3].y = (float)y;

    this->GetDevice()->SetTexture(0, NULL);

    this->GetDevice()->SetRenderState(D3DRS_ZENABLE, FALSE);

    this->GetDevice()->SetRenderState(D3DRS_ALPHABLENDENABLE, TRUE);
    this->GetDevice()->SetRenderState(D3DRS_SRCBLEND, D3DBLEND_SRCALPHA);
    this->GetDevice()->SetRenderState(D3DRS_DESTBLEND, D3DBLEND_INVSRCALPHA);
    this->GetDevice()->SetRenderState(D3DRS_ALPHATESTENABLE, TRUE);
    this->GetDevice()->SetRenderState(D3DRS_ALPHAREF, 0x00);
    this->GetDevice()->SetRenderState(D3DRS_ALPHAFUNC, D3DCMP_GREATEREQUAL);
    this->GetDevice()->SetRenderState(D3DRS_FILLMODE, D3DFILL_SOLID);
    this->GetDevice()->SetRenderState(D3DRS_CULLMODE, D3DCULL_CCW);
    this->GetDevice()->SetRenderState(D3DRS_STENCILENABLE, FALSE);
    this->GetDevice()->SetRenderState(D3DRS_CLIPPING, TRUE);
    this->GetDevice()->SetRenderState(D3DRS_CLIPPLANEENABLE, FALSE);
    this->GetDevice()->SetRenderState(D3DRS_VERTEXBLEND, D3DVBF_DISABLE);
    this->GetDevice()->SetRenderState(D3DRS_INDEXEDVERTEXBLENDENABLE, FALSE);
    this->GetDevice()->SetRenderState(D3DRS_FOGENABLE, FALSE);
    this->GetDevice()->SetRenderState(
        D3DRS_COLORWRITEENABLE,
        D3DCOLORWRITEENABLE_RED | D3DCOLORWRITEENABLE_GREEN | D3DCOLORWRITEENABLE_BLUE | D3DCOLORWRITEENABLE_ALPHA);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_COLOROP, D3DTOP_MODULATE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG1, D3DTA_TEXTURE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_COLORARG2, D3DTA_DIFFUSE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAOP, D3DTOP_MODULATE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_ALPHAARG2, D3DTA_DIFFUSE);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_TEXCOORDINDEX, 0);
    this->GetDevice()->SetTextureStageState(0, D3DTSS_TEXTURETRANSFORMFLAGS,
        D3DTTFF_DISABLE);
    this->GetDevice()->SetTextureStageState(1, D3DTSS_COLOROP, D3DTOP_DISABLE);
    this->GetDevice()->SetTextureStageState(1, D3DTSS_ALPHAOP, D3DTOP_DISABLE);

    this->GetDevice()->DrawPrimitiveUP(D3DPT_TRIANGLESTRIP, 2, qV,
        sizeof(QuadVertex));
}

Point2D CGraphics::GetScreenSize()
{
    D3DDEVICE_CREATION_PARAMETERS cparams;
    RECT rect;

    device->GetCreationParameters(&cparams);
    GetWindowRect(cparams.hFocusWindow, &rect);
    Point2D point = {rect.right-rect.left, rect.bottom-rect.top};
    return point;
}

