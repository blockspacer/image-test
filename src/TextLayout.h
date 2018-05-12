#pragma once

#include <iostream>
using std::cout;
using std::endl;

#include "GlContext.h"
#include "TextTextureAtlas.h"

class TextLayout {
	TextTextureAtlas myTextAtlas;
	GLuint staticTextVAO {0}

	;
public:
	void setupOnSharedContext(GlContext &ctx);
	void initOnFirstContext(GlContext &ctx);
};


