#ifndef DHFASTFONT_H_
#define DHFASTFONT_H_

//******************************************************************
// Dev Notes:
//******************************************************************
// - It was recommended that this could be sped up by having a pool
//   of vertex buffers and cycling between them.  I tried this and
//   got no noticable speed up.  I tried various values between 2
//   and 6 with no benefit.  The Dynamic/Discard combo seems to work
//   well enough that extra trickery is not required.
//*******************************************************************

//*******************************************************************
// Change Log
//*******************************************************************
// - Version 0.2 - April 9, 2002
//
// - Fixed declarations to be 'inline' so it could be included into
//   multiple files. Oops. - April 4, 2002
//
// - Fixed a few places where the dhEngine wasn't used when requested
//   so it's even faster now - December 29, 2001
//
// - Retired the old Draw & DrawFast, and renames the DrawPretty to
//   Draw and DrawPrettyFast to DrawFast - December 30, 2001
//
// - Original Version 0.1
//*******************************************************************

#include <d3dx8.h>
#include <stdio.h>

#ifdef DHFF_USE_DHENGINE
   #define DHFF_DEVORENG m_engine
   #define DHFF_DEVORENG_TYPE dhEngine
#else
   #define DHFF_DEVORENG m_device
   #define DHFF_DEVORENG_TYPE IDirect3DDevice8
#endif


class dhFastFont{

protected:

   enum{
      cMaxChars = 80,
      cMaxVerts = cMaxChars * 6,
      cVertexFVF = (D3DFVF_XYZRHW|D3DFVF_TEX1)
   };

   struct FFSVertex{
      float x, y, z, rhw;
      float tu,tv;

      void Init(float p_x,float p_y,float p_tu,float p_tv){
         x=p_x;
         y=p_y;
         tu=p_tu;
         tv=p_tv;
         rhw=1.0f;
         z=0.0f;
      }
   };

#ifdef DHFF_USE_DHENGINE
   dhEngine *m_engine;
#endif

   IDirect3DTexture8 *m_font_texture;

   IDirect3DDevice8 *m_device;

   IDirect3DVertexBuffer8 *m_vb;

   int m_tex_width;
   int m_tex_height;
   float m_char_width;
   float m_char_height;

   char m_metrics[256];

   bool m_is_okay;
   bool m_has_metrics;

   void init_quad(char p_char,float p_x,float p_y,FFSVertex *p_verts);
   void init_quad_pretty(char p_char,float p_x,float p_y,FFSVertex *p_verts);

   inline HRESULT init(const char *p_metrics_name);
   inline bool draw(const char *p_text,float p_x,float p_y);
   inline bool draw_pretty(const char *p_text,float p_x,float p_y);

public:
   inline dhFastFont(void);
   inline dhFastFont(DHFF_DEVORENG_TYPE *p_device,const char *p_file_name,const char *p_metrics_name=NULL);

   inline void Init(DHFF_DEVORENG_TYPE *p_engine,const char *p_file_name,const char *p_metrics_name=NULL);

   ~dhFastFont(void);

   void Kill(void);

   bool IsOkay(void);

   bool Draw(const char *p_text,float p_x,float p_y,DWORD p_colour);
   bool DrawFast(const char *p_text,float p_x,float p_y);

   void SetStates(void);
};

inline dhFastFont::dhFastFont(void){

   m_font_texture=NULL;

   m_vb=NULL;

   m_tex_width=-1;
   m_tex_height=-1;
   m_char_width=-1;
   m_char_height=-1;

   memset(m_metrics,0,256);

   m_is_okay=false;
   m_has_metrics=false;
}
inline dhFastFont::dhFastFont(DHFF_DEVORENG_TYPE *p_device,const char *p_file_name,const char *p_metrics_name){

   Init(p_device,p_file_name,p_metrics_name);

}
inline HRESULT dhFastFont::init(const char *p_metrics_name){
D3DSURFACE_DESC desc;
HRESULT hr;
FILE *metric_file;

   m_font_texture->GetLevelDesc(0,&desc);

   m_tex_width=desc.Width;
   m_tex_height=desc.Height;

   m_char_width=m_tex_width/16.0f;
   m_char_height=m_tex_height/16.0f;

   m_has_metrics=false;

   
   if(p_metrics_name!=NULL){
 
      //ToDo:put this into its own read_metrics routine
      metric_file=fopen(p_metrics_name, "rb");
      if (!metric_file){
         #if defined(_DEBUG) && !defined(DHFF_USE_DHENGINE)
            OutputDebugString("dhFastFont::init Error loading Font Metrics - doesn't exist");
         #elif defined(DHFF_USE_DHENGINE)
            m_engine->Log("dhFastFont::init Error open font metric file:%s - doesn't exist\n",p_metrics_name);
         #endif
      }else if(fread(m_metrics,1,256,metric_file) != 256){
         #if defined(_DEBUG) && !defined(DHFF_USE_DHENGINE)
            OutputDebugString("dhFastFont::init Error loading Font Metrics");
         #elif defined(DHFF_USE_DHENGINE)
            m_engine->Log("dhFastFont::init Error open font metric file:%s\n",p_metrics_name);
         #endif
      }else{
         m_has_metrics=true;
      }
      if(metric_file){
         fclose(metric_file);
      }

   }

   
   hr=m_device->CreateVertexBuffer((cMaxVerts+1)*sizeof(FFSVertex),
                                   D3DUSAGE_WRITEONLY|D3DUSAGE_DYNAMIC ,
                                   cVertexFVF,
                                   D3DPOOL_DEFAULT,
                                   &m_vb);
   return hr;
}

inline void dhFastFont::Kill(void){

   if(m_font_texture){
      m_font_texture->Release();
      m_font_texture=NULL;
   }

   if(m_vb){
      m_vb->Release();
      m_vb=NULL;
   }
}
inline dhFastFont::~dhFastFont(void){

   Kill();

}


inline bool dhFastFont::IsOkay(void){

   return m_is_okay;

}
inline void dhFastFont::init_quad(char p_char,float p_x,float p_y,FFSVertex *p_verts){
float tu,tv;
float tu2,tv2;
float x2,y2;

   x2=p_x+m_char_width;
   y2=p_y+m_char_height;

   tu=(p_char%16)/16.0f;
   tu2=tu + 1/16.0f;

   tv=(p_char - (p_char%16))/256.0f;
   tv2=tv+ 1/16.0f;

   p_verts[0].Init(p_x,y2, tu, tv2);
   p_verts[1].Init(p_x,p_y,tu, tv);
   p_verts[2].Init(x2, p_y,tu2,tv);
   p_verts[3]=p_verts[0];
   p_verts[4]=p_verts[2];
   p_verts[5].Init(x2, y2, tu2,tv2);
}
inline void dhFastFont::init_quad_pretty(char p_char,float p_x,float p_y,FFSVertex *p_verts){
float tu,tv;
float tu2,tv2;
float x2,y2;
float width_scale;

   x2=p_x+m_metrics[p_char];
   y2=p_y+m_char_height;

   width_scale=m_metrics[p_char]/m_char_width;

   tu=(p_char%16)/16.0f;
   tu2=tu + ((1/16.0f)*width_scale);

   tv=(p_char - (p_char%16))/256.0f;
   tv2=tv+ 1/16.0f;

   p_verts[0].Init(p_x,y2, tu, tv2);
   p_verts[1].Init(p_x,p_y,tu, tv);
   p_verts[2].Init(x2, p_y,tu2,tv);
   p_verts[3]=p_verts[0];
   p_verts[4]=p_verts[2];
   p_verts[5].Init(x2, y2, tu2,tv2);
}
inline void dhFastFont::SetStates(void){

   DHFF_DEVORENG->SetTextureStageState( 0, D3DTSS_COLOROP,   D3DTOP_MODULATE );
   DHFF_DEVORENG->SetTextureStageState( 0, D3DTSS_COLORARG1, D3DTA_TEXTURE );
   DHFF_DEVORENG->SetTextureStageState( 0, D3DTSS_COLORARG2, D3DTA_TFACTOR );

   DHFF_DEVORENG->SetRenderState( D3DRS_ALPHABLENDENABLE, TRUE );
   DHFF_DEVORENG->SetRenderState( D3DRS_SRCBLEND,   D3DBLEND_SRCALPHA );
   DHFF_DEVORENG->SetRenderState( D3DRS_DESTBLEND,  D3DBLEND_INVSRCALPHA );

   DHFF_DEVORENG->SetTextureStageState( 0, D3DTSS_ALPHAARG1, D3DTA_TEXTURE );
   DHFF_DEVORENG->SetTextureStageState( 0, D3DTSS_ALPHAARG2, D3DTA_TFACTOR );
   DHFF_DEVORENG->SetTextureStageState( 0, D3DTSS_ALPHAOP,   D3DTOP_MODULATE );

   m_device->SetTexture(0, m_font_texture);

   m_device->SetStreamSource(0,m_vb,sizeof(FFSVertex));
   m_device->SetVertexShader(cVertexFVF);

}
inline bool dhFastFont::draw(const char *p_text,float p_x,float p_y){
FFSVertex *vert_ptr;
int counter,length;
float x;

   if(strlen(p_text) > cMaxChars){
      length=cMaxChars;
   }else{
      length=strlen(p_text);
   }

   m_vb->Lock(0,0,(unsigned char **)&vert_ptr,D3DLOCK_DISCARD );

   for(counter=0;counter < length;counter++){
      x=p_x + counter*m_char_width;

      init_quad(p_text[counter],x,p_y,vert_ptr);

      vert_ptr+=6;
   }

   m_vb->Unlock();


   m_device->DrawPrimitive(D3DPT_TRIANGLELIST,0,length*2);

   
   return true;

}
inline bool dhFastFont::draw_pretty(const char *p_text,float p_x,float p_y){
FFSVertex *vert_ptr;
int counter,length;
float x;
char last_char;

   if(strlen(p_text) > cMaxChars){
      length=cMaxChars;
   }else{
      length=strlen(p_text);
   }

   x=p_x;

   m_vb->Lock(0,0,(unsigned char **)&vert_ptr,D3DLOCK_DISCARD );

   for(counter=0;counter < length;counter++){
      if(counter!=0){
         last_char=p_text[counter-1];
         x+=m_metrics[last_char];
      }

      init_quad_pretty(p_text[counter],x,p_y,vert_ptr);

      vert_ptr+=6;
   }

   m_vb->Unlock();


   m_device->DrawPrimitive(D3DPT_TRIANGLELIST,0,length*2);

   
   return true;

}

inline bool dhFastFont::Draw(const char *p_text,float p_x,float p_y,DWORD p_colour){
bool ret;

   SetStates();

   DHFF_DEVORENG->SetRenderState(D3DRS_TEXTUREFACTOR,p_colour);

   if(m_has_metrics){
      ret=draw_pretty(p_text,p_x,p_y);
   }else{
      ret=draw(p_text,p_x,p_y);
   }

   m_device->SetTexture(0, NULL);

   
   return ret;
}

inline bool dhFastFont::DrawFast(const char *p_text,float p_x,float p_y){

   if(m_has_metrics){
      return draw_pretty(p_text,p_x,p_y);
   }else{
      return draw(p_text,p_x,p_y);
   }

}

// *************************************************************************************
// ************** Functions to support using the dhEngine*******************************
// *************************************************************************************
#ifdef DHFF_USE_DHENGINE

inline void dhFastFont::Init(dhEngine *p_engine,const char *p_file_name,const char *p_metrics_name){
HRESULT hr;

   m_device=p_engine->GetEngineDevice();
   m_engine=p_engine;

   hr=m_engine->CreateTextureFromFile(p_file_name,&m_font_texture,D3DFMT_A8R8G8B8);

   if(FAILED(hr)){
      m_engine->dhERROR("FastFont::Init() - Error loading texture\n");
   }

   hr=init(p_metrics_name);

   if(FAILED(hr)){
      m_engine->dhERROR("FastFont::Init() - Error creating vertex buffer\n");
   }

   m_is_okay=true;
}

#else
// *************************************************************************************
// ************** Functions to handle lack of dhEngine*********************************
// *************************************************************************************

inline void dhFastFont::Init(IDirect3DDevice8 *p_device,const char *p_file_name,const char *p_metrics_name){
HRESULT hr;

   m_device=p_device;

   hr=D3DXCreateTextureFromFileEx(m_device, //Our D3D Device
                                  p_file_name,    //Filename of our texture
                                  D3DX_DEFAULT, //Width:D3DX_DEFAULT = Take from file 
                                  D3DX_DEFAULT, //Height:D3DX_DEFAULT = Take from file
                                  1,            //MipLevels
                                  0,            //Usage, Is this to be used as a Render Target? 0 == No
                                  D3DFMT_A8R8G8B8, //32-bit with Alpha, everything should support this
                                  D3DPOOL_MANAGED,//Pool, let D3D Manage our memory
                                  D3DX_DEFAULT, //Filter:Default filtering
                                  D3DX_DEFAULT, //MipFilter, used for filtering mipmaps
                                  0,            //Disable ColourKey
                                  NULL,         //SourceInfo, returns extra info if we want it (we don't)
                                  NULL,         //Palette:We're not using one
                                  &m_font_texture);  // Our texture goes here.

   if(FAILED(hr)){
      return;
   }

   hr=init(p_metrics_name);

   if(FAILED(hr)){
      return;
   }

   m_is_okay=true;
}

#endif  //#ifdef DHDHFF_USE_DHENGINE


#endif //#ifndef DHFASTFONT_H_









