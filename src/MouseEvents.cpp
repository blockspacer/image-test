#include "MouseEvents.h"

void MouseEvents::moved(GLFWwindow* window, double xpos, double ypos, GlContext &ctx, RedrawRequests &redrawQueue) {

	Window &win = ctx.lookupWindow(window);

	//check if mouse button down, in which case send a drag event

	bool handled = win.mouseMotion( Point(float(xpos),float(ypos)), redrawQueue );
	if (! handled) {
		// check to see if it's over a bubble halo, or bubble
	}
}

void MouseEvents::buttonInput(GLFWwindow* window, int button, int action, int mods, GlContext &ctx, RedrawRequests &redrawQueue) {
	Window &win = ctx.lookupWindow(window);

	bool handled = false;

	if (action == GLFW_PRESS) {
		WindowId parent = win.id;
		SHOW_TYPE(parent)
		redrawQueue.newWindow(parent);
	}

//	bool handled = win.mouseButtonInput( Point(float(xpos),float(ypos)), redrawQueue );
	if (! handled) {
		// check to see if it's over a bubble halo, or bubble
	}
}



