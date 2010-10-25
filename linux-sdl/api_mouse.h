/*
 * api_mouse.h
 *
 *  Created on: 2010/09/20
 *      Author: whatisthis
 */

#ifndef API_MOUSE_H_
#define API_MOUSE_H_

#ifdef __cplusplus
extern "C"{
#endif

extern BYTE nMidBtnMode;
extern BOOL bMouseCaptureFlag;

void FASTCALL   PollMos(void);
void FASTCALL   SetMouseCapture(BOOL en);
gboolean OnButtonPress(GtkWidget * widget, GdkEventButton * event,
			       gpointer user_data);

gboolean OnButtonRelease(GtkWidget * widget, GdkEventButton * event,
				 gpointer user_data);
    extern BYTE     nMidBtnMode;/* 中央ボタン状態取得モード */

#ifdef __cplusplus
}
#endif

#endif /* API_MOUSE_H_ */
