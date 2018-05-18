#pragma once

#include <string>
using std::string;
#include <vector>
using std::vector;

#include "globals.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "Windows.h"

#include <complex>
using std::complex;

struct Monitor {
	GLFWmonitor* glfwHandle;
	complex<float> physicalSize,
					screenUnitsSize,
					position;
	float	screenUnitsPerCM;
	string 	name;
	Monitor(GLFWmonitor*& h, std::complex<float> phys, std::complex<float> pixs, std::complex<float> posn, float ppcm, string nom)
		 : glfwHandle {h}, physicalSize {phys}, screenUnitsSize {pixs}, position {posn}, screenUnitsPerCM {ppcm}, name {nom}
		 {};
};

struct GlContext {
	GLFWwindow* pCurrentContext;
	WindowId    currentWindow;

	vector<Window>  windows;
	static vector<Monitor> sMonitors;

	GLuint shaderProgramHandle {0},
		spareHandle {0};


	GLuint linkShadersIntoProgram(GLuint vertex, GLuint fragment);
	GLuint compileShaderFromSourceString(GLenum type, std::string source);

	WindowId 	createWindow(complex<float> center);
	GLFWwindow*	setupFirstContext();
	GLFWwindow* setupSharedContext();

	static void getMonitorsInfo();
	static void monitor_callback(GLFWmonitor* monitor, int event);

	WindowId lookupWindow(GLFWwindow* pWin);
    
//	void enlargeBuffer(GLenum target, size_t oldSize, size_t newSize);

	bool checkglerror(int err, int errnum, string errname, string label);
	void check_gl_errors(string label);
	void check_gl_errors();
	void changeCurrentContext(GLFWwindow *pWin);
	void changeWindow(WindowId win);
};

	bool checkglerror(int err, int errnum, string errname, string label);
	void check_gl_errors(string label);
	void check_gl_errors();

