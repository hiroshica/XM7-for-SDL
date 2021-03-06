/*
 *      FM-7 EMULATOR "XM7"
 *
 *      Copyright (C) 1999-2010 ＰＩ．(yasushi@tanaka.net)
 *      Copyright (C) 2001-2010 Ryu Takegami
 *
 *      [ ビルドコンフィグ ]
 */

/*
 * Toolkit
 */
//#define USE_GTK

/*
 * Target
 */
//#define _XWIN

/*
 *
 */
#ifndef FONTPATH
#define FONTPATH "/usr/local/share/xm7:/usr/share/fonts/X11/misc:/usr/share/fonts/X11/75dpi:/usr/share/fonts/truetype/ipafont:/usr/share/fonts/truetype/takao-gothic:/usr/share/fonts/truetype/mona:/usr/share/fonts/truetype/misaki:.:./.xm7::./.xm7/fonts"
#endif

#ifndef FUNC_FONT
#define FUNC_FONT "TakaoPGothic.ttf"
#endif

#ifndef STAT_FONT
#define STAT_FONT "TakaoPGothic.ttf"
#endif

#ifndef CMT_FONT
#define CMT_FONT "TakaoPGothic.ttf"
#endif

#ifndef VFD_FONT
#define VFD_FONT "TakaoPGothic.ttf"
#endif

#ifndef UI_FONT
#define UI_FONT "TakaoPGothic.ttf"
#endif

#ifndef STAT_PT
#define STAT_PT 10
#endif

#ifndef UI_PT
#define UI_PT  14
#endif

/*
 * GUIレイアウト
 */
#define OSD_HEIGHT 24
#define DRAW_OFSET 64

/*
 * Embedの場合にはサイズと表示レイアウトを変更する必要がある？
 */
#define LED_WIDTH 27
#define LED_HEIGHT 20

#define VFD_WIDTH 100
#define VFD_HEIGHT 20

#define CMT_WIDTH 40
#define CMT_HEIGHT 20

#define STAT_WIDTH 300
#define STAT_HEIGHT 15

#ifndef SHAREDIR
#define SHAREDIR "/usr/local/share/xm7/"
#endif

#ifndef UIDIR
#define UIDIR SHAREDIR
#endif

#ifndef RSSDIR
#define RSSDIR SHAREDIR
#endif

#ifndef FUNC_FONT
#define FUNC_FONT UIFONT
#endif

#ifndef STAT_FONT
#define STAT_FONT UIFONT
#endif

/*
 * エミュレーション設定
 */
#ifndef XM7_VER
#define XM7_VER 3
#endif

#undef CPU_ASM
//#define MOUSE
//#define MR2
//#define FDDSND


/*
 * デバッグ関連
 */
//#define MEMWATCH
//#undef NODEBUG
