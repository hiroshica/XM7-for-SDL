/*
 * agar_draw.cpp
 *
 *  Created on: 2010/11/09
 *      Author: whatisthis
 */

#include <agar/core/types.h>
#include <agar/core.h>
#include <agar/gui.h>
#include <agar/gui/opengl.h>
#include <libemugrph/EmuAgarGL.h>
#include "api_draw.h"
#include "api_scaler.h"
#include "agar_gldraw.h"
#include <SDL.h>

extern BYTE bMode;
extern "C" {
extern AG_GLView *OsdArea;
extern AG_Window *MainWindow;
}
Uint32 nDrawTick1E;
static BYTE oldBMode;


void InitGUI(int w, int h)
{
}

void ResizeWindow_Agar(int w, int h)
{
	int hh;
	int ww;
//	int sh;
//	int sw;
//	sh = RootVideoHeight-(MainWindow->tPad + MainWindow->bPad);
//	sw = RootVideoWidth-(MainWindow->rPad + MainWindow->lPad);
	if(DrawArea == NULL) return;
//	if(w > sw) w = sw;
//	if(h > sh) h = sh;
#if 0
	switch(nAspect) {
	case nAspect11:
		if(w>h) {
			h = (w * 5) / 8;
		} else {
			w = (h * 8) / 5;
		}
		break;
	case nAspect43:
		if(w>h) {
			h = (w * 6) / 8;
		} else {
			w = (h * 8) / 6;
		}
		break;
	}
#endif
//	if(w > sw) w = sw;
//	if(h > sh) h = sh;

	AG_WidgetSetSize(AGWIDGET(DrawArea), w, h);
	AG_GLViewSizeHint(DrawArea, w, h);
//	AG_WidgetSetPosition(AGWIDGET(DrawArea), 0, 0);
	AG_WidgetSetPosition(AGWIDGET(DrawArea), 0, MenuBar->wid.h);
	nDrawWidth = w;
	nDrawHeight = h;

	if(OsdArea != NULL){
		ww = w>OsdArea->wid.w?w:OsdArea->wid.w;
		hh = h;
	} else{
		ww = w;
		hh = h;
	}
	if(MenuBar != NULL) {
		hh += MenuBar->wid.h;
//		ww = ww>MenuBar->wid.w?ww:MenuBar->wid.w;
	}
//	sh = RootVideoHeight;
//	sw = RootVideoWidth;
//	if(ww > sw) ww = sw;
//	if(hh > sh) hh = sh;
	AG_ResizeDisplay(ww, hh);
        printf("Resize to %d x %d\n", ww, hh);
	if(MainWindow) {
		AG_WindowSetGeometry(MainWindow, 0, MenuBar->wid.h, nDrawWidth, nDrawHeight);
	}
	AG_WidgetSetSize(AGWIDGET(MenuBar), ww, MenuBar->wid.h);
        AG_WidgetEnable(AGWIDGET(MenuBar));
        AG_Redraw(AGWIDGET(MainWindow));
        AG_Redraw(AGWIDGET(MenuBar));
	AG_WidgetFocus(AGWIDGET(MenuBar));
	if(MenuBar) {
		AG_MenuSetPadding(MenuBar, 0 , 0, 0, 0);
	}
}

void ResizeWindow_Agar2(int w, int h)
{
	int hh;
	int ww;
//	int sh = nDrawHeight;
//	int sw = nDrawWidth;
//	sh = RootVideoHeight-(MainWindow->tPad + MainWindow->bPad);
//	sw = RootVideoWidth-(MainWindow->rPad + MainWindow->lPad);
//	AG_ResizeDisplay(w, h);
	if(DrawArea == NULL) return;
//	if(w > sw) w = sw;
//	if(h > sh) h = sh;
#if 0
	switch(nAspect) {
	case nAspect11:
		if(w>h) {
			h = (w * 5) / 8;
		} else {
			w = (h * 8) / 5;
		}
		break;
	case nAspect43:
		if(w>h) {
			h = (w * 6) / 8;
		} else {
			w = (h * 8) / 6;
		}
		break;
	}
#endif

	AG_WidgetSetSize(AGWIDGET(DrawArea), w, h);
	AG_WidgetSetPosition(AGWIDGET(DrawArea), 0, MenuBar->wid.h);
	AG_GLViewSizeHint(DrawArea, w, h);
//	AG_ResizeDisplay(w, h + MenuBar->wid.h);
   	AG_ResizeDisplay(w, h);
         printf("Resize to %d x %d\n", w, h + MenuBar->wid.h);
 	AG_WidgetSetSize(AGWIDGET(MenuBar), w, MenuBar->wid.h);
        AG_WidgetEnable(AGWIDGET(MenuBar));
        AG_Redraw(AGWIDGET(MainWindow));
        AG_Redraw(AGWIDGET(MenuBar));
        nDrawWidth = w;
	nDrawHeight = h;
//	if(MainWindow) {
//		AG_WindowSetGeometry(MainWindow, 0, MenuBar->wid.h , nDrawWidth, nDrawHeight);
//	}
}


void AGDrawTaskMain(void)
{
	Uint32 nDrawTick2E;
	Uint32 fps;

	if(nEmuFPS > 2) {
		fps = 1000 / nEmuFPS;
	} else {
		fps = 500;
	}
	nDrawTick2E = AG_GetTicks();
	if(nDrawTick1E > nDrawTick2E) {
		nDrawTick1E = 0;
	}
	if(((nDrawTick2E - nDrawTick1E)<fps) && (bMode == oldBMode)) return;
	nDrawTick1E = nDrawTick2E;
	oldBMode = bMode;
#if XM7_VER >= 3
	switch (bMode) {
	case SCR_400LINE:
		Draw400l();
		break;
	case SCR_262144:
		Draw256k();
		break;
	case SCR_4096:
		Draw320();
		break;
	case SCR_200LINE:
		Draw640All();
		break;
	}
#else				/*  */
	/*
	 * どちらかを使って描画
	 */
	if (bAnalog) {
		Draw320All();
	}
	else {
		Draw640All();
	}
#endif				/*  */
	SelectDraw2();
		/* Render the Agar windows */
}

