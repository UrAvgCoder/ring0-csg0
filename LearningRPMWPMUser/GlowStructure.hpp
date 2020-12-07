#pragma once
#include "definitions.hpp"

typedef struct _GLOW_STRUCT_{
	BYTE base[4];
	float red, green, blue, alpha;
	BYTE buff[16];
	bool renderwhenocculed, renderwhenunocculed, fullbloom;
	BYTE buff2[5];
	int glowStyle;
} SGlowStruct, * pGlowStruct;
