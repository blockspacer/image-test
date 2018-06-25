#include "Window.h"


Window::Window(WindowId id) {
	myId = id;
}

void Window::setupVAOs() {
	glGenVertexArrays(1, &bubblesVAO);
	glGenVertexArrays(1, &textVAO);
//	glBindVertexArray(    bubblesVAO);
	glGenVertexArrays(1, &backgroundVAO);
	glGenVertexArrays(1, &panningBarBubbleVAO);
}

int Window::panningBarPixelHeight(Workspace& wksp) {
	const Point s = wksp.size();
	cout<<"um"<<endl;
	return (::y(s) * pixelWidth()) / ::x(s);
}

bool Window::mouseMotion(Point pos, RedrawRequests &redrawReqests) {
	cout<<::x(pos)<<endl;
	redrawReqests.redrawAllWindows();

	// if over panning bar then don't do anything

	// if going to or from a bubble's halo, or a different part of one, mark it for redrawing (for that window only?)
	return true;

	// if over a menu, highlight selected option if it's changed

}