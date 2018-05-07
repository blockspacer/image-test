#include "GlContext.h"

#include "globals.h"

GLuint GlContext::compileShaderFromSourceString(GLenum type, std::string source) {
	GLuint shader;
	GLint compiled;

	shader = glCreateShader(type);

	if (shader == 0) {
		complain("glCreateShader failed");
		return 0;
	}

	const char *src = source.c_str();
	glShaderSource(shader, 1, &src, NULL);

	glCompileShader(shader);

	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled) {
		GLint infoLen = 0;
		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) {
			char* infoLog = (char*) malloc(sizeof(char) * infoLen);

			glGetShaderInfoLog(shader, infoLen, NULL, infoLog);
			complain("Error compiling shader : ", infoLog);
			free(infoLog);
		}
		glDeleteShader(shader);
		return 0;
	}
	return shader;
}

GLuint GlContext::linkShadersIntoProgram(GLuint vertex, GLuint fragment) {
	
	GLuint program = glCreateProgram();

	if (program == 0) {
		complain("glCreateProgram() failed");
		return 0;
	}

	glAttachShader(program, vertex);
	glAttachShader(program, fragment);

	glLinkProgram(program);

	GLint linked;
	glGetProgramiv(program, GL_LINK_STATUS, &linked);

	if (!linked) {
		GLint infoLen = 0;
		glGetShaderiv(program, GL_INFO_LOG_LENGTH, &infoLen);

		if (infoLen > 1) {
			char* infoLog = (char*) malloc(sizeof(char) * infoLen);

			glGetProgramInfoLog(program, infoLen, NULL, infoLog);
			complain("Error linking shader program : ", infoLog);
			free(infoLog);
		}
		glDeleteProgram(program);
		return 0;
	}
	return program;
}

void GlContext::setup() {
	glfwSwapInterval(1);

	#ifdef __APPLE__
		string vert = "#version 330\n";
		string frag = "#version 330\n";
	#else
		string vert = "#version 300 es\n";
		string frag = "#version 300 es\n";
	#endif
	
	// https://stackoverflow.com/questions/1246301/c-c-can-you-include-a-file-into-a-string-literal
	// https://gcc.gnu.org/onlinedocs/cpp/Stringizing.html#Stringizing
	#define STRINGIFY(x) #x
	// the included files contain the shader source code, wrapped in STRINGIFY( ... )

	const char * vertex_shader =
		#include "shaders/bubble_shader.vert"
	;

	const char * fragment_shader =
		#include "shaders/bubble_shader.frag"
	;

	vert.append(vertex_shader);
	frag.append(fragment_shader);

	GLuint fragshader = compileShaderFromSourceString(GL_FRAGMENT_SHADER, frag);
	GLuint vertshader = compileShaderFromSourceString(GL_VERTEX_SHADER,   vert);

	shaderProgramHandle = linkShadersIntoProgram(vertshader, fragshader);

//	glValidateProgram(shaderProgramHandle);


	glUseProgram(shaderProgramHandle);
//	glEnable(GL_CULL_FACE);


	positionAttrib    = glGetAttribLocation(shaderProgramHandle, "position");
	bubbleIndexAttrib = glGetAttribLocation(shaderProgramHandle, "bubbleId");

	glViewport(0,0, 1000, 1000);
//glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
complain("created context");
	if (GLEW_ARB_debug_output){ // Ta-Dah ! 
		cout<<"ARB yes"<<'\n';
	}

}

