#include "Window.h"

#include "Workspace.h"

Window::Window(WindowId id) {
	myId = id;
}

void Window::setupVAOs() {
	glGenVertexArrays(1, &textVAO);
	glGenVertexArrays(1, &bubblesVAO);
	glBindVertexArray(    bubblesVAO);
}

bool Window::screenUnitsSizeChanged(int w, int h) {
	if (::x(mySizeInScreenUnits)==w && ::y(mySizeInScreenUnits)==h)
		return false;
	mySizeInScreenUnits.real(w);
	mySizeInScreenUnits.imag(h);
	return true;
}

bool Window::mouseMotion(Point pos, RedrawRequests &redrawReqests) {
	cout<<::x(pos)<<endl;
	redrawReqests.redrawAllWindows();

	// if over panning bar then don't do anything

	// if going to or from a bubble's halo, or a different part of one, mark it for redrawing (for that window only?)
	return true;

	// if over a menu, highlight selected option if it's changed

}