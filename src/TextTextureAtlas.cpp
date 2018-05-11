#include "TextTextureAtlas.h"

#include "SkGraphics.h"

//#include "SkDocument.h"

#ifdef NATIVE
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
	TextTextureAtlas::TextTextureAtlas() {
		SkGraphics::Init();

		// my linux machine refuses to create RGBA skia surfaces, but will allow BGRA ones
		// my macbook is the opposite
		// i have no idea if i'm compiling skia wrong or it's about endianness or what

		// it doesn't matter to OpenGL, though, as all text is rendered white and colored in the shader

		SkImageInfo drawingSurfaceInfo = SkImageInfo::Make(2048, 2048, // minimum guaranteed texture size on WebGL
		// kN32_SkColorType, // crash linux, safe mac
		kRGBA_8888_SkColorType,  // crash linux, safe mac
		// kBGRA_8888_SkColorType,    //safe linux, mac crash

		kPremul_SkAlphaType
		// kOpaque_SkAlphaType
	);

//	size_t rowBytes = drawingSurfaceInfo.minRowBytes();
//	size_t mSize = drawingSurfaceInfo.getSafeSize(rowBytes);
	//myPixelMemory.resize(2048 * 2048 * 4);  // allocate memory

	myDrawingSurface = 
		SkSurface::MakeRasterDirect(drawingSurfaceInfo, &myPixelMemory[0], 2048 * 4);

	if (myDrawingSurface != 0)
		return;

	drawingSurfaceInfo = SkImageInfo::Make(2048, 2048, 
		kBGRA_8888_SkColorType,    //safe linux, mac crash
		kPremul_SkAlphaType
	);	

	myDrawingSurface = 
		SkSurface::MakeRasterDirect(drawingSurfaceInfo, &myPixelMemory[0], 2048 * 4);

	if (myDrawingSurface != 0)
		return;

	cout << "Neither RGBA nor BGRA skia canvasses can be created." << endl;


	}

#else
	TextTextureAtlas::TextTextureAtlas() {
		// obtain an html 5 canvas
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