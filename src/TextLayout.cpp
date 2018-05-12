#include "TextLayout.h"


void TextLayout::setupOnSharedContext(GlContext &ctx) {

}

void TextLayout::initOnFirstContext(GlContext &ctx) {
	cout<<"hi"<<endl;
	myTextAtlas.initOnFirstContext();
	myTextAtlas.createTextureAtlas();
check_gl_errors("pree test");
	myTextAtlas.test();
check_gl_errors("post test");
}


