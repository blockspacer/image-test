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

// nb: remember to update constants in vertex shader if these change!
enum struct Layer {
	menu, 
	helpLine, 
	PB_potentialWindowOutline, 
	PB_potentialWindowViewArea, 
	PB_currentWindowOutline,
	PB_currentWindowViewArea,
	PB_otherWindowOutline,
	PB_otherWindowViewArea,
	PB_bubble,                  // *
	PB_background,
	bubbleContentsOverlay, 
	bubbleHaloHighlighted,      // *
	bubbleHalo,                 // *
	bubbleContentsBackground, 
	panningBar, 
	background,
	maxLayer
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
	void callFromEachContext(std::function<void(Window& win)> callMe);


	static void showLayerValues();
	static float getLayerValue(Layer n);
};

	bool checkglerror(int err, int errnum, string errname, string label);
	void check_gl_errors(string label);
	void check_gl_errors();

struct Vertex {
	float	x, y, z;
	uint8_t	r, g, b, a;
	float	t, v ;
	Vertex(float _x, float _y, float _z, uint8_t _r, uint8_t _g, uint8_t _b, uint8_t _a, float _t, float _v) :
		x {_x}, y {_y}, z {_z}, r {_r}, g {_g}, b {_b}, a {_a}, t {_t}, v {_v} {}

	Vertex(float _x, float _y, float _z, Color c, float _t, float _v=0.0f) :
		x {_x}, y {_y}, z {_z}, r (c.redPremultipliedInt()), g (c.greenPremultipliedInt()), b (c.bluePremultipliedInt()), a (c.alphaInt()), t {_t}, v {_v} {}

	Vertex(float _x, float _y, float _z, float _t, float _v) :
		x {_x}, y {_y}, z {_z}, r {255}, g {255}, b {255}, a {255}, t {_t}, v {_v} {}

	Vertex() {};
	void setColor(Color c) {r = c.redPremultipliedInt(); g = c.greenPremultipliedInt(); b = c.bluePremultipliedInt(); a = 255; c.alphaInt();};
	void setXY(Point p) {x = ::x(p); y = ::y(p);};
	void setXY(float _x, float _y) {x = _x; y = _y;};
	void setXYZ(float _x, float _y, float _z) {x = _x; y = _y; z = _z;};
	void setType(float _t, float _v = 0.0f) {t = _t; v = _v;};

};


