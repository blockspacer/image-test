#pragma once

#include "GlContext.h"

#include <iostream>
using std::cout;
using std::endl;

#include <unordered_map>
using std::unordered_map;

#ifdef NATIVE
	#include "SkImageInfo.h"
	#include "SkSurface.h"

	#include "SkData.h"

	#include "SkImage.h"

	#include "SkImageInfo.h"
	#include "SkStream.h"
	#include "SkSurface.h"
	#include "SkCanvas.h"
	#include "SkGraphics.h"
	#include "SkColor.h"
#endif

#include "tinyutf8.h"

struct SpritePosition {
	float top {-1}, left {-1}, layer {-1},
		width {-1}, height {-1};
};

enum FontFacee {normal, bold, italic, bolditalic, monospaced, fontFaceCount};

using FontFaceVariants = SpritePosition[5];

struct FontFace {SpritePosition
 normal ,
 bold ,
 italic ,
 bolditalic ,
 monospaced ,
 fontFaceCount ;
};

struct WordSprites {
	size_t smallestSize   {0};
	vector<FontFace> textSizes;
};

struct MultipartWord {
	vector <float> segments; // segment lengths given as multiples of the pixel height of the text
};

using Token = size_t;

class TextTextureAtlas {
	unordered_map< const char *, WordSprites > myStringsToSpritesDictionary;

	GLuint myTextAtlasBuffer;

	#ifdef NATIVE
//	std::vector<char> testpixelMemory;

		char myPixelMemory[2048 * 2048 * 4];
		sk_sp<SkSurface> myDrawingSurface;
	#endif

public:
	void test();
	// renderTempWord();
	// copyTempWordToAtlas();
	TextTextureAtlas();
	
	void uploadEntireTexture();
	void fetchWordSprite(utf8_string word);
};


