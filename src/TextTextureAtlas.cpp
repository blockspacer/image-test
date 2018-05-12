#include "TextTextureAtlas.h"

//#include "SkDocument.h"

void TextTextureAtlas::test() {
#ifdef NATIVE
    SkPaint paint;
    paint.setStyle(SkPaint::kFill_Style);
    paint.setAntiAlias(true);
    paint.setStrokeWidth(4);
    paint.setColor(0xffFE938C);

    SkCanvas *canvas = myDrawingSurface->getCanvas();
    SkRect rect = SkRect::MakeXYWH(10, 10, 100, 160);
    canvas->drawRect(rect, paint);
#endif

uploadEntireSurface();
//cout<<hi[3]<<endl;
}

void TextTextureAtlas::uploadEntireSurface() {
//glEnable(GL_TEXTURE_3D);
int i;
	glGetIntegerv(GL_MAX_TEXTURE_SIZE, &i);
cout<<"fax: "<<i<<endl;
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

	const GLvoid *pixels  = myPixelMemory.data();
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

cout<<"pixi "<<pixels<<endl;
#if NATIVE
//cout<<glGetString(GL_EXTENSIONS)<<endl;
	check_gl_errors("oh noes!");
#else
#endif
}

void TextTextureAtlas::createTextureAtlas() {

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


	glGenTextures(1, &myTextureAtlas);
	glBindTexture(GL_TEXTURE_2D_ARRAY, myTextureAtlas);

	// Allocate the storage.
	// https://github.com/kripken/emscripten/issues/5747
	// https://www.khronos.org/webgl/public-mailing-list/public_webgl/1410/msg00033.php
//	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, ATLAS_SIZE, ATLAS_SIZE, 1 + myAvailablePages);

	const GLenum  target 	= GL_TEXTURE_2D_ARRAY;
	const GLint   level 	= 0;
	const GLint   internalFormat = GL_RGBA;
	const GLsizei width 	= ATLAS_SIZE;
	const GLsizei height 	= ATLAS_SIZE;
	const GLsizei depth 	= 1;
	const GLint   border 	= 0;
	const GLenum  format 	= GL_RGBA;
	const GLenum  type 	    = GL_UNSIGNED_BYTE;
	const GLvoid* data 	    = &myPixelMemory[0];

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
check_gl_errors("creating atlas texture");
	// Always set reasonable myTextureAtlas parameters
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D_ARRAY, GL_TEXTURE_MAG_FILTER, GL_NEAREST); //GL_LINEAR
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

void TextTextureAtlas::initOnFirstContext() {
	myPixelMemory.resize(ATLAS_BYTES);
#ifdef NATIVE
cout<<"hum"<<endl;
	SkGraphics::Init();

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

	myDrawingSurface = 
		SkSurface::MakeRasterDirect(drawingSurfaceInfo, &myPixelMemory[0], ATLAS_SIZE * 4);

	if (myDrawingSurface != 0)
		return;

	cout << "Couldn't make an RGBA Skia drawing surface :("<<endl;

	drawingSurfaceInfo = SkImageInfo::Make(ATLAS_SIZE, ATLAS_SIZE, 
		kBGRA_8888_SkColorType,    //safe linux, mac crash
		kPremul_SkAlphaType
	);	

	myDrawingSurface = 
		SkSurface::MakeRasterDirect(drawingSurfaceInfo, &myPixelMemory[0], ATLAS_SIZE * 4);

	if (myDrawingSurface != 0)
		return;

	cout << "Neither BGRA now RGBA drawing surfaces can be created >:O" << endl;
#else // web version
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
