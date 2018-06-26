#pragma once

#include "globals.h"
#include "GlContext.h"
#include "RedrawRequests.h"
#include "MouseEventInterface.h"
#include "Window.h"
#include "Bubbles.h"
#include "PanningBar.h"

class MouseEvents {

public:
	void moved(GLFWwindow* window, double xpos, double ypos, GlContext &ctx, PanningBar &panBar, RedrawRequests &redrawQueue);
	void buttonInput(GLFWwindow* window, int button, int action, int mods, GlContext &ctx, RedrawRequests &redrawQueue);

};


