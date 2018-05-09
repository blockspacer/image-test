#pragma once

#include <string>
using std::string;

#include "globals.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <vector>
using std::vector;

struct GlContext {
	//vector<Window> mWindows;

	GLuint shaderProgramHandle {0},

		triangleVertexArray    {0},
		triangleVertexBuffer   {0},
		triangleElementIndices {0},
	
		starVertexArray    {0},
		starVertexBuffer   {0},
		starElementIndices {0},


		bubblesVertexBufferHandle      {0},
		bubblesVertexIndexBufferHandle {0},
		bubblesDataTextureHandle       {0},
		bubblesVAO                     {0},

		spareHandle {0};

	size_t bubbleBufferContentsCount {0};

	int positionAttribLoc,
		bubbleIndexAttribLoc
		;

	void setup();
	GLuint linkShadersIntoProgram(GLuint vertex, GLuint fragment);
	GLuint compileShaderFromSourceString(GLenum type, std::string source);
	GLFWwindow* setupSharedContext(GLFWwindow *pWin);
	
	void enlargeBuffer(GLenum target, size_t oldSize, size_t newSize);

bool checkglerror(int err, int errnum, string errname, string label) {
	if (err == errnum) {
		string complaint = "ERROR at label ";
		complaint.append(label);
		complaint.append(" : ");
		complaint.append(errname);
		complain(complaint);

		return true;
	} else
		return false;
}

void check_gl_errors(string label) {
	#ifdef DEBUG
		bool errs = false;
		int err = -1;

		while ((err = glGetError()) != GL_NO_ERROR){
			errs=true;
			if (true==checkglerror(err,GL_INVALID_ENUM,"GL_INVALID_ENUM",label))
				continue;
			else if (checkglerror(err,GL_INVALID_VALUE,"GL_INVALID_VALUE",label))
				continue;
			else if (checkglerror(err,GL_INVALID_OPERATION,"GL_INVALID_OPERATION",label))
				continue;
			else 
				complain("UNKNOWN ERROR number ");//, err <<" at marker <"<<label<<"> (see https://www.opengl.org/wiki/OpenGL_Error )"<<endl;
		}

		if (!errs) {
			// string complaint = "No Errors at label ";
			// complaint.append(label);
			// complain(complaint);
		}
	#endif
}

void check_gl_errors() {check_gl_errors("unknown");}
};
