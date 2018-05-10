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


struct GlContext {
	vector<Window> windows;
	GLFWwindow* pCurrentContext;

	GLuint shaderProgramHandle {0},


		spareHandle {0};


	GLuint linkShadersIntoProgram(GLuint vertex, GLuint fragment);
	GLuint compileShaderFromSourceString(GLenum type, std::string source);

	WindowId 	createWindow(complex<float> center);
	GLFWwindow*	setupFirstContext();
    GLFWwindow* setupSharedContext();

    WindowId lookupWindow(GLFWwindow* pWin);
    
    void enlargeBuffer(GLenum target, size_t oldSize, size_t newSize);

	bool checkglerror(int err, int errnum, string errname, string label);
	void check_gl_errors(string label);
	void check_gl_errors();
	void changeCurrentContext(GLFWwindow *pWin);
	void changeWindow(WindowId win);
};

	bool checkglerror(int err, int errnum, string errname, string label);
	void check_gl_errors(string label);
	void check_gl_errors();
