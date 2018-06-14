#pragma once

#include <iostream>
using std::cout;
using std::endl;

#include "GlContext.h"
#include "TextTextureAtlas.h"

struct TextSpriteVertex {
	float x, y,
		red, green, blue,
		bgRed, bgGreen, bgBlue,
		texU, texV, texLayer;

	TextSpriteVertex(float x, float y, float u, float v) :
		x{x}, y{y},
		red{1.0f}, green{1.0f}, blue{1.0f},
		bgRed{1.0f}, bgGreen{1.0f}, bgBlue{1.0f},
		texU{u}, texV{v}, texLayer{0.0f}
		{};
};

class TextLayout {
	static TextTextureAtlas myTextAtlas;
	GLuint staticTextVAO {0},
			myVertexBuffer {0},
			myVertexBufferSize {6}
	;

	vector<TextSpriteVertex> myVertices;

public:
	void sharedSetup(GlContext &ctx);
	void setupSharedContext(GlContext &ctx);
	void initializeFirstContext(GlContext &ctx);
	void addTextureQuad(GlContext &ctx, float x, float y, float w, float h, float texU, float texV, float texLayer, float texWidth, float texHeight);
	void test(GlContext &ctx);
	void uploadVertexData(GlContext &ctx);
	void draw(GlContext &ctx);

};




