#include "MouseEvents.h"

void MouseEvents::moved(GLFWwindow* window, double xpos, double ypos, GlContext &ctx, Workspace &wksp, PanningBar &panBar, Bubbles & bubls, RedrawRequests &redrawQueue) {

	Window &win = ctx.lookupWindow(window);

	//check if mouse button down, in which case send a drag event

	Point pos = Point(float(xpos),float(ypos));

	float pbph = win.panningBarPixelHeight(wksp) / win.pixelsPerScreenunit();

	// if mouse button is down and myMouseTarget isn't OverNothing then send it to that target

	if (ypos <= pbph) {
		Point wkspPos = pos * (::x(wksp.size()) / win.screenunitWidth()) - wksp.topLeft();

		panBar.mouseMotion(wkspPos, win, ctx, wksp, bubls, redrawQueue);
		cout << "over panning bar"<<endl;
	}
	else
		cout<<"over workspace"<<endl;

	//check if it's over a menu?

}

void MouseEvents::buttonInput(GLFWwindow* window, int button, int action, int mods, GlContext &ctx, RedrawRequests &redrawQueue) {
	Window &win = ctx.lookupWindow(window);

	bool handled = false;

	if (action == GLFW_PRESS) {
		WindowId parent = win.id();
		SHOW_TYPE(parent)
		redrawQueue.newWindow(parent);
	}

//	bool handled = win.mouseButtonInput( Point(float(xpos),float(ypos)), redrawQueue );
	if (! handled) {
		// check to see if it's over a bubble halo, or bubble
	}
}



