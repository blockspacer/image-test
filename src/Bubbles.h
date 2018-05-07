#pragma once

#include <vector>
using std::vector;

#include "GlContext.h"

#define VERTICES_PER_BUBBLE_SIDE   8
#define VERTICES_PER_BUBBLE_CORNER 6

using BubbleGroupId = size_t;
using BubbleId = GLuint;

struct Bubble {
	BubbleId id;
//	bool memberOfGroup {false};
	bool unused {true};
	BubbleGroupId groupId {0};
	// pointer to contents...
	float x, y, w, h;

	Bubble(float _x, float _y, float _w, float _h, BubbleId _id) :
		x {_x}, y {_y}, w {_w}, h {_h}, id {_id} {};
	void markUnused();
};

struct BubbleGroup {
	vector<BubbleId> members;
	float x,y,w,h;
};

struct BubbleVertex {
	GLfloat x,y;
	GLuint  id;
	BubbleVertex(GLfloat x, GLfloat y, GLuint id) : x {x}, y{y}, id{id} {}; 
};

struct BubbleInfo {
	GLfloat x, y,
		w, h,
		gradientLeft, gradientRight,
		mouseOver;
	GLuint gradientLeftColor, gradientRightColor;
};

class AllBubbles {
	vector<Bubble>      mBubbles;
	vector<BubbleGroup> mGroups;
	
	vector<BubbleVertex> mBubbleVertices; // the same for every GL context
	vector<GLshort>      mBubbleIndices;
public:
	AllBubbles();
	void setupContext(GlContext &ctx);
	BubbleId createBubble(float x, float y, float w, float h);
	void uploadBubbleVertexDataToContext(GlContext &ctx, BubbleId id);
	void uploadBubblePositionDataToContext(GlContext &ctx, BubbleId id);
	void draw(GlContext &ctx);
};