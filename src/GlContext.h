#pragma once

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "globals.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "include/glm/glm.hpp"
using glm::mat4;
#include "include/glm/gtc/type_ptr.hpp"

#include "Window.h"

#include <algorithm>
#include <functional>


struct Monitor {
	GLFWmonitor* glfwHandle;
	Point physicalSize,
			screenUnitsSize,
			position;
	float	screenUnitsPerCM;
	string 	name;
	Monitor(GLFWmonitor*& h, std::complex<float> phys, std::complex<float> pixs, std::complex<float> posn, float ppcm, string nom)
		 : glfwHandle {h}, physicalSize {phys}, screenUnitsSize {pixs}, position {posn}, screenUnitsPerCM {ppcm}, name {nom}
		 {};
	void print() {
		cout << ::x(screenUnitsSize) <<" x "<< ::y(screenUnitsSize) << " screen unit, " <<
			::x(physicalSize) << " cm x "<<::y(physicalSize)<<" cm monitor found, named \""<<name<<"\", positioned at "<<::x(position)<<", "<<::y(position)<<"\n";
	}
};

/*
drawing order should be front to back except for text:

panning bar bubbles
panning bar background

[menu, menu text]
help line

bubbles contents background
background
overlays
text
*/

enum struct Layer {
menu, 
helpLine, 
bubbleContentsOverlay, 
bubbleContentsBackground, 
bubbleHalo, 
panningBarWindowOutline, 
panningBarBubble, 
panningBar, 
background
};

class GlContext {
	GLFWwindow* pCurrentContext;
	WindowId    myCurrentWindow;

	vector<Window>  myWindows;
	vector<Monitor> myMonitors;



	GLuint linkShadersIntoProgram(GLuint vertex, GLuint fragment);
	GLuint compileShaderFromSourceString(GLenum type, std::string source);

	int 	myTransformationUniform {-1};
	GLuint	myShaderProgramHandle {0};


    
//	void enlargeBuffer(GLenum target, size_t oldSize, size_t newSize);

	bool checkglerror(int err, int errnum, string errname, string label);
	void check_gl_errors(string label);
	void check_gl_errors();

	static void drawCurvedOutlineCorner(Point center, Point xAxis, Point yAxis, float innerRadius, float outerRadius, size_t steps, std::function<void(Point v, float io)> func, float startAngle, float stopAngle);
	static void drawCurvedOutlineSide(PointD center, PointD xAxis, PointD yAxis, Point innerEdgeStart, float innerEdgeSide, std::function<void(Point v, float io)> func, size_t steps, double distToCenter, double radius, double startAngle);

public:
	GLuint shaderHandle() {return myShaderProgramHandle;};
	GLuint spareHandle {0};

	GLFWwindow*	initializeFirstContext();
	GLFWwindow* setupSharedContext();

	WindowId createWindow(complex<float> center);
	WindowId createWindow(WindowId parent);
	GLFWwindow* currentContext() {return pCurrentContext;};
	void     changeCurrentContext(GLFWwindow *pWin);
	void     changeWindow(WindowId win);
	bool     isCurrentWindow(WindowId id) {return myWindows[id].glfwHandle() != pCurrentContext;};
	WindowId currentWindowId() {return myCurrentWindow;};
	Window   &currentWindow () {return myWindows[myCurrentWindow];};
	Window   &lookupWindow(GLFWwindow* pWin);

	int    windowCount() {return myWindows.size();};
	Window &firstWindow() {return myWindows[0];};
	Window &window(WindowId win) {return myWindows[win];};
	Window &window(GLFWwindow* pWin) {return myWindows[(WindowId) glfwGetWindowUserPointer(pWin)];};
	
	Point getMonitorsInfo();
	void  windowMoved(GLFWwindow* pWin, int xpos, int ypos, RedrawRequests &myRedrawQueue);
	void  swapBuffers() {glfwSwapBuffers(pCurrentContext);};

	void setMatrix(mat4& m);
	static void drawCurvedOutline(float leftX, float topY, float rightX, float bottomY, float innerCornerRadius, float outerCornerRadius, std::function<void(Point v, float io)> vertexAccumulatorFunction, size_t cornerSteps, size_t sideSteps = 0);
	void forEachWindow(std::function<void(Window& win)>);
};

	bool checkglerror(int err, int errnum, string errname, string label);
	void check_gl_errors(string label);
	void check_gl_errors();

