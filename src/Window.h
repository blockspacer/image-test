#pragma once
 
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <complex>
using std::complex;

#include "globals.h"
#include "MouseEventInterface.h"
#include "Workspace.h"



class Window : public MouseEventInterface {
	WindowId    myId;
	GLFWwindow  *myGlfwHandle;
	GLFWmonitor *myGlfwMonitorHandle;
	bool	    unused {false};
	complex<float> topLeftInWorkspaceCM
				,   widthHeightInWorkspaceCM
				,   positionInScreenUnits
				,   myScreenunitSize
				,   myPixelSize
	;
	float screenUnitsPerCM;

	bool		iNeedRefresh {true};

public:
	GLuint bubblesVAO {0}
		,	textVAO {0}
		,	backgroundVAO {0}
		,	panningBarBubbleVAO {0}
	;
	Window(WindowId id);

	void setupVAOs();

	complex<float> center() {return topLeftInWorkspaceCM + 0.5f * widthHeightInWorkspaceCM;};

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

	Point 	topLeft()     { return topLeftInWorkspaceCM ;};
	Point 	bottomRight() { return topLeftInWorkspaceCM + widthHeightInWorkspaceCM ;};

	WindowId id() {return myId;};

	bool mouseMotion(Point pos, RedrawRequests &redrawReqests);
//	void discoverMonitor
};


