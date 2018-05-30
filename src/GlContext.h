#pragma once

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "globals.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

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

class GlContext {
	GLFWwindow* pCurrentContext;
	WindowId    myCurrentWindow;

	static vector<Window>  windows;
	static vector<Monitor> sMonitors;



	GLuint linkShadersIntoProgram(GLuint vertex, GLuint fragment);
	GLuint compileShaderFromSourceString(GLenum type, std::string source);


	static void getMonitorsInfo();
	static void monitor_callback(GLFWmonitor* monitor, int event);

    
//	void enlargeBuffer(GLenum target, size_t oldSize, size_t newSize);

	bool checkglerror(int err, int errnum, string errname, string label);
	void check_gl_errors(string label);
	void check_gl_errors();
	void changeCurrentContext(GLFWwindow *pWin);

public:
	GLuint shaderProgramHandle {0};
	GLuint spareHandle {0};

	GLFWwindow*	setupFirstContext();
	GLFWwindow* setupSharedContext();

	WindowId createWindow(complex<float> center);
	WindowId createWindow(WindowId parent);
	void     changeWindow(WindowId win);
	bool     isCurrentWindow(WindowId id) {return windows[id].glfwHandle != pCurrentContext;};
	WindowId currentWindowId() {return myCurrentWindow;};
	Window   &currentWindow () {return windows[myCurrentWindow];};
	Window   &lookupWindow(GLFWwindow* pWin);

	int    windowCount() {return windows.size();};
	Window &window(WindowId win) {return windows[win];};
	
	void swapBuffers() {glfwSwapBuffers(pCurrentContext);};

};

	bool checkglerror(int err, int errnum, string errname, string label);
	void check_gl_errors(string label);
	void check_gl_errors();

