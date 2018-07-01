#pragma once

#include <vector>
//using std::vector;

#include "GlContext.h"

#include "Window.h"
#include "EnlargeableArrayBuffer.h"

//#include "glm/mat3x3.hpp"
#include "include/glm/glm.hpp"
using glm::mat4;
#include "include/glm/gtx/transform.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

using glm::scale;
using glm::translate;
using glm::mat4;
using glm::vec3;
// using glm::;
// using glm::;
// using glm::;
// using glm::;

#define STEPS_PER_BUBBLE_SIDE   8
#define STEPS_PER_BUBBLE_CORNER 5

#define VERTICES_PER_BUBBLE (4 + (STEPS_PER_BUBBLE_CORNER+2 + STEPS_PER_BUBBLE_SIDE)*4*2)

// drugs and wires playlist https://open.spotify.com/user/cryoclaire/playlist/7pSKHOUu7vRxFTPmrlghbB



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
	GLfloat x, y, id, insideOrOutside;
	BubbleVertex(GLfloat x, GLfloat y, GLfloat id, GLfloat io) : x {x}, y{y}, id{id}, insideOrOutside {io} {}; 
};

enum BubbleInfoMembers {bubbleX, bubbleY,
		bubbleW, bubbleH,
		bubbleMouseOver,
		BubblePositionInfoMemberCount
	};

enum BubbleGroupInfoMembers {
		bubbleGradientLeft, bubbleGradientRight,
		bubbleGradientYIntercept, bubbleGradientGradient,
		bubbleGradientLeftRed, bubbleGradientLeftGreen, bubbleGradientLeftBlue,
		bubbleGradientRightRed, bubbleGradientRightGreen, bubbleGradientRightBlue,
		BubbleGroupInfoMembersCount
	};

struct BubbleGroupInfo {
	GLfloat gradientLeft {0.0f}, gradientRight {1.0f},
		gradientYIntercept {0.0f}, gradientGradient {0.0f},
		gradientLeftRed {0.0f}, gradientLeftGreen {0.0f}, gradientLeftBlue {0.0f},
		gradientRightRed {0.0f}, gradientRightGreen {0.0f}, gradientRightBlue {0.0f};

};

struct BubbleInfo {
	GLfloat x, y,
		w, h,
		mouseOver,
		groupId;

	BubbleInfo(
		GLfloat x,
		GLfloat y,
		GLfloat w,
		GLfloat h,

		GLfloat mouseOver,
		GLfloat groupId
		) :
			x{x},
			y{y},
			w{w},
			h{h},
			mouseOver{mouseOver},
			groupId{groupId}
			// gradientLeft{gradientLeft}, 
			// gradientRight{gradientRight},
			
			// gradientYIntercept{gradientYIntercept},
			// gradientGradient{gradientGradient},

			// gradientLeftRed{gradientLeftRed},
			// gradientLeftGreen{gradientLeftGreen}, 
			// gradientLeftBlue{gradientLeftBlue},
			// gradientRightRed{gradientRightRed},
			// gradientRightGreen{gradientRightGreen}, 
			// gradientRightBlue{gradientRightBlue}
		{};

		BubbleInfo(
		GLfloat _x,
		GLfloat _y,
		GLfloat _w,
		GLfloat _h
		) : x{_x},
			y{_y},
			w{_w},
			h{_h},
			mouseOver{0.0f},
			groupId{0.0f}
		{};

		BubbleInfo()
		 : x{0.0f},
			y{0.0f},
			w{10.0f},
			h{10.0f},
			mouseOver{0.0f},
			groupId{0.0f}
			// gradientLeft{-1.0f}, 
			// gradientRight{1.0f},
			
			// gradientYIntercept{1.0f},
			// gradientGradient{-1.0f},

			// gradientLeftRed{0.5f},
			// gradientLeftGreen{0.0f}, 
			// gradientLeftBlue{0.5f},

			// gradientRightRed{0.0f},
			// gradientRightGreen{0.5f}, 
			// gradientRightBlue{0.0f}
		{};
};

class Bubbles {
	vector<Bubble>      myBubbles;
	vector<BubbleGroup> myGroups;
	
	vector<BubbleVertex> myBubbleVertices;
	vector<BubbleInfo>   myBubblePositions;

	EnlargeableArrayBuffer myBubbleHalos {VERTICES_PER_BUBBLE};

    size_t mySpaceAvailable {1};

	GLuint myVertexBuffer,
			myDataTexture
		,	myBackgroundBuffer
			;

	int myPositionVarying {-1},
		myBubbleIdVarying {-1},
		myBubbleInfoTextureUniform {-1},
		myBubbleInfoTextureWidthUniform {-1},
		myDrawDepthUniform {-1}
		;
	mat4 myTransformationMatrix;

	void setupBuffers(GlContext &ctx);
	void setupBuffersInOtherContexts(GlContext &ctx);
	void enlargeBubbleBuffers(GlContext &ctx);
public:
	Bubbles();
	void initializeFirstContext(GlContext &ctx);
	void setupSharedContext(GlContext &ctx, WindowId win);
	void commonSetup();
	GLuint dataTexture() {return myDataTexture;};

	BubbleId createBubble(GlContext &ctx, float x, float y, float w, float h);
	void uploadVertexData(GlContext &ctx, BubbleId id);
	void uploadBubblePositions();
	void draw(GlContext &ctx, WindowId id, Workspace& wksp);
	size_t count() {return myBubbles.size();};

	bool mouseMotion(Point pos, GlContext &ctx, Workspace &wksp, RedrawRequests &redrawReqests);

};