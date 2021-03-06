/*
 *  FM-7 EMULATOR "XM7"  Copyright (C) 1999-2003
 * ＰＩ．(ytanaka@ipc-tokai.or.jp) Copyright (C) 2001-2003 Ryu
 * Takegami Copyright (C) 2004 GIMONS  [ XWIN コントロールバー
 * ]
 */

#ifndef _xw_bar_h_
#define _xw_bar_h_

#ifdef __cplusplus
extern          "C" {

#endif				/*  */

        /*
         * Global変数
         */
        extern char StatusFont[MAXPATHLEN];
	/*
	 *  主要エントリ
	 */
    void CreateStatus(AG_Widget *parent);
	/*
	 * ステータスバーの生成
	 */
        extern void DrawStatus(void);

	/*
	 * 描画
	 */
        extern void  PaintStatus(void);

	/*
	 * 再描画
	 */
        extern void DestroyStatus(void);

/*
* Widget Textures
*/
enum {
    ID_EMPTY = 0,
    ID_IN,
    ID_READ,
    ID_WRITE,
    ID_END
};
enum {
    ID_OFF = 0,
    ID_ON
};




#ifdef __cplusplus
}
#endif				/*  */

#endif	/* _xw_bar_h_ */
