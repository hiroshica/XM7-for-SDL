/*
 * EmuGrphScale1x2.h
 *
 *  Created on: 2010/10/25
 *      Author: whatisthis
 */

#ifndef EMUGRPHSCALE1X2_H_
#define EMUGRPHSCALE1X2_H_
#include <SDL/SDL.h>

class EmuGrphScale1x2 {
public:
	EmuGrphScale1x2();
	virtual ~EmuGrphScale1x2();
	void SetVramReader(void f(Uint32, Uint32 *, Uint32));
	void SetConvWord(void f(SDL_Surface *, Uint32 *, Uint32 *));
	void SetPutWord(void f(Uint32 *, Uint32, Uint32 *));
	void PutVram(SDL_Surface *p, int x, int y, int w, int h, Uint32 mpage);
protected:
	void (*getvram)(Uint32, Uint32 *, Uint32);
	void (*putword)(Uint32 *, Uint32 , Uint32 *);
	void (*convword)(SDL_Surface *, Uint32 *, Uint32 *);

};

#endif /* EMUGRPHSCALE1X2_H_ */
