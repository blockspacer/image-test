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
#include "PanningBar.h"


class App {
	static GlContext  myGlContext;
	TextLayout text;
	Bubbles    myBubbles;

//	KeyEvents  
	static MouseEvents    myMouseHandler;
	static RedrawRequests myRedrawQueue;
	static PanningBar     myPanningBar;

	void setCallbacks(GLFWwindow* pWin);

	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
public:
	GlContext      &glContext()   {return myGlContext;};
	RedrawRequests &redrawQueue() {return myRedrawQueue;};
	void init();
	void draw();
	void createWindow(WindowId parent);

};


