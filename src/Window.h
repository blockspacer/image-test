#pragma once
 
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <complex>
using std::complex;

#include "globals.h"
#include "MouseEventInterface.h"
#include "Workspace.h"


class Window {
	WindowId    myId;
	GLFWwindow  *myGlfwHandle;
	GLFWmonitor *myGlfwMonitorHandle;
	bool	    unused {false};
	complex<float> myViewportCentre {15.0f, 10.0f}
				,   myScreenunitSize
				,   myPixelSize
	;
	float myScreenunitsPerCM {37.8}; // based on assumed dpi of 96

	bool		iNeedRefresh {true};

public:
	GLuint bubblesVAO {0}
		,	textVAO {0}
		,	panningBarBackgroundVAO {0}
		,	panningBarBubbleVAO {0}
		,	panningBarWindowOutlineVAO {0}
		,	panningBarWindowViewAreaVAO {0}
	;
	Window(WindowId id);

	void setupVAOs();

	complex<float> center() {return myViewportCentre;};

	void	setGlfwHandle(GLFWwindow* pWin) {myGlfwHandle = pWin; unused = false;};
	GLFWwindow* glfwHandle() {return myGlfwHandle;};

	bool 	isUnused() {return unused;};
	bool 	inUse   () {return ! unused;};

	bool    needsRefresh  () { return iNeedRefresh;  };
	void    markForRefresh() { iNeedRefresh = true;  };
	void    markRefreshed () { iNeedRefresh = false; };

	void	setPixelSize     (int w, int h) { myPixelSize     .real(w); myPixelSize     .imag(h); };
	void	setScreenunitSize(int w, int h) { myScreenunitSize.real(w); myScreenunitSize.imag(h); };
	bool	isScreenunitSizeDifferent(int w, int h)
				{ return ::x(myScreenunitSize) != w || ::y(myScreenunitSize) != h; };
	bool	isPixelSizeDifferent(int w, int h) { return ::x(myPixelSize) != w || ::y(myPixelSize) != h; };

	int 	pixelWidth () { return ::x(myPixelSize); };
	int 	pixelHeight() { return ::y(myPixelSize); };
	int 	panningBarPixelHeight(Workspace& wksp);
	Point	viewportPixelSize(Workspace & wksp) { return myPixelSize - Point(0, panningBarPixelHeight(wksp));};
	Point 	viewportScreenunitSize(Workspace &wksp) {return viewportPixelSize(wksp) / float(pixelsPerScreenunit());};
	Point	viewportCMsize(Workspace &wksp) {return viewportScreenunitSize(wksp) / myScreenunitsPerCM;};

	int 	screenunitWidth()  { return ::x(myScreenunitSize);};
	int 	screenunitHeight() { return ::y(myScreenunitSize);};
	float 	screenunitsPerCM() { return myScreenunitsPerCM   ;};
	void 	setScreenunitsPerCM(float s) {myScreenunitsPerCM = s;};
	int 	pixelsPerScreenunit() { return (! screenunitWidth()) ? 1 : pixelWidth() / screenunitWidth();};

	Point 	viewportCenter() {return myViewportCentre;};
	void 	setViewportCenter(const Point &p) {myViewportCentre = p;};
	Point 	topLeft(Workspace &wksp)     { return myViewportCentre - 0.5f * viewportCMsize(wksp);};
	Point 	bottomRight(Workspace &wksp) { return myViewportCentre + 0.5f * viewportCMsize(wksp);};

	WindowId id() {return myId;};

//	bool mouseMotion(Point pos, Workspace &wksp, RedrawRequests &redrawReqests);
//	void discoverMonitor
};


