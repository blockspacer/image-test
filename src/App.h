#pragma once

//#include <GL/glew.h>

//#define GLFW_INCLUDE_ES3 1

//#include <GLFW/glfw3.h>


//#include <vector>
//using std::vector;

#include "globals.h"

DISABLE_WARNING(unused-parameter,unused-parameter,unused-parameter)
DISABLE_WARNING(reorder, reorder, reorder) 

#include "GlContext.h"
#include "Bubbles.h"
#include "TextLayout.h"
#include "MouseEvents.h"
#include "RedrawRequests.h"
#include "PanningBar.h"
#include "Workspace.h"

class App {
	static GlContext  myGlContext;
	static TextLayout myText;
	static Bubbles    myBubbles;

//	KeyEvents  
	static MouseEvents    myMouseHandler;
	static RedrawRequests myRedrawQueue;
	static PanningBar     myPanningBar;
	static Workspace      myWorkspace;

	static bool myFirstFrameResize; // fix for my linux window manager (i3) which seems bad about callbacks
	static bool myFirstWindowResize;

	static	void setCallbacks(GLFWwindow* pWin);

	static void cursorPositionCallback(GLFWwindow* window, double xpos, double ypos);
	static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);

	static void monitorCallback(GLFWmonitor* monitor, int event);
	static void framebufferSizeCallback(GLFWwindow* window, int width, int height);
	static void windowSizeCallback(GLFWwindow* window, int width, int height);
	static void redrawCallback(GLFWwindow*);
	static void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	static void windowPosCallback(GLFWwindow* pWin, int xpos, int ypos);

	static void windowJustCreated() {myFirstFrameResize = true; myFirstWindowResize = true;};
public:
	GlContext      &glContext()   {return myGlContext;};
	RedrawRequests &redrawQueue() {return myRedrawQueue;};
static	void init();
static	void draw();
static	void createWindow(WindowId parent);
	void webCanvasResize(int w, int h);
};


