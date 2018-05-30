#pragma once

//#include <GL/glew.h>

//#define GLFW_INCLUDE_ES3 1

//#include <GLFW/glfw3.h>


//#include <vector>
//using std::vector;

#include "GlContext.h"
#include "Bubbles.h"
#include "TextLayout.h"
#include "MouseEvents.h"
#include "RedrawRequests.h"

struct App {
	static GlContext  glContext;
	TextLayout text;
	Bubbles    bubbles;

//	KeyEvents  
	static MouseEvents    mouseHandler;
	static RedrawRequests redrawQueue;

	void init();
	void draw();
	void setCallbacks(GLFWwindow* pWin);
	void createWindow(WindowId parent);

	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);


};


