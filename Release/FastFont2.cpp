
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#include <D3DX8.h>

#define DHFF_USE_DHENGINE

#include "dhEngine.h"
#include "dhFPSTimer.h"
#include "dhFastFont.h"

#include <stdio.h>

#pragma comment(lib,"d3d8.lib")

#ifdef _DEBUG
   #pragma comment(lib,"dhEngineD.lib")
   #pragma comment(lib,"d3dx8d.lib")
#else
   #pragma comment(lib,"dhEngine.lib")
   #pragma comment(lib,"d3dx8.lib")
#endif

LRESULT CALLBACK default_window_proc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam);
void Render(void);

dhEngine g_engine;
bool g_app_done=false;

dhFastFont *g_font;

dhFPSTimer g_timer("Fast Font Test2");

int APIENTRY WinMain(HINSTANCE p_instance,HINSTANCE p_prev_instance,LPSTR p_cmd_line,int p_show){

   g_engine.SetEngineAppName("Fast Font Test2");
   g_engine.SetEngineWidth(800);
   g_engine.SetEngineHeight(600);
   g_engine.SetEngineInstance(p_instance);
   g_engine.SetEngineWindowProc(default_window_proc);

   g_engine.SetEngineClearColour(0x00000000);
   g_engine.SetEngineClearZ(1.0f);

   g_engine.SetEngineFullScreen(g_engine.AskFullScreen());

   g_engine.InitWindow();

   g_engine.InitD3D();

   g_timer.SetDevice(g_engine.GetEngineDevice());

#ifdef DHFF_USE_DHENGINE
   g_font=new dhFastFont(&g_engine,"font.tga","font.dat");
#else
   g_font=new dhFastFont(g_engine.GetEngineDevice(),"font.tga","font.dat");
#endif

   if(!g_font->IsOkay()){
      g_engine.dhERROR("Error loading font");
   }

   g_timer.StartTimer();

   while(!g_app_done){

      g_engine.MessagePump();

      Render();
   }

   g_timer.LogTimer();

   delete g_font;

   g_engine.KillD3D();

   g_engine.KillWindow();


   return 0;
}
void Render(void){
char buffer[20];
float fps=0.0f;
DWORD dummy1,dummy2;

   g_engine.ClearC();

   g_timer.CheckTimer(&fps,&dummy1,&dummy2);

   sprintf(buffer,"FPS:%.2f",fps);

   g_font->SetStates();

   //Notify the device that we're ready to render
   if(SUCCEEDED(g_engine.BeginScene())){

      g_engine.SetRenderState(D3DRS_TEXTUREFACTOR,0xFFBBBBBB);
      g_font->DrawFast(buffer,0,0);

      g_engine.SetRenderState(D3DRS_TEXTUREFACTOR,0xFFFFFFFF);
      g_font->DrawFast("I have the best intentions, but the worst ideas.",0,50);

      g_engine.SetRenderState(D3DRS_TEXTUREFACTOR,0xFFFFFF33);
      g_font->DrawFast("What we learn to do, we learn by doing.-Aristotle",0,90);

      g_engine.SetRenderState(D3DRS_TEXTUREFACTOR,0xFF33FFFF);
      g_font->DrawFast("The best mind-altering drug is truth.-Lily Tomlin",0,130);

      g_engine.SetRenderState(D3DRS_TEXTUREFACTOR,0xFFFFFFFF);
      g_font->DrawFast("Hello World! This is a bunch of text!",0,170);


      g_engine.EndScene();
   }

   //Show the results
   g_timer.Present();
}
LRESULT CALLBACK default_window_proc(HWND hwnd,UINT msg,WPARAM wparam,LPARAM lparam){
//char virt_key = (char)wparam;

   switch(msg){
      case WM_KEYDOWN: 
         g_app_done=true;
         return 0;
      case WM_CLOSE:    //User hit the Close Window button, end the app
         g_app_done=true;
         return 0;
      case WM_DESTROY:  //This window is being destroyed, tell Windows we're quitting
         PostQuitMessage(0);
         return 0;
   }

   return (DefWindowProc(hwnd,msg,wparam,lparam));

}
