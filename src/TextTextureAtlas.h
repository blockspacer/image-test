#pragma once

#include "globals.h"

#include <GL/glew.h>

#define GLFW_INCLUDE_ES3 1

#include <GLFW/glfw3.h>


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

	#include "SkTypeface.h"

	#include "SkStream.h"
	#include "SkSurface.h"
	#include "SkCanvas.h"
	#include "SkGraphics.h"
	#include "SkColor.h"
#endif

#include "tinyutf8.h"

#define ATLAS_SIZE  512 // 2048 is the minimum guaranteed texture size on WebGL
#define ATLAS_BYTES (ATLAS_SIZE * ATLAS_SIZE * 4)

struct SpritePosition {
	float layer {-1}, top {-1}, left {-1}, width {-1}, height {-1};
	SpritePosition(int x, int y, int w, int h) : layer{0.0f}, left{float(x)}, top{float(y)}, width{float(w)}, height{float(h)} {};
	SpritePosition(int layer, int x, int y, int w, int h) : layer{float(layer)}, left{float(x)}, top{float(y)}, width{float(w)}, height{float(h)} {};
	SpritePosition(float layer, float x, float y, float w, float h) : layer{layer}, left{x}, top{y}, width{w}, height{h} {};
};

enum FontFaceTypes {normal = 0, bold, italic, bolditalic, monospaced, fontFaceTypesCount};

using FontFaceVariants = SpritePosition[5];

struct FontFace {SpritePosition
 normal ,
 bold ,
 italic ,
 bolditalic ,
 monospaced ;
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
	vector< SpritePosition >                   myTempToPermTransfersToMake;

	int myCurrentRowTop    {0},
		myCurrentRowHeight {0},
		myCurrentRowLeft   {0},

		myTextSize         {20}
		;
	FontFaceTypes myCurrentFont {normal};





	GLuint myTextureAtlas {0},
			myFramebuffer {0};
	size_t myAvailablePages {1},
			myHighestUsedPage{1};

	#ifdef NATIVE
		SkCanvas *pMySkiaCanvas;
		sk_sp<SkSurface> pMyDrawingSurface;
		SkPaint myTextPaint;
	#else // web
		GLubyte *myDrawingSurface;
	#endif

public:
	void           setTextStyle(int pixelSize, FontFaceTypes face);
	SpritePosition drawText(const utf8_string str); // returns width in pixels
	void           drawBox(const SpritePosition &sp);
//	int  measureTextWidth(const utf8_string str);
	void test();
	// renderTempWord();
	// copyTempWordToAtlas();
	void initOnFirstContext(GlContext &ctx);
	void createTextureAtlas(GlContext &ctx);
	void drawCrosshairs(int x, int y, int len = 50);
#ifdef NATIVE
	float attemptToDraw(const utf8_string str, uint32_t *unicodeChar, float widthSoFar, bool abort_allowed);
	SpritePosition drawSkiaString(const utf8_string str, bool abort_allowed, int measure_char_count = -2);
#endif
void uploadEntireTexture();
	void uploadEntireSurface();
	SpritePosition getTemporarySprite(utf8_string word, int pixelSize, FontFaceTypes face);
	SpritePosition getPermanentSprite(utf8_string word, int pixelSize, FontFaceTypes face);

};

extern "C" int receive_canvas(char * buffer, int width, int height);
