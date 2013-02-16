#ifndef FONTINFOH
#define FONTINFOH

#include "VG/openvg.h"
#include "VG/vgu.h"

typedef struct {
	const short *CharacterMap;
	const int *GlyphAdvances;
	int Count;
	VGPath Glyphs[256];
} Fontinfo;

//Fontinfo SansTypeface, SerifTypeface, MonoTypeface;

#endif