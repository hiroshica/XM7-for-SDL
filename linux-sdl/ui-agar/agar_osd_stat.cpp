/*
 * agar_osd_stat.cpp
 * Created on: 2012/06/28
 *      Author: K.Ohta <whatisthis.sowhat@gmail.com>
 *     History: June,28,2012 Split from agar_osd.cpp
 */
#include <SDL/SDL.h>
#include <agar/core.h>
#include <agar/core/types.h>
#include <agar/gui.h>
#include <iconv.h>

#include "xm7.h"
#include "keyboard.h"
#include "tapelp.h"
#include "display.h"
#include "ttlpalet.h"
#include "apalet.h"
#include "subctrl.h"
#include "fdc.h"
#include "mouse.h"

#ifdef USE_AGAR
#include "agar_xm7.h"
#else
#include "xm7_sdl.h"
#endif

#include "agar_osd.h"
#include "sdl_sch.h"
#include "api_draw.h"
#include "api_mouse.h"

#include "agar_draw.h"
#ifdef USE_OPENGL
#include "agar_gldraw.h"
#endif
#include "agar_sdlview.h"

#define OSD_STRLEN 256
struct OsdStatPack
{
        int Changed;
        BOOL init;
        char message[OSD_STRLEN + 1];
        AG_Surface *surface;
        AG_Mutex mutex;
        int width;
        int height;
        BOOL mouse_capture;
};
static struct OsdStatPack *pOsdStat;
static struct XM7_SDLView *pwSTAT;

extern int getnFontSize(void);
extern void SetPixelFormat(AG_PixelFormat *fmt);
extern AG_Font *pStatusFont;

struct OsdStatPack *InitStat(int w,int h)
{
    struct OsdStatPack *p;
    AG_PixelFormat fmt;
    AG_Color black;
    int size = getnFontSize();

    SetPixelFormat(&fmt);
    p = (struct OsdStatPack *)malloc(sizeof(struct OsdStatPack));
    
    if(p == NULL) return NULL;
    black.r = 0;
    black.g = 0;
    black.b = 0;
    black.a = 255;
   
    memset(p, 0x00, sizeof(struct OsdStatPack));
    p->init = TRUE;
    p->Changed = FALSE;
    p->message[0] = '\0';
    AG_MutexInit(&(p->mutex));
    p->width = w;
    p->height = h;
    SetPixelFormat(&fmt);
    p->surface = AG_SurfaceNew(AG_SURFACE_PACKED , w,  h, &fmt, AG_SRCALPHA);
    AG_FillRect(p->surface, NULL, black); 
    p->mouse_capture = FALSE;
    return p;
}

void DeleteStat(struct OsdStatPack *p)
{
   if(p == NULL) return;
   AG_MutexDestroy(&(p->mutex));
   free(p);
}

extern "C" {
static void DrawStatFn(AG_Event *event)
{
   XM7_SDLView *my = (XM7_SDLView *)AG_SELF();
   struct OsdStatPack *disp = (struct OsdStatPack *)AG_PTR(1);
   AG_Surface *dst;
   AG_Color black, n;
   AG_Rect rect;
   char str_t[OSD_STRLEN + 1];
   int size;
   int len;

   if((disp == NULL)  || (my == NULL)) return;

   str_t[0] = '\0';
   dst = disp->surface;
   if(dst == NULL) return;
    n.r = 255;
    n.g = 255;
    n.b = 255;
    n.a = 255;

    black.r = 0;
    black.g = 0;
    black.b = 0;
    black.a = 255;

   AG_MutexLock(&(disp->mutex));
   if((disp->Changed == FALSE) && (disp->init == FALSE) && (disp->mouse_capture == bMouseCaptureFlag)) {
       AG_MutexUnlock(&(disp->mutex));
       XM7_SDLViewSetDirty(my);
       return;
   }
   rect.w = dst->w;
   rect.h = dst->h;
   rect.x = 0;
   rect.y = 0;
   //AG_FillRect(dst, NULL, black);
   size = getnFontSize();
   len = strlen(disp->message);
   
   strncpy(str_t, disp->message, OSD_STRLEN);
   if((pStatusFont != NULL) && (size > 2) && (len > 0)){
      AG_Surface *tmps;
      AG_Font *font;

      AG_FillRect(dst, NULL, black);
      AG_PushTextState();
      AG_TextFont(pStatusFont);
      font = AG_TextFontPts(size);
      AG_TextColor(n);
      AG_TextBGColor(black);
      tmps = AG_TextRender(str_t);
      AG_PopTextState();
      AG_SurfaceBlit(tmps, NULL, dst, 0, 0);
      AG_SurfaceFree(tmps);
      XM7_SDLViewLinkSurface(my, dst);
      XM7_SDLViewSetDirty(my);
   }
//   XM7_SDLViewLinkSurface(my, dst);
//   XM7_SDLViewSetDirty(my);
   disp->init = FALSE;
   disp->Changed = FALSE;
   disp->mouse_capture = bMouseCaptureFlag;
   AG_MutexUnlock(&(disp->mutex));
}
}

static void CreateStat(AG_Widget *parent, struct OsdStatPack *p)
{

  if(p == NULL) return;
  if(parent == NULL) return;
  pwSTAT = XM7_SDLViewNew(parent, NULL, NULL);
  if(pwSTAT == NULL) return;
  XM7_SDLViewSurfaceNew(pwSTAT, STAT_WIDTH , STAT_HEIGHT);
  AG_WidgetSetSize(pwSTAT, STAT_WIDTH , STAT_HEIGHT);
  XM7_SDLViewDrawFn(pwSTAT, DrawStatFn, "%p", p);
  AG_WidgetShow(pwSTAT);
}

void InitStatOSD(AG_Widget *parent)
{
   AG_Box *box;
   if(parent == NULL) return;
   pOsdStat = NULL;
   pOsdStat = InitStat(STAT_WIDTH, STAT_HEIGHT);
   if(pOsdStat == NULL) return;
   box = AG_BoxNew(parent, AG_BOX_HORIZ, 0);
   CreateStat(AGWIDGET(box), pOsdStat);
}

void DestroyStatOSD(void)
{
   if(pOsdStat != NULL) {
     DeleteStat(pOsdStat);
     pOsdStat = NULL;
   }
}

void LinkSurfaceSTAT(void)
{
}

void ResizeStatOSD(AG_Widget *parent, int w, int h)
{
  int total =  STAT_WIDTH + VFD_WIDTH * 2
          + CMT_WIDTH + LED_WIDTH * 3;
  float ww = (float)w;
  float wSTAT = (float)STAT_WIDTH / (float)total;
  AG_Color black;

  if(pwSTAT == NULL) return;
  if(pOsdStat == NULL) return;
  AG_MutexLock(&(pOsdStat->mutex));
  pOsdStat->width = (int)(ww / 640.0f * (float)STAT_WIDTH);
  pOsdStat->height =  (int)(ww / 640.0f * (float)STAT_HEIGHT);
  pOsdStat->init = TRUE;
  AG_WidgetSetSize(pwSTAT, pOsdStat->width, pOsdStat->height);
  AG_SurfaceResize(pOsdStat->surface, pOsdStat->width, pOsdStat->height);
  black.r = 0;
  black.g = 0;
  black.b = 0;
  black.a = 255;
  AG_FillRect(pOsdStat->surface, NULL, black); 

  AG_MutexUnlock(&(pOsdStat->mutex));
  {
     AG_SizeAlloc a;
     a.w = pOsdStat->width;
     a.h = pOsdStat->height;
     a.x = 0;
     a.y = 0;
     AG_WidgetSizeAlloc(pwSTAT, &a);
  }
  pOsdStat->Changed = TRUE;

}

void ClearStatOSD(void)
{
   if(pOsdStat == NULL) return;
   pOsdStat->message[0] = '\0';
   pOsdStat->Changed = TRUE;
}

void DrawMainCaption(BOOL redraw)
{
   char string[1024];
   char tmp[128];
   char *p;

   string[0] = '\0';
   if(pOsdStat == NULL) return;
   memset(string, 0x00, sizeof(string));
   /*
    * RUN MODE
    */
   if (run_flag) {
       strcpy(string, "[RUN ]");
   } else {
       strcpy(string, "[STOP]");
   }
   /*
    * BOOT MODE
    */
   if(boot_mode == BOOT_BASIC){
      strcat(string, "[BAS]");
   } else if(boot_mode == BOOT_DOS) {
        strcat(string, "[DOS]");
    } else {
        strcat(string, "[???]");
    }
    if(bMouseCaptureFlag == FALSE) {
       strcat(string, "[ ]");
    } else {
       strcat(string, "[M]");
    }
    strcat(string, " ");

	/*
	 * CPU速度比率
	 */
	if (bAutoSpeedAdjust) {
		sprintf(tmp, "(%3d%%) ", speed_ratio / 100);
		strcat(string, tmp);
	}

	/*
	 * フロッピーディスクドライブ 0
	 */
	if (fdc_ready[0] != FDC_TYPE_NOTREADY) {

		/*
		 * ファイルネーム＋拡張子のみ取り出す
		 */
#ifdef _WINDOWS
		p = strrchr(fdc_fname[0], '\\');
#else
		p = strrchr(fdc_fname[0], '/');
#endif
		if (p == NULL) {
			p = fdc_fname[0];
		} else {
			p++;
		}
		sprintf(tmp, "- %s ", p);
		strcat(string, tmp);
	}

	/*
	 * フロッピーディスクドライブ 1
	 */
	if (fdc_ready[1] != FDC_TYPE_NOTREADY) {
		if ((strcmp(fdc_fname[0], fdc_fname[1]) != 0) ||
				(fdc_ready[0] == FDC_TYPE_NOTREADY)) {
			/*
			 * ファイルネーム＋拡張子のみ取り出す
			 */
#ifdef _WINDOWS
		p = strrchr(fdc_fname[1], '\\');
#else
		p = strrchr(fdc_fname[1], '/');
#endif
			if (p == NULL) {
				p = fdc_fname[1];
			} else {
				p++;
			}
			sprintf(tmp, "(%s) ", p);
			strcat(string, tmp);
		}
	}

	/*
	 * テープ
	 */
	if (tape_fileh != NULL) {

		/*
		 * ファイルネーム＋拡張子のみ取り出す
		 */
#ifdef _WINDOWS
		p = strrchr(tape_fname, '\\');
#else
		p = strrchr(tape_fname, '/');
#endif
		if (p == NULL) {
			p = tape_fname;
		} else {
			p++;
		}
		sprintf(tmp, "- %s ", p);
		strcat(string, tmp);
	}
    /*
     * Compare
     */
    if((strncmp(string, pOsdStat->message, OSD_STRLEN - 1) != 0) ||
       (redraw == TRUE)){
            AG_MutexLock(&(pOsdStat->mutex));
            pOsdStat->Changed = TRUE;
            memset(pOsdStat->message, 0x00, OSD_STRLEN);
            strncpy(pOsdStat->message, string, OSD_STRLEN - 1);
            AG_MutexUnlock(&(pOsdStat->mutex));
    }

}

