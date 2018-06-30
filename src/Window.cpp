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
	return (::y(s) * pixelWidth()) / ::x(s);
}
