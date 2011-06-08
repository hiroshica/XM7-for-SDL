/*
 * api_js.h
 *
 *  Created on: 2010/09/17
 *      Author: whatisthis
 */

#ifndef API_JS_H_
#define API_JS_H_

#include "xm7.h"
#include "api_kbd.h"

#ifdef __cplusplus
extern "C" {
#endif

extern int            nJoyType[2];	/* ジョイスティックタイプ */
extern int            nJoyRapid[2][2];	/* 連射タイプ */
extern int            nJoyCode[2][7];	/* 生成コード */
extern BYTE FASTCALL joy_request(BYTE no);
extern BOOL FASTCALL InitJoy(void);
extern void FASTCALL PollJoy(void);
extern void FASTCALL CleanJoy(void);
extern BOOL OnMoveJoy(SDL_Event * event);
extern BOOL OnPressJoy(SDL_Event * event);
extern BOOL OnReleaseJoy(SDL_Event * event);
#ifdef __cplusplus
}
#endif

#endif /* API_JS_H_ */