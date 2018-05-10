#pragma once
 
#include <complex>
using std::complex;

#include "globals.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>


using WindowId = size_t;

struct Window {
	WindowId   id;
	GLFWwindow *glfwHandle;
	bool	   unused {false};
	complex<float> topLeftInCM,
				   widthHeightInCM;

	GLuint 	 bubblesVAO {0};

	Window(WindowId id);

	void setupVAOs();

	complex<float> center() {return topLeftInCM + 0.5f * widthHeightInCM;};

//	void discoverMonitor
};


