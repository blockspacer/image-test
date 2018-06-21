#pragma once

#include <vector>
//using std::vector;

#include "GlContext.h"

#include "Window.h"
#include "DataTextureTemplate.h"

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

#define VERTICES_PER_BUBBLE_SIDE   8
#define VERTICES_PER_BUBBLE_CORNER 6

#define VERTICES_PER_BUBBLE (4 * (VERTICES_PER_BUBBLE_SIDE + VERTICES_PER_BUBBLE_CORNER) + 1)

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
	GLfloat x, y, id;
	BubbleVertex(GLfloat x, GLfloat y, GLfloat id) : x {x}, y{y}, id{id} {}; 
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
		GLfloat x,
		GLfloat y
		) : x{x},
			y{y},
			w{10.0f},
			h{10.0f},
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
	vector<GLshort>      myBubbleIndices;
	vector<BubbleInfo>   myBubblePositions;

    size_t mySpaceAvailable {1};

	GLuint myVertexBuffer,
			myVertexIndices,
			myDataTexture
			;

	int myPositionVarying {-1},
		myBubbleIdVarying {-1},
		myBubbleInfoSamplerUniform {-1},
		myDataTextureWidthUniform {-1}
		;
	mat4 myTransformationMatrix;

	void generateBubbleVertexIndices(size_t first, size_t last);
	void setupBuffers(GlContext &ctx);
	void setupBuffersInOtherContexts(GlContext &ctx);
	void enlargeBubbleBuffers(GlContext &ctx);
public:
	Bubbles();
	void initializeFirstContext(GlContext &ctx);
	void setupSharedContext(GlContext &ctx, WindowId win);
	void commonSetup();

	BubbleId createBubble(GlContext &ctx, float x, float y, float w, float h);
	void uploadVertexData(GlContext &ctx, BubbleId id);
	void uploadBubblePositions();
	void draw(GlContext &ctx, WindowId id);
	size_t count() {return myBubbles.size();};
};