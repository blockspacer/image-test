#include "Window.h"

#include "Workspace.h"

Window::Window(WindowId _id) {
	id = _id;
}

void Window::setupVAOs() {
	glGenVertexArrays(1, &textVAO);
	glGenVertexArrays(1, &bubblesVAO);
	glBindVertexArray(    bubblesVAO);
}

bool Window::mouseMotion(Point pos, RedrawRequests &redrawReqests) {
	cout<<::x(pos)<<endl;
	redrawReqests.redrawAllWindows();

	// if over panning bar then don't do anything

	// if going to or from a bubble's halo, or a different part of one, mark it for redrawing (for that window only?)
	return true;

	// if over a menu, highlight selected option if it's changed

}