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

	static vector<Window>  windows;
	static vector<Monitor> sMonitors;



	GLuint linkShadersIntoProgram(GLuint vertex, GLuint fragment);
	GLuint compileShaderFromSourceString(GLenum type, std::string source);

	int 	myTransformationUniform {-1};
	GLuint	myShaderProgramHandle {0};


    
//	void enlargeBuffer(GLenum target, size_t oldSize, size_t newSize);

	bool checkglerror(int err, int errnum, string errname, string label);
	void check_gl_errors(string label);
	void check_gl_errors();

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
	bool     isCurrentWindow(WindowId id) {return windows[id].glfwHandle != pCurrentContext;};
	WindowId currentWindowId() {return myCurrentWindow;};
	Window   &currentWindow () {return windows[myCurrentWindow];};
	Window   &lookupWindow(GLFWwindow* pWin);

	int    windowCount() {return windows.size();};
	Window &window(WindowId win) {return windows[win];};
	
	static Point getMonitorsInfo();
	void swapBuffers() {glfwSwapBuffers(pCurrentContext);};

	void setMatrix(mat4& m);
};

	bool checkglerror(int err, int errnum, string errname, string label);
	void check_gl_errors(string label);
	void check_gl_errors();

