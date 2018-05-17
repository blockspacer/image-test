#include "TextTextureAtlas.h"

//#include "SkDocument.h"


void TextTextureAtlas::drawCrosshairs(int x, int y, int len) {
#ifdef NATIVE
	SkPaint paint;
    paint.setStyle(SkPaint::kFill_Style);
    paint.setColor(SK_ColorGREEN);
    SkRect rect = SkRect::MakeXYWH(x-len, y, 2*len+1, 1);
    
	pMySkiaCanvas->drawRect(rect, paint);
	rect = SkRect::MakeXYWH(x, y-len, 1, 2*len+1);
    pMySkiaCanvas->drawRect(rect, paint);
#else
    EM_ASM_({
		var x = $0;
		var y = $1;
		var l = $2;
		textCtx.beginPath();

		textCtx.strokeStyle = 'green';

		textCtx.moveTo(x - l , y);
		textCtx.lineTo(x + l, y);
		textCtx.moveTo(x, y - l);
		textCtx.lineTo(x, y + l);
		textCtx.stroke();

	}, x, y, len);
#endif
}

#ifdef NATIVE
	#include "SkFontMgr.h"
#endif
//#include ""


#include <iostream>
using std::cout;
using std::endl;


#include <algorithm>
#include "tinyutf8.h"
using namespace std;

int tiny()
{
	cout<<"HIIIII"<<endl;
    utf8_string str = u8"!🌍 olleH";
    for_each( str.rbegin() , str.rend() , []( char32_t codepoint ){
    	cout<<int(codepoint)<<" ";;
      cout << codepoint << endl;
    } );
    return 0;
}

void TextTextureAtlas::setTextStyle(int pixelSize, FontFaceTypes face) {
#ifdef NATIVE
	if (myTextSize != pixelSize) {
		myTextSize = pixelSize;
		myTextPaint.setTextSize(float(pixelSize));
	}
	if (face != myCurrentFont) {
		myCurrentFont = face;
		// Roboto font? user font?
		#ifdef __linux__
			string font =  "Sans"; // ::gStrings["Linux font"];
		//	string font = "Free Sans";
		#elif __APPLE__
			string font = "Helvetica"; // ::gStrings["Apple font"];
		#elif _WIN32
			string font = "Arial"; // ::gStrings["Helvetica font"];
		#else
			string font = "Sans";
		#endif

		auto weight = 400;
		auto slant  = SkFontStyle::kUpright_Slant;

		switch(face) {
			case 0:
				break;
			case 1:
				weight = 700;
				break;
			case 2:
				slant = SkFontStyle::kItalic_Slant;
				break;
			case 3:
				weight = 700;
				slant = SkFontStyle::kItalic_Slant;
				break;
			case 4:
				font = "Mono";
				break;
			}

		//todo: maybe try experimenting with condensed text?
		myTextPaint.setTypeface(SkTypeface::MakeFromName(font.c_str(), SkFontStyle (weight, 5,  slant)));
	}
#else
	if (face != myCurrentFont || myTextSize != pixelSize) {
		myCurrentFont = face; myTextSize = pixelSize;
		EM_ASM_({
			var pre = '';
			var s = 'px sans';
			switch($1) {
				case 0:
					break;
				case 1:
					pre = 'bold ';
					s = 'px sans';
					break;
				case 2:
					pre = 'italic ';
					s = 'px italic sans';
					break;
				case 3:
					pre = 'italic bold ';
					s = 'px bold italic sans';
					break;
				case 4:
					s = 'px monospace';
					break;
			}
			textCtx.font = pre + $0 + s;
			console.log(pre + $0 + s)
		}, pixelSize, face);
	}
#endif
	myTextSize    = pixelSize;
	myCurrentFont = face;
}

void TextTextureAtlas::drawBox(const SpritePosition &pos) {
#ifdef WEB
	EM_ASM_({
		textCtx.beginPath();
		var x = $0;
		var y = $1;
		var w = $2;
		var h = $3;

		textCtx.strokeStyle = 'yellow';

		textCtx.moveTo(x, y);
		textCtx.lineTo(x+w, y);
		textCtx.lineTo(x+w, y+h);
		textCtx.lineTo(x, y+h);
		textCtx.lineTo(x, y);
		textCtx.stroke();

	}, pos.left, pos.top, pos.width, pos.height);
#else
//    myTextPaint.setStyle(SkPaint::kFill_Style);

	SkRect rect = SkRect::MakeXYWH(pos.left, pos.top, pos.width, 1);
	pMySkiaCanvas->drawRect(rect, myTextPaint);

	rect = SkRect::MakeXYWH(pos.left, pos.top + pos.height, pos.width, 1);
	pMySkiaCanvas->drawRect(rect, myTextPaint);

	rect = SkRect::MakeXYWH(pos.left, pos.top, 1, pos.height);
	pMySkiaCanvas->drawRect(rect, myTextPaint);

	rect = SkRect::MakeXYWH(pos.left + pos.width, pos.top, 1, pos.height);
	pMySkiaCanvas->drawRect(rect, myTextPaint);

#endif
}

#ifdef NATIVE
// this is annoying because skia doesn't do automatic fallback for non-ASCII characters, it just draws an empty box
// i'm not sure if you're supposed to use something else for positioning your glyphs, like the situation with the Gnome project libraries Cairo and Pango, where Cairo ~can~ draw text but only in a very basic manner, and certainly not fun foreign scripts like Arabic and Devanagari
// anyway, i'm mostly concerned with mathematical symbols and greek letters, so am just going to be lazy and collect spans of < 127 ascii character code symbols to do all at once and (as i assume kerning and ligatures would mean going character-by-character wouldn't work) and doing other characters one by one
// if the optional argument measure_char_count is given this doesn't draw anything but instead measures the width of the specified number of characters into the string, a behaviour used for cursor positioning


// this tries to draw text but and return the width, but returns -1 if it wouldn't fit on that row of the texture sheet
float TextTextureAtlas::attemptToDraw(utf8_string str, uint32_t *unicodeChar, float widthSoFar, bool abort_allowed) {

	float newWidth;
	if (unicodeChar == 0)
		newWidth = myTextPaint.measureText(str.c_str(), str.size());
	else
		newWidth = myTextPaint.measureText(unicodeChar, 4);

	if (abort_allowed && myCurrentRowLeft + widthSoFar + newWidth > ATLAS_SIZE) {
		// start over
		myCurrentRowLeft = 0;
		myCurrentRowTop += myCurrentRowHeight;
		myCurrentRowHeight = myTextSize;
		return -1.0f;
	}

	myTextPaint.setColor(SK_ColorBLACK);
	SkRect rect = SkRect::MakeXYWH(myCurrentRowLeft + widthSoFar, myCurrentRowTop, newWidth, myTextSize);
	pMySkiaCanvas->drawRect(rect, myTextPaint);

	myTextPaint.setColor(SK_ColorWHITE);
	if (unicodeChar == 0)
		pMySkiaCanvas->drawText(str.c_str(), str.size(), myCurrentRowLeft + widthSoFar, myCurrentRowTop + myTextSize * 0.8f, myTextPaint);
	else
		pMySkiaCanvas->drawText(unicodeChar, 4, myCurrentRowLeft + widthSoFar, myCurrentRowTop + myTextSize * 0.8f, myTextPaint);

	return newWidth;
}

SpritePosition TextTextureAtlas::drawSkiaString(const utf8_string str, bool abort_allowed, int measure_char_count) {
	float widthSoFar = 0.0f;

	float x   = myCurrentRowLeft,
			y = myCurrentRowTop,
			h = myTextSize;

	// collect spans of ascii text to measure
	utf8_string span;
	uint32_t unicodeString[1] = {0x0};
	auto i = str.begin();
	while (i != str.end()) {
		char32_t chr = *i;
		if (int(chr) < 128) {
			// ASCII char
			span.append(chr);
		}
		else {
			// Unicode char
			if (span.size() > 0 && abort_allowed) {
//				draw and measure
				float w = attemptToDraw(span, 0, widthSoFar, abort_allowed);
				if (w == -1.0f) {
cout<<"overlap, forced to wrap"<<endl;
					return drawSkiaString(str, false);
				}

				widthSoFar += w;
				span = "";
			}

			sk_sp<SkFontMgr> mgr(SkFontMgr::RefDefault());
	//		uint32_t utf32string[] = { 0x1F310 };
			sk_sp<SkTypeface> tf(
				mgr->matchFamilyStyleCharacter(
					nullptr, SkFontStyle(), nullptr, 0, chr));
			if (tf) {
//				unicodeString.(chr);
				myTextPaint.setTypeface(tf);

		        myTextPaint.setTextEncoding(SkPaint::kUTF32_TextEncoding);
		        unicodeString[0] = chr;

				float w = attemptToDraw(span, unicodeString, widthSoFar, abort_allowed);
				if (w == -1.0f) {
cout<<"FORCED 2"<<endl;
					return drawSkiaString(str, false);

				}

				widthSoFar += w;

				FontFaceTypes font = myCurrentFont;
				myCurrentFont = fontFaceTypesCount;
		        myTextPaint.setTextEncoding(SkPaint::kUTF8_TextEncoding);
				setTextStyle(myTextSize, font);
		    }
		}
		i++;
	}
	if (span.size() != 0) {
//		draw and measure
		float w = attemptToDraw(span, 0, widthSoFar, abort_allowed);
		if (w == -1.0f) {
cout<<"FORCEDDD 3"<<endl;
			return drawSkiaString(str, false);
		}
		widthSoFar += w;
	}
	myCurrentRowLeft += widthSoFar;
	return SpritePosition(x, y, widthSoFar, h);
}
#endif

// returns position of drawn thing
SpritePosition TextTextureAtlas::drawText(const utf8_string str) {
	if (myTextSize > myCurrentRowHeight)
		myCurrentRowHeight = myTextSize;

#ifdef NATIVE
	return drawSkiaString(str, true);
#else // web
	int w = EM_ASM_INT({
		str = Pointer_stringify($0);
		return textCtx.measureText(str).width;
	}, str.c_str());

	int x = myCurrentRowLeft,
		h = myTextSize,
		y = myCurrentRowTop;

	if (myCurrentRowLeft + w <= ATLAS_SIZE) {
		// fit it onto line
		myCurrentRowLeft += w + 1;
	}
	else {
		// start a new line
		x = 0;
		y += myCurrentRowHeight + 1;
		myCurrentRowLeft = w + 1;
	}
	EM_ASM_({
		textCtx.fillStyle = "black";
		textCtx.fillRect($0, $1, $2, $3);
		textCtx.fillStyle = "white";

		textCtx.fillText(str, $0, $1 + $3 * 0.51);
	}, x, y, w, h);
	return SpritePosition(0,x,y,w,myTextSize);
#endif
}


SpritePosition TextTextureAtlas::getTemporarySprite(utf8_string word, int pixelSize, FontFaceTypes face) {
	// check if we've already got a suitable permanent sprite and if so return that
	// if not, check if we've already got a temporary that fits
	// draw it to our pixel surface and queue for transfer, return the position it will be at
}

SpritePosition TextTextureAtlas::getPermanentSprite(utf8_string word, int pixelSize, FontFaceTypes face) {
	// check if we have it
	// if not, check if this is currently a temporary; if so, copy to permanent and return the position
	// draw it to pixel memory and queue for transfer to graphics card memory; return the position it will be at
}



void TextTextureAtlas::test() {
#ifdef WEB

	myCurrentRowLeft = 10;
	myCurrentRowTop  = 20;

	setTextStyle(24, normal);

	auto sp = drawText(utf8_string("M∀xx ∃!🌍 _olleH "));
	drawBox(sp);

	drawText(utf8_string("boop "));
	drawText(utf8_string("boop "));
	drawText(utf8_string("boop "));
	drawText(utf8_string("boop "));
// drawText(utf8_string("boop "));
	// drawText(utf8_string("boop "));
	sp = drawText(utf8_string("boop "));
	drawBox(sp);

	cout<<myTextSize<<" text size"<<endl;
//	sp = drawText(utf8_string("TehWfiflay^'@\"'fgj_jgq, ∀ x ∃ "));
	drawBox(sp);
	drawCrosshairs(sp.left, sp.top);
//	drawText(utf8_string("TehWfiflay^'@\"'fgj_jgq, ∀ x ∃ "));


#else
	setTextStyle(32, bolditalic);
	auto sp = drawText(utf8_string("M∀xx ∃!🌍 olleH"));
myTextPaint.setColor(SK_ColorRED);

	drawBox(sp);
	// drawCrosshairs(myCurrentRowLeft,myCurrentRowTop+myTextSize);

	setTextStyle(24, bolditalic);
    SkPaint paint;
    paint.setStyle(SkPaint::kFill_Style);
    paint.setAntiAlias(true);
    paint.setStrokeWidth(4);
    paint.setColor(0xffFE938C);

    SkPaint paint1;
    paint1.setTextSize(32.0f);
    paint1.setAntiAlias(true);
    paint1.setColor(0x8142ffA4);
    paint1.setStyle(SkPaint::kFill_Style);

	const string font = "Ubuntu";
//	paint.setTypeface(SkTypeface::MakeFromName(font.c_str(), SkFontStyle (400, 5,  SkFontStyle::kUpright_Slant)));

    const char text[] = "Testy fight _jgq, ∀ x ∃ ";

    // SkRect rect = SkRect::MakeXYWH(10, 10, 100, 160);
    // pMySkiaCanvas->drawRect(rect, paint);

    pMySkiaCanvas->drawText(text, strlen(text), 130, 120, myTextPaint);
    drawCrosshairs(131, 121);

tiny();


    sk_sp<SkFontMgr> mgr(SkFontMgr::RefDefault());
    uint32_t utf32string[] = { 0x1F310 };
    sk_sp<SkTypeface> tf(
            mgr->matchFamilyStyleCharacter(
                    nullptr, SkFontStyle(), nullptr, 0, utf32string[0]));
    if (tf) {
	SkPaint paint;
	paint.setTextSize(64.0f);
        paint.setAntiAlias(true);
	paint.setColor(0xff4281A4);
	paint.setStyle(SkPaint::kFill_Style);
        paint.setTypeface(tf);
        paint.setTextEncoding(SkPaint::kUTF32_TextEncoding);
        pMySkiaCanvas->drawText(utf32string, sizeof(utf32string), 200.0f, 264.0f, paint);
    }

#endif

uploadEntireSurface();
//cout<<hi[3]<<endl;
}

void TextTextureAtlas::uploadEntireSurface() {
#ifdef WEB
	EM_ASM_({
		transfer_entire_canvas($0);
	}, ATLAS_SIZE);
#endif
	
//glEnable(GL_TEXTURE_3D);
int i;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &i);

check_gl_errors("enum?");
	const GLenum  target  = GL_TEXTURE_2D_ARRAY;
	const GLint   level   = 0,
	              xoffset = 0,
	              yoffset = 0,
	              zoffset = 0;
	const GLsizei width   = ATLAS_SIZE,
	              height  = ATLAS_SIZE,
	              depth   = 1;
	const GLenum  format  = GL_RGBA,
	              type    = GL_UNSIGNED_BYTE;

	const GLvoid *pixels  = gPixelMemory.data();
	glTexSubImage3D(target,
					level,
					xoffset,
					yoffset,
					zoffset,
					width,
					height,
					depth,
					format,
					type,
					pixels);

//cout<<glGetString(GL_EXTENSIONS)<<endl;
	check_gl_errors("oh noes!");
}

void TextTextureAtlas::createTextureAtlas(GlContext &ctx) {

	if (myTextureAtlas != 0) {
//		glGenFramebuffers(1, &myFramebuffer);
//		glBindFramebuffer(GL_FRAMEBUFFER, myFramebuffer);
//		GLuint spareHandle =               myTextureAtlas;

//		glBindTexture(GL_COPY_READ_BUFFER, myTextureAtlas);


// void glCopyImageSubData(
//  GLuint srcName​,
//  GLenum srcTarget​,
//  GLint srcLevel​,
//  GLint srcX​,
//  GLint srcY​,
//  GLint srcZ​,
//  GLuint dstName​,
//  GLenum dstTarget​,
//  GLint dstLevel​,
//  GLint dstX​,
//  GLint dstY​,
//  GLint dstZ​,
//  GLsizei srcWidth​,
//  GLsizei srcHeight​,
//  GLsizei srcDepth​);

// https://www.khronos.org/opengl/wiki/Texture_Storage#Texture_copy
// void glCopyImageSubData(GLuint srcName​, GLenum srcTarget​, GLint srcLevel​, GLint srcX​, GLint srcY​, GLint srcZ​, GLuint dstName​, GLenum dstTarget​, GLint dstLevel​, GLint dstX​, GLint dstY​, GLint dstZ​, GLsizei srcWidth​, GLsizei srcHeight​, GLsizei srcDepth​);


// https://www.khronos.org/registry/OpenGL/extensions/OES/OES_copy_image.txt
// glCopyImageSubData

	}

	glActiveTexture(GL_TEXTURE1);
	GLint mySamplerUniform = glGetUniformLocation(ctx.shaderProgramHandle, "spriteSheets");
	glUniform1i(mySamplerUniform, 1);

	glGenTextures(1, &myTextureAtlas);
	glBindTexture(GL_TEXTURE_2D_ARRAY, myTextureAtlas);

	// Allocate the storage.
	// https://github.com/kripken/emscripten/issues/5747
	// https://www.khronos.org/webgl/public-mailing-list/public_webgl/1410/msg00033.php
//	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, ATLAS_SIZE, ATLAS_SIZE, 1 + myAvailablePages);
https://commanderrab.deviantart.com/art/On-Being-Motivated-327103148
	const GLenum  target         = GL_TEXTURE_2D_ARRAY;
	const GLint   level          = 0,
	              internalFormat = GL_RGBA;
	const GLsizei width          = ATLAS_SIZE,
	              height         = ATLAS_SIZE,
	              depth          = 1;
	const GLint   border         = 0;
	const GLenum  format         = GL_RGBA,
	              type           = GL_UNSIGNED_BYTE;
	const GLvoid* data           = &gPixelMemory[0];

	glTexImage3D(target,
				 level,
				 internalFormat,
				 width,
				 height,
				 depth,
				 border,
				 format,
				 type,
				 data);

check_gl_errors("creating texture atlas");
	// Always set reasonable myTextureAtlas parameters
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_LINEAR); //GL_LINEAR
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);



	// https://www.khronos.org/opengl/wiki/Array_Texture
	// Upload pixel data.
	// The first 0 refers to the mipmap level (level 0, since there's only 1)
	// The following 2 zeroes refers to the x and y offsets in case you only want to specify a subrectangle.
	// The final 0 refers to the layer index offset (we start from index 0 and have 2 levels).
	// Altogether you can specify a 3D box subset of the overall myTextureAtlas, but only one mip level at a time.
	// glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, layerCount, GL_RGBA, GL_UNSIGNED_BYTE, texels);

//	glTexSubImage3D(GL_TEXTURE_2D_ARRAY, 0, 0, 0, 0, width, height, layerCount, GL_RGBA, GL_UNSIGNED_BYTE, texels);

// void gl.copyTexSubImage3D(target, level, xoffset, yoffset, zoffset, x, y, width, height);
// https://developer.mozilla.org/en-US/docs/Web/API/WebGL2RenderingContext/copyTexSubImage3D
}

void TextTextureAtlas::initOnFirstContext(GlContext &ctx) {
	gPixelMemory.resize(ATLAS_BYTES);
#ifdef NATIVE
	SkGraphics::Init();

	myTextPaint.setTextSize(20.0f);
	myTextPaint.setAntiAlias(true);
	myTextPaint.setColor(0xffffffff);
	myTextPaint.setStyle(SkPaint::kFill_Style);

	// my linux machine refuses to create RGBA skia surfaces, but will allow BGRA ones
	// my macbook is the opposite
	// i have no idea if i'm compiling skia wrong or it's about endianness or what

	// it doesn't matter to OpenGL, though, as all text is rendered white and colored in the shader


	SkImageInfo drawingSurfaceInfo = SkImageInfo::Make(ATLAS_SIZE, ATLAS_SIZE,
	// kN32_SkColorType, // crash linux, safe mac
	kRGBA_8888_SkColorType,  // crash linux, safe mac
	// kBGRA_8888_SkColorType,    //safe linux, mac crash

	kPremul_SkAlphaType
	// kOpaque_SkAlphaType
	);

	pMyDrawingSurface = 
		SkSurface::MakeRasterDirect(drawingSurfaceInfo, &gPixelMemory[0], ATLAS_SIZE * 4);

	if (pMyDrawingSurface != 0) {
		pMySkiaCanvas = pMyDrawingSurface->getCanvas();
		return;
	}

	cout << "Couldn't make an RGBA Skia drawing surface :("<<endl;

	drawingSurfaceInfo = SkImageInfo::Make(ATLAS_SIZE, ATLAS_SIZE, 
		kBGRA_8888_SkColorType,    //safe linux, mac crash
		kPremul_SkAlphaType
	);	

	pMyDrawingSurface = 
		SkSurface::MakeRasterDirect(drawingSurfaceInfo, &gPixelMemory[0], ATLAS_SIZE * 4);

	if (pMyDrawingSurface != 0) {
		pMySkiaCanvas = pMyDrawingSurface->getCanvas();
		return;
	}


	cout << "Neither BGRA now RGBA drawing surfaces can be created >:O" << endl;
#else // web version
//	int n = ;

	EM_ASM_({
		create_canvas($0);
	}, ATLAS_SIZE);
#endif
}



// #include "SkDocument.h"

// void WritePDF(SkWStream* outputStream,
//               const char* documentTitle,
//               void (*writePage)(SkCanvas*, int page),
//               int numberOfPages,
//               SkSize pageSize) {
//     SkDocument::PDFMetadata metadata;
//     metadata.fTitle = documentTitle;
//     metadata.fCreator = "Example WritePDF() Function";
//     SkTime::DateTime now;
//     SkTime::GetDateTime(&now);
//     metadata.fCreation.fEnabled  = true;
//     metadata.fCreation.fDateTime = now;
//     metadata.fModified.fEnabled  = true;
//     metadata.fModified.fDateTime = now;
//     sk_sp<SkDocument> pdfDocument = SkDocument::MakePDF(
//             outputStream, SK_ScalarDefaultRasterDPI, metadata,
//             nullptr, true);
//     assert(pdfDocument);

//     for (int page = 0; page < numberOfPages; ++page) {
//         SkCanvas* pageCanvas =
//                 pdfDocument->beginPage(pageSize.width(),
//                                        pageSize.height());
//         writePage(pageCanvas, page);
//         pdfDocument->endPage();
//     }
//     pdfDocument->close();
// }



extern "C" {
	int receive_canvas(char * buffer, int width, int height) {
		cout<<"got it! "<<width<<", "<<height<<endl;
		memcpy(&gPixelMemory[0], buffer, width * height * 4);
	//    g_game.SetImageLoaded( buffer, width, height );
		cout<<"got it!"<<endl;
	    return 0;
	}
}
