#pragma once

#include <vector>
//using std::vector;

#include "GlContext.h"

#include "Windows.h"

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
		bubbleGradientRightRed, bubbleGradientRightGreen, bubbleGradientRightBlue,
		BubblePositionInfoMemberCount
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

		BubbleInfo(
		GLfloat x,
		GLfloat y
		) : x{x},
			y{y},
			w{10.0f},
			h{10.0f},
			mouseOver{0.0f},
			gradientLeft{-1.0f}, 
			gradientRight{1.0f},
			
			gradientYIntercept{1.0f},
			gradientGradient{-1.0f},

			gradientLeftRed{0.5f},
			gradientLeftGreen{0.0f}, 
			gradientLeftBlue{0.5f},

			gradientRightRed{0.0f},
			gradientRightGreen{0.5f}, 
			gradientRightBlue{0.0f}
		{};
};

class Bubbles {
	vector<Bubble>      mBubbles;
	vector<BubbleGroup> mGroups;
	
	vector<BubbleVertex> mBubbleVertices; // these contain the same info for every GL context
	vector<GLshort>      mBubbleIndices;  // but have to be uploaded to each individually
	vector<BubbleInfo>   mBubblePositions;

    size_t mySpaceAvailable {1};

	GLuint myVertexBuffer,
			myVertexIndices,
			myDataTexture
			;

    int myPositionVarying {-1},
    	myBubbleIdVarying {-1},
    	mySamplerUniform {-1},
    	myDataTextureWidthUniform {-1}
    ;

	void generateBubbleVertexIndices(size_t first, size_t last);
	void setupBuffers(GlContext &ctx);
	void setupBuffersInOtherContexts(GlContext &ctx);
	void enlargeBuffers(GlContext &ctx);
public:
	Bubbles();
	void setupOnFirstContext(GlContext &ctx);
	void setupOnSharedContext(GlContext &ctx, WindowId win);
	void commonSetup();

	BubbleId createBubble(GlContext &ctx, float x, float y, float w, float h);
	void uploadVertexData(GlContext &ctx, BubbleId id);
	void uploadBubblePositions();
	void draw(GlContext &ctx, WindowId id);
};