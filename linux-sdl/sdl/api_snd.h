/*
 *  FM-7 EMULATOR "XM7"  Copyright (C) 1999-2003
 * ＰＩ．(ytanaka@ipc-tokai.or.jp) Copyright (C) 2001-2003 Ryu
 * Takegami Copyright (C) 2004 GIMONS  [ XWIN サウンド ] 
 */  
    


#ifndef _api_snd_h_
#define _api_snd_h_
#include "xm7.h"
    
/*
 *  定数定義 
 */ 
#define SNDBUF	20	      /* WAV最大同時発音数 */
#define XM7_PCM_MAX_VOLUME 0x3fff

#define CHSEPARATION_DEFAULT    7       /* チャンネルセパレーションデフォルト値 */
#define FMVOLUME_DEFAULT                0       /* FM音源ボリュームデフォルト値 */
#define PSGVOLUME_DEFAULT               -2      /* PSGボリュームデフォルト値 */
#define BEEPVOLUME_DEFAULT              -24     /* BEEP音ボリュームデフォルト値 */
#define CMTVOLUME_DEFAULT               -24     /* CMT音モニタボリュームデフォルト値 */
#define WAVEVOLUME_DEFAULT              -6      /* 各種効果音ボリュームデフォルト値 */

    
#ifdef __cplusplus
#include "SndDrvIF.h"
#include "SndDrvBeep.h"
#include "SndDrvWav.h"
#include "SndDrvOpn.h"
#include "SndDrvCMT.h"

#endif

#ifdef __cplusplus
extern          "C" {
    
#endif				/*  */
/*
 *  主要エントリ 
 */ 
extern void        InitSnd(void);
                
/*
 * 初期化 
 */            
extern void        CleanSnd(void);
                   
/*
 * クリーンアップ 
 */            
extern BOOL        SelectSnd(void);
               
/*
 * セレクト 
 */            
extern void        ApplySnd(void);
                   
/*
 * 適用 
 */            
extern void        PlaySnd(void);
          
extern void         SetSoundVolume(void);

extern void  SetSoundVolume2(UINT uSp, int nFM, int nPSG,
                             int nBeep, int nCMT, int nWav);

/*
 * 演奏開始 
 */            
void        StopSnd(void);
                   
/*
 * 演奏停止 
 */            
void        ProcessSnd(BOOL bZero);
                   
/*
 * バッファ充填定時処理 
 */            
void        OpenCaptureSnd(char *fname);
                   
/*
 * WAVキャプチャ開始 
 */            
void        CloseCaptureSnd(void);
                   
/*
 * WAVキャプチャ終了 
 */            
#ifdef FDDSND
void        InitFDDSnd(void);
                   
/*
 * 初期化 
 */            
void        CleanFDDSnd(void);
                   
/*
 * クリーンアップ 
 */            
#endif				/*  */
                   
/*
 *  主要ワーク 
 */            
/*
 * サンプルレート(Hz、0で無し) 
 */            
extern UINT     nSampleRate;
             
/*
 * サウンドバッファ(ダブル、ms) 
 */            
extern UINT     nSoundBuffer;
/*
 * 出力モード
 */
extern UINT    nStereoOut;

/*
 * FM高品質合成モード 
 */            
extern BOOL     bFMHQmode;                  

/*
 * BEEP周波数(Hz) 
 */            
extern UINT     nBeepFreq;
                   
/*
 * WAVキャプチャファイルハンドル 
 */            
extern int      hWavCapture;
                   
/*
 * WAVキャプチャ開始後 
 */            
extern BOOL     bWavCapture;
                   
                   
/*
 * 強制ステレオ出力 
 */            
extern BOOL     bForceStereo;
                   
/*
 * テープ音モニタ 
 */            
extern BOOL     bTapeMon;
                   
/*
 * クリッピングカウンタ 
 */            
extern UINT     uClipCount;


               
/*
 * 音楽演奏用データパッケージ 
 */            
                   
extern int             nFMVolume;      /* FM音源ボリューム */
extern int             nPSGVolume;     /* PSGボリューム */
extern int             nBeepVolume;    /* BEEP音ボリューム */
extern int             nCMTVolume;     /* CMT音モニタボリューム */
extern int             nWaveVolume;    /* 各種効果音ボリューム */
extern int             iTotalVolume;   /* 全体ボリューム */
extern UINT            uChSeparation;  /* チャンネルセパレーション */

                   
/*
 * 定数など
 */
#define CHUNKS 2
#define WAV_CHANNELS 5
enum {
	CH_SND_BEEP = 0,
	CH_SND_CMT = 2,
	CH_SND_OPN = 4,
	CH_WAV_RELAY_ON = 6,
	CH_WAV_RELAY_OFF,
	CH_WAV_FDDSEEK,
	CH_WAV_RESERVE1,
	CH_WAV_RESERVE2,
	CH_CHANNELS
};
enum {
	GROUP_SND_BEEP = 0,
	GROUP_SND_CMT ,
	GROUP_SND_OPN ,
	GROUP_SND_SFX
};

struct SndBufType {
	Sint32 *pBuf32;
	Sint16 *pBuf;
	Uint32 nSize;
	Uint32 nReadPTR;
	Uint32 nWritePTR;
	int nHeadChunk;
	int nLastChunk;
	DWORD nLastTime;
	int nChunks;
	int nChunkNo;
	Mix_Chunk **mChunk; /* Chunkの配列へのポインタ */
} ;

     
                  
//#define SND_FIFO_SIZE (96000 * sizeof(WORD) * 2 * 1000)/1000
#ifdef __cplusplus
}              
#endif				/*  */

#ifdef __cplusplus
/* snd_buffer.cpp */
extern struct SndBufType *InitBufferDesc(void);
extern void DetachBufferDesc(struct SndBufType *p);
extern void SetupBuffer(struct SndBufType *p, int members, BOOL flag16, BOOL flag32);
extern void DetachBuffer(struct SndBufType *p);
extern int CopyChunk(struct SndBufType *p, Sint16 *buf, int offset);

extern int SndCalcSamples(struct SndBufType *p, DWORD ttime);
extern DWORD RenderSub(struct SndBufType *p, SndDrvIF *drv, DWORD ttime, int samples, BOOL bZero);
extern BOOL FlushOpnSub(struct SndBufType *p, SndDrvIF *drv, DWORD ttime,  BOOL bZero, int maxchunk);
extern BOOL FlushBeepSub(struct SndBufType *p, SndDrvIF *drv, DWORD ttime,  BOOL bZero, int maxchunk);
extern BOOL FlushCMTSub(struct SndBufType *p, SndDrvIF *drv, DWORD ttime,  BOOL bZero, int maxchunk);
#endif

#endif	/* _api_snd_h_ */