#pragma once

#include <functional>

//#include "glm/mat3x3.hpp"
#include "include/glm/glm.hpp"
#include "include/glm/gtx/transform.hpp"
#include "include/glm/gtc/matrix_transform.hpp"
#include "include/glm/gtc/type_ptr.hpp"

using glm::scale;
using glm::translate;
using glm::mat4;
using glm::vec3;
using glm::vec4;
// using glm::;
// using glm::;
// using glm::;
// using glm::;

#include "globals.h"
#include "Window.h"
#include "GlContext.h"
#include "Workspace.h"
#include "Bubbles.h"
#include "MouseEventInterface.h"

struct ColoredVertex {
	float x, y, z,
		  r, g, b, a,
		  t, v ;
	ColoredVertex(float _x, float _y, float _z, float _r, float _g, float _b, float _a, float _t, float _v) :
		x {_x}, y {_y}, z {_z}, r {_r}, g {_g}, b {_b}, a {_a}, t {_t}, v {_v} {}
	ColoredVertex() {};
};

class PanningBar {
	bool 	 myMouseOverMe {false};
	bool 	 myMouseBeingDragged {false};
	bool	 myMouseOverWindowOutline {false};
	WindowId myMouseOverWindowId {0};
	mat4    myTransformationMatrix;
	GLuint 	myUBO {0}
		,	myVertexBuffer {0}
		,	myPanningBarBubbleVertexBuffer {0}
		,	myWindowOutlineBuffer {0}
		,	myWindowViewAreaBuffer {0}
	;
	int myPosAttrib {0};
	int myColAttrib {0};
	int myTexAttrib {0};

	Vertex myBackgroundVertices[4];
	Vertex  myPanningBarBubbleVertices[8]
				;
	vector<Vertex> myWindowOutlineVertices
				,  myWindowViewAreaVertices;

	Point myDragOffset {0,0}
		, myMousePos {0,0};


public:
	void draw(GlContext &ctx, WindowId win, Workspace &wksp, Bubbles &bubbles);
	void initializeFirstContext(GlContext &ctx);
	void setupSharedContext(GlContext &ctx, WindowId newId);
	void setupAttribArray(GLuint vaoHandle, GLuint bufferHandle = 0);

	void prepWindowOutline(Window& win, Workspace& wksp);
	PanningBar() {};
	bool mouseMotion(Point pos, Window &win, GlContext &ctx, Workspace &wksp, Bubbles &bubls, RedrawRequests &redrawReqests);
	void mouseButtonInput(Window &win, int button, int action, int mods, GlContext &ctx, PanningBar &pBar, RedrawRequests &redrawQueue);
	void mouseOverYou(RedrawRequests &redrawQueue)    { myMouseOverMe = true ; };
	void mouseNotOverYou(RedrawRequests &redrawQueue) { myMouseOverMe = false; };

};


