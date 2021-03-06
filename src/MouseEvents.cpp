#include "MouseEvents.h"

void MouseEvents::moved(GLFWwindow* window, double xpos, double ypos, GlContext &ctx, Workspace &wksp, PanningBar &panBar, Bubbles & bubls, RedrawRequests &redrawQueue) {


	// double x,y;
	// glfwGetCursorPos(window, &x, &y);
	// xpos = x; ypos = y;


	Window &win = ctx.lookupWindow(window);
	GlContext::setMouseOverWindow(win.id());

	//check if mouse button down, in which case send a drag event

	Point pos = Point(float(xpos),float(ypos));

	float pbsuh = win.panningBarPixelHeight(wksp) / win.pixelsPerScreenunit();

	// if mouse button is down and myMouseTarget isn't OverNothing then send it to that target
cout<<ypos<<endl;
	if (0 <= ypos && ypos <= pbsuh && 0 <= xpos && xpos < win.screenunitWidth()) {
		if (myMouseTarget != Panningbar) {
			panBar.mouseIsOverYou(redrawQueue);
		}
		myMouseTarget = Panningbar;

		Point wkspPos = pos * (::x(wksp.size()) / win.screenunitWidth()) - wksp.topLeft();
		panBar.mouseMotion(wkspPos, win, ctx, wksp, bubls, redrawQueue);
	}
	else {
		if (myMouseTarget == Panningbar) {
			panBar.mouseIsNotOverYou(redrawQueue);
			redrawQueue.redrawPanningBar(win.id());
		}
		myMouseTarget = WorkArea;
		
		float x = ::x(pos), y = ::y(pos) - pbsuh;
		x /= win.screenunitWidth();
		y /= win.screenunitHeight() - pbsuh;

		Point s = win.viewportCMsize(wksp);
		// Point p {( ::x(pos)          /  win.screenunitWidth()          ) * ::x(s)
		// 	,	 ((::y(pos) - pbsuh) / (win.screenunitHeight() - pbsuh)) * ::y(s) };

		x *= ::x(s);
		y *= ::y(s);

		Point tlCorner = win.viewportCenter() - s/2.0f;

		Point wkspPos = tlCorner + Point{x, y};

		bubls.mouseMotion(wkspPos, ctx, wksp, redrawQueue);
//		cout<<"over workspace "<<wkspPos<<endl;
	}

	//check if it's over a menu?

}

void MouseEvents::buttonInput(GLFWwindow* window, int button, int action, int mods, GlContext &ctx, PanningBar &panBar, Workspace& wksp, RedrawRequests &redrawQueue) {
	Window &win = ctx.lookupWindow(window);
cout<<"iii"<<endl;
	bool handled = false;

	if (myMouseTarget == Panningbar) {
		panBar.mouseButtonInput(win, button, action, mods, ctx, panBar, wksp, redrawQueue);
	}

	if (action == GLFW_PRESS) {
		// WindowId parent = win.id();
		// SHOW_TYPE(parent)
		// redrawQueue.newWindow(parent);
	}

//	bool handled = win.mouseButtonInput( Point(float(xpos),float(ypos)), redrawQueue );
	if (! handled) {
		// check to see if it's over a bubble halo, or bubble
	}
}



