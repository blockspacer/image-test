#pragma once

//#include "glm/mat3x3.hpp"
#include "include/glm/glm.hpp"
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

#include "globals.h"
#include "Window.h"
#include "GlContext.h"
#include "Workspace.h"
#include "Bubbles.h"

struct ColoredVertex {
	float x, y, z,
		  r, g, b, a,
		  t, v ;};

class PanningBar {
	mat4   myTransformationMatrix;
	GLuint myUBO {0}
		,	myVertexBuffer {0}
		,	myPanningBarBubbleVertexBuffer {0}
		,	myWindowOutlineBuffer {0}
	;
	ColoredVertex myBackgroundVertices[4]
				, myPanningBarBubbleVertices[8]
				;
	vector<ColoredVertex> myWindowOutline;

public:
	void draw(GlContext &ctx, WindowId win, Workspace &wksp, Bubbles &bubbles);
	void initializeFirstContext(GlContext &ctx);
	void prepWindowOutline(Window& win);

};


