#pragma once
 
#include <complex>
using std::complex;

#include "globals.h"
#include "MouseEventInterface.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>


struct Window : public MouseEventInterface {
	WindowId    id;
	GLFWwindow  *glfwHandle;
	GLFWmonitor *glfwMonitorHandle;
	bool	    unused {false};
	complex<float> topLeftInWorkspaceCM,
				   widthHeightInWorkspaceCM,
				   positionInScreenUnits,
				   sizeInScreenUnits,
				   sizeInPixels;
	float screenUnitsPerCM;

	bool		needsRefresh {true};

	GLuint bubblesVAO {0},
			textVAO {0};

	Window(WindowId id);

	void setupVAOs();

	complex<float> center() {return topLeftInWorkspaceCM + 0.5f * widthHeightInWorkspaceCM;};

	bool mouseMotion(Point pos, RedrawRequests &redrawReqests);
//	void discoverMonitor
};


