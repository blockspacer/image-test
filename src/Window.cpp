#include "Window.h"


Window::Window(WindowId id) {
	myId = id;
}

void Window::setupVAOs() {
	glGenVertexArrays(6, &bubblesVAO);
	// glGenVertexArrays(1, &textVAO);
	// glGenVertexArrays(1, &panningBarBackgroundVAO);
	// glGenVertexArrays(1, &panningBarBubbleVAO);
	// glGenVertexArrays(1, &panningBarWindowOutlineVAO);
	// glGenVertexArrays(1, &panningBarWindowViewAreaVAO);

}

int Window::panningBarPixelHeight(Workspace& wksp) {
	const Point s = wksp.size();
	return (::y(s) * pixelWidth()) / ::x(s);
}
