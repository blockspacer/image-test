#pragma once

#include "globals.h"
#include "RedrawRequests.h"
#include "GlContext.h"
#include "MouseEventInterface.h"
#include "Window.h"
#include "Bubbles.h"
#include "PanningBar.h"
#include "Workspace.h"

enum MouseTarget {Nothing, Panningbar, WorkArea, Menu};

class MouseEvents {
	MouseTarget myMouseTarget {Nothing};

public:
	void moved(GLFWwindow* window, double xpos, double ypos, GlContext &ctx, Workspace& wksp, PanningBar &panBar, Bubbles &bubls, RedrawRequests &redrawQueue);
	void buttonInput(GLFWwindow* window, int button, int action, int mods, GlContext &ctx, PanningBar &pBar, Workspace& wksp, RedrawRequests &redrawQueue);

};


