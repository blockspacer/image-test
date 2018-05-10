#include "Windows.h"

Window::Window(WindowId _id) {
	id = _id;
}

void Window::setupVAOs() {
	glGenVertexArrays(1, &bubblesVAO);
	glBindVertexArray(    bubblesVAO);
}