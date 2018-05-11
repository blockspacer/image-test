#include "TextTextureAtlas.h"

#include "SkGraphics.h"

//#include "SkDocument.h"

void TextTextureAtlas::createTextureAtlas() {

	if (myTextureAtlas != 0) {
		glGenFramebuffers(1, &myFramebuffer);
		glBindFramebuffer(GL_FRAMEBUFFER, myFramebuffer);
		GLuint spareHandle =               myTextureAtlas;
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
	glTexStorage3D(GL_TEXTURE_2D_ARRAY, 1, GL_RGBA8, ATLAS_SIZE, ATLAS_SIZE, 1 + myAvailablePages);

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

#ifdef NATIVE
void TextTextureAtlas::initOnFirstContext() {
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
}
#else // web version
vois TextTextureAtlas::initOnFirstContext() {
		// obtain an html 5 canvas
	cout<<"wub"<<endl;
	}

#endif


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
