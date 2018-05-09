#pragma once

#include <vector>
using std::vector;

#include "GlContext.h"

#define VERTICES_PER_BUBBLE_SIDE   8
#define VERTICES_PER_BUBBLE_CORNER 6

#define VERTICES_PER_BUBBLE (4 * (VERTICES_PER_BUBBLE_SIDE + VERTICES_PER_BUBBLE_CORNER) + 1)


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
	GLfloat x, y, id;
	BubbleVertex(GLfloat x, GLfloat y, GLfloat id) : x {x}, y{y}, id{id} {}; 
};

enum BubbleInfoMembers {bubbleX, bubbleY,
		bubbleW, bubbleH,
		bubbleMouseOver,
		bubbleGradientLeft, bubbleGradientRight,
		bubbleGradientYIntercept, bubbleGradientGradient,
		bubbleGradientLeftRed, bubbleGradientLeftGreen, bubbleGradientLeftBlue,
		bubbleGradientRightRed, bubbleGradientRightGreen, bubbleGradientRightBlue
	};

struct BubbleInfo {
	GLfloat x, y,
		w, h,
		mouseOver,
		gradientLeft, gradientRight,
		gradientYIntercept, gradientGradient,
		gradientLeftRed, gradientLeftGreen, gradientLeftBlue,
		gradientRightRed, gradientRightGreen, gradientRightBlue;

	BubbleInfo(
		GLfloat x,
		GLfloat y,
		GLfloat w,
		GLfloat h,

		GLfloat mouseOver,
		GLfloat gradientLeft, 
		GLfloat gradientRight,

		GLfloat gradientYIntercept,
		GLfloat gradientGradient,

		GLfloat gradientLeftRed,
		GLfloat gradientLeftGreen, 
		GLfloat gradientLeftBlue,

		GLfloat gradientRightRed,
		GLfloat gradientRightGreen, 
		GLfloat gradientRightBlue
		) :
			x{x},
			y{y},
			w{w},
			h{h},
			mouseOver{mouseOver},
			gradientLeft{gradientLeft}, 
			gradientRight{gradientRight},
			
			gradientYIntercept{gradientYIntercept},
			gradientGradient{gradientGradient},

			gradientLeftRed{gradientLeftRed},
			gradientLeftGreen{gradientLeftGreen}, 
			gradientLeftBlue{gradientLeftBlue},
			gradientRightRed{gradientRightRed},
			gradientRightGreen{gradientRightGreen}, 
			gradientRightBlue{gradientRightBlue}
		{};
};

class AllBubbles {
	vector<Bubble>      mBubbles;
	vector<BubbleGroup> mGroups;
	
	vector<BubbleVertex> mBubbleVertices; // these contain the same info for every GL context
	vector<GLshort>      mBubbleIndices;  // but have to be uploaded to each individually
	vector<BubbleInfo>   mBubblePositions;
	size_t mNextBubbleIndices {0};

	void setupVertexBuffer(GlContext &ctx, size_t bubbleCount);
	void generateBubbleVertexIndices(size_t first, size_t last);
	void enlargeVertexBuffer(GlContext &ctx);
public:
	AllBubbles();
	void setupContext(GlContext &ctx);
	void setupSharedContext(GlContext &ctx);

	BubbleId createBubble(float x, float y, float w, float h);
	void uploadBubbleVertexDataToContext(GlContext &ctx, BubbleId id);
	void uploadBubblePositionDataToContext();
	void draw(GlContext &ctx);
};