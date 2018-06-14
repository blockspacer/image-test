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

class PanningBar {
	mat4 myTransformationMatrix;
public:
	void draw(GlContext &ctx, WindowId win, Workspace &wksp, Bubbles &bubbles);
};


