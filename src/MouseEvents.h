#pragma once

#include "globals.h"
#include "RedrawRequests.h"
#include "MouseEventInterface.h"
#include "GlContext.h"
#include "Window.h"
#include "Bubbles.h"

class MouseEvents {

public:
	void moved(GLFWwindow* window, double xpos, double ypos, GlContext &ctx, RedrawRequests &redrawQueue);
	void buttonInput(GLFWwindow* window, int button, int action, int mods, GlContext &ctx, RedrawRequests &redrawQueue);

};


