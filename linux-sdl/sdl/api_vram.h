/*
 * XM7: Vram Emulation Routines
 *  (C) 2011 K.Ohta <whatisthis.sowhat@gmail.com>
 */
#ifndef API_VRAM_H_INCLUDED
#define API_VRAM_H_INCLUDED

#include <SDL/SDL.h>
#include <GL/gl.h>
#include <GL/glext.h>
#include "api_draw.h"
//#include "api_scaler.h"

#include "xm7.h"
#include "display.h"
#include "subctrl.h"
#include "device.h"

#include "agar_draw.h"
#include "agar_gldraw.h"
#include "agar_xm7.h"
#include "xm7_types.h"

#ifdef __cplusplus
extern "C" {
#endif
/*
 * api_vram8.c
*/
extern void SetVram_200l(Uint8 *p);
extern void SetVram_400l(Uint8 *p);
extern void CalcPalette_8colors(Uint32 index, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
extern void CreateVirtualVram8_1Pcs(Uint32 *p, int x, int y, int pitch, int mpage);
/*
 * api_vram4096.c
 */
extern void CalcPalette_4096Colors(Uint32 index, Uint8 r, Uint8 g, Uint8 b, Uint8 a);
extern void CreateVirtualVram4096_1Pcs(Uint32 *p, int x, int y, int pitch, int mpage);

/*
 * api_vram256k.c
 */
extern void CreateVirtualVram256k_1Pcs(Uint32 *p, int x, int y, int pitch, int mpage);

#if (__GNUC__ >= 4)
extern void initvramtbl_8_vec(void);
extern void initvramtbl_4096_vec(void);
extern void detachvramtbl_8_vec(void);
extern void detachvramtbl_4096_vec(void);
extern v4hi lshift_6bit8v(v4hi *v);
extern v4si *aPlanes;

enum {
   B0 = 0,
   B1 = 256,
   B2 = 512,
   B3 = 768,
   R0 = 1024,
   R1 = 1280,
   R2 = 1536,
   R3 = 1792,
   G0 = 2048,
   G1 = 2304,
   G2 = 2560,
   G3 = 2816
};

   
enum {
   PLAINB0 = 0,
   PLAINB1,
   PLAINB2,
   PLAINB3,
   PLAINR0,
   PLAINR1,
   PLAINR2,
   PLAINR3,
   PLAING0,
   PLAING1,
   PLAING2,
   PLAING3
};

enum {
   PLAINB = 0,
   PLAINR,
   PLAING,
   PLAINW
};

#endif    // __GNUC__ >= 4
extern Uint8 *vram_pb;
extern Uint8 *vram_pr;
extern Uint8 *vram_pg;



#ifdef __cplusplus
}
#endif


#endif // API_VRAM_H_INCLUDED
