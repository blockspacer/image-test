#pragma once
 
#include <complex>
using std::complex;

#include "globals.h"
#include "MouseEventInterface.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>


class Window : public MouseEventInterface {
	WindowId    myId;
	GLFWwindow  *myGlfwHandle;
	GLFWmonitor *myGlfwMonitorHandle;
	bool	    unused {false};
	complex<float> topLeftInWorkspaceCM,
				   widthHeightInWorkspaceCM,
				   positionInScreenUnits,
				   mySizeInScreenUnits,
				   sizeInPixels;
	float screenUnitsPerCM;

	bool		iNeedRefresh {true};

public:
	GLuint bubblesVAO {0},
			textVAO {0};
	Window(WindowId id);

	void setupVAOs();

	complex<float> center() {return topLeftInWorkspaceCM + 0.5f * widthHeightInWorkspaceCM;};

	void	setGlfwHandle(GLFWwindow* pWin) {myGlfwHandle = pWin; unused = false;};
	GLFWwindow* glfwHandle() {return myGlfwHandle;};

	bool 	isUnused() {return unused;};
	bool 	inUse()    {return ! unused;};

	bool    needsRefresh()   {return iNeedRefresh;};
	void    markForRefresh() {iNeedRefresh = true ;};
	void    markRefreshed()  {iNeedRefresh = false;};

	bool	screenUnitsSizeChanged(int w, int h);

	WindowId id() {return myId;};

	bool mouseMotion(Point pos, RedrawRequests &redrawReqests);
//	void discoverMonitor
};


