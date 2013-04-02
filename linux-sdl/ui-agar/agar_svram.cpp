/*
 * XM7 : 断片化VRAMでの書き込み
 * 2011 (C) K.Ohta <whatithis.sowhat@gmail.com>
 */

#include <agar/core.h>
#include <agar/core/types.h>
#include <agar/gui.h>
//#include <agar/gui/opengl.h>

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glext.h>

#ifdef _OPENMP
#include <omp.h>
#endif // _OPENMP

#include "api_draw.h"
//#include "api_scaler.h"
#include "agar_xm7.h"
#include "agar_draw.h"
#include "agar_gldraw.h"
#include "sdl_cpuid.h"
#include "xm7.h"
#include "display.h"
#include "subctrl.h"
#include "device.h"

extern "C" {
extern XM7_SDLView *DrawArea;
extern BOOL bUseOpenCL;
extern BOOL bUseSIMD;
extern SDL_Surface *DrawSurface;
extern struct  XM7_CPUID *pCpuID;
}


#ifdef USE_OPENGL
extern AG_GLView *GLDrawArea;
#ifdef _USE_OPENCL
extern class GLCLDraw *cldraw;
#endif // _USE_OPENCL
#endif // USE_OPENGL

static void BuildVirtualVram8(Uint32 *pp, int x, int y, int  w, int h, int mode)
{
    int xx;
    int yy;
    int ww;
    int hh;
    Uint32 *p;

   if(pp == NULL) return;
   ww = (w + x) >> 3;
   hh = (h + y) >> 3;
  
    LockVram();
//    p = pp;
    if(pCpuID == NULL) return;
    if(SDLDrawFlag.DPaletteChanged) { // Palette changed
#ifdef _OPENMP
       #pragma omp parallel for shared(pp, SDLDrawFlag, hh, ww, mode) private(p, xx)
#endif
	for(yy = (y >> 3); yy < hh ; yy++) {
	   p = &pp[64 * ((x >> 3) + 80 * yy)];
	   for(xx = (x >> 3); xx < ww ; xx++) {
#if defined(USE_SSE2)
	      if(pCpuID->use_sse2) {
		 CreateVirtualVram8_1Pcs_SSE2(p, xx , yy << 3, sizeof(Uint32) * 8, mode);
	      } else {
		 CreateVirtualVram8_1Pcs(p, xx , yy << 3, sizeof(Uint32) * 8, mode);
	      }
#else
	      CreateVirtualVram8_1Pcs(p, xx , yy << 3, sizeof(Uint32) * 8, mode);
#endif
	      SDLDrawFlag.write[xx][yy] = TRUE;
	      SDLDrawFlag.read[xx][yy]  = FALSE;
	      p += 64;
	   }
	}
	SDLDrawFlag.Drawn = TRUE;
	SDLDrawFlag.DPaletteChanged = FALSE;
     } else { // Palette not changed
#ifdef _OPENMP
       #pragma omp parallel for shared(pp, SDLDrawFlag, hh, ww, mode) private(p, xx)
#endif
	for(yy = (y >> 3); yy < hh ; yy++) {
   	   p = &pp[64 * ((x >> 3) + 80 * yy)];
	   for(xx = (x >> 3); xx < ww ; xx++) {
	      if(SDLDrawFlag.read[xx][yy]) {
#if defined(USE_SSE2)
	      if(pCpuID->use_sse2) {
		 CreateVirtualVram8_1Pcs_SSE2(p, xx , yy << 3, sizeof(Uint32) * 8, mode);
	      } else {
		 CreateVirtualVram8_1Pcs(p, xx , yy << 3, sizeof(Uint32) * 8, mode);
	      }
#else
	      CreateVirtualVram8_1Pcs(p, xx , yy << 3, sizeof(Uint32) * 8, mode);
#endif
		 SDLDrawFlag.write[xx][yy] = TRUE;
		 SDLDrawFlag.read[xx][yy]  = FALSE;
		 SDLDrawFlag.Drawn = TRUE;
	      }
	      p += 64;
	   }
	}
     }
   
    UnlockVram();
}

static void BuildVirtualVram4096(Uint32 *pp, int x, int y ,int  w, int h, int mode)
{
    int xx;
    int yy;
    int ww;
    int hh;
    Uint32 *p;

   if(pp == NULL) return;
   ww = (w + x) >> 3;
   hh = (h + y) >> 3;

   LockVram();
//    p = pp;
    if(SDLDrawFlag.APaletteChanged) { // Palette changed
#ifdef _OPENMP
       #pragma omp parallel for shared(pp, SDLDrawFlag, hh, ww, mode) private(p, xx)
#endif
       for(yy = (y >> 3); yy < hh ; yy++) {
	  p = &pp[64 * ((x >> 3) + 40 * yy)];
	  for(xx = (x >> 3); xx < ww ; xx++) {
#if defined(USE_SSE2)
	      if(pCpuID->use_sse2) {
		 CreateVirtualVram4096_1Pcs_SSE2(p, xx , yy << 3, sizeof(Uint32) * 8, mode);
	      } else {
		 CreateVirtualVram4096_1Pcs(p, xx , yy << 3, sizeof(Uint32) * 8, mode);
	      }
#else
	     CreateVirtualVram4096_1Pcs(p, xx, yy << 3, 8 * sizeof(Uint32), mode);
#endif
	     SDLDrawFlag.write[xx][yy] = TRUE;
	     SDLDrawFlag.read[xx][yy] = FALSE;
	     p += 64;
	  }
       }
       SDLDrawFlag.Drawn = TRUE;
       SDLDrawFlag.APaletteChanged = FALSE;
    } else {
	// Palette not changed   
#ifdef _OPENMP
       #pragma omp parallel for shared(pp, SDLDrawFlag, hh, ww, mode) private(p, xx)
#endif
       for(yy = (y >> 3); yy < hh ; yy++) {
	  p = &pp[64 * ((x >> 3) + 40 * yy)];
	  for(xx = (x >> 3); xx < ww ; xx++) {
	     if(SDLDrawFlag.read[xx][yy]) {
                CreateVirtualVram4096_1Pcs(p, xx, yy << 3, 8 * sizeof(Uint32), mode);
                SDLDrawFlag.write[xx][yy] = TRUE;
                SDLDrawFlag.read[xx][yy] = FALSE;
		SDLDrawFlag.Drawn = TRUE;
	     }
	     p += 64;
	  }
       }
    }
   
    UnlockVram();
}

static void BuildVirtualVram256k(Uint32 *pp, int x, int y, int  w, int h, int mpage)
{
    int xx;
    int yy;
    int ww;
    int hh;
    Uint32 *p;

   if(pp == NULL) return;
   ww = (w + x) >> 3;
   hh = (h + y) >> 3;
//   LockVram();
//    p = pp;
#ifdef _OPENMP
       #pragma omp parallel for shared(pp, SDLDrawFlag, hh, ww) private(p, xx)
#endif
    for(yy = (y >> 3); yy < hh ; yy++) {
        for(xx = (x >> 3); xx < ww ; xx++) {
            if(SDLDrawFlag.read[xx][yy]) {
                p = &pp[64 * (xx + 40 * yy)];
#if defined(USE_SSE2)
	      if(pCpuID->use_sse2) {
		 CreateVirtualVram256k_1Pcs_SSE2(p, xx , yy << 3, sizeof(Uint32) * 8, mpage);
	      } else {
		 CreateVirtualVram256k_1Pcs(p, xx , yy << 3, sizeof(Uint32) * 8, mpage);
	      }
#else
	     CreateVirtualVram256k_1Pcs(p, xx, yy << 3, 8 * sizeof(Uint32), mpage);
#endif
                SDLDrawFlag.read[xx][yy] = FALSE;
                SDLDrawFlag.write[xx][yy] = TRUE;
                SDLDrawFlag.Drawn = TRUE;
            }
        }
    }
//    UnlockVram();
}

void PutVram_AG_SP(SDL_Surface *p, int x, int y, int w, int h,  Uint32 mpage)
{
	int xx, yy;
	int hh, ww;
	int addr;
	int ofset;
	int size;
	Uint32 *pp;

	// Test

    if((vram_pb == NULL) || (vram_pg == NULL) || (vram_pr == NULL)) return;

    LockVram();
#ifdef _USE_OPENCL
   if((cldraw != NULL) && (GLDrawArea != NULL)){ // Snip builing-viryual-vram if GLCL mode.
//        bVramUpdateFlag = TRUE;
        SDLDrawFlag.Drawn = TRUE;
        UnlockVram();
        return;
   }
#endif   
   if(pVirtualVram == NULL) return;
   pp = &(pVirtualVram->pVram[0][0]);

   if(pp == NULL) {
      UnlockVram();
      return;
   }
   
   if((bClearFlag)) {
      memset(pp, 0x00, 640 * 400 * sizeof(Uint32)); // モードが変更されてるので仮想VRAMクリア
      SetDrawFlag(TRUE);
      bClearFlag = FALSE;
   }
   
 
     switch (bMode) {
      case SCR_400LINE:
        BuildVirtualVram8(pp, x, y, w, h, bMode);
		break;
	case SCR_262144:
        BuildVirtualVram256k(pp, x, y, w, h, mpage);
		break;
	case SCR_4096:
        BuildVirtualVram4096(pp, x, y, w, h, mpage);
		break;
	case SCR_200LINE:
        BuildVirtualVram8(pp, x, y, w, h, bMode);
		break;
	}

   UnlockVram();
}
