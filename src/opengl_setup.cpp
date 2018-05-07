#include "globals.h"

#include <iostream>
using std::cout;
#include <string>
using std::string;
using std::endl;

#ifdef __APPLE__

#else
#endif
#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include "opengl_setup.h"

void glfw_error_callback(int error, const char* description) {
	cout << BOLD "GLFW error " << error << NORMAL " : " << description << std::endl;
}


GLuint compile_shader_from_source_string(GLenum type, string source) {
	GLuint shader;
	GLint compiled;

	shader = glCreateShader(type);

	if(shader == 0) {
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

GLuint link_shaders_into_program(GLuint vertex, GLuint fragment) {
	
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



GLFWwindow* initialise_glfw_and_compile_shader(ContextData &data) {
	if (!glfwInit()) {
		complain("GLFW init failed :(");
		exit(-1);
	}

	glfwSetErrorCallback(glfw_error_callback);

glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, 1);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	#ifdef __APPLE__
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	#else
		glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
		glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
	#endif

	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);


	#ifdef __EMSCRIPTEN__
		int width = EM_ASM_INT( {return window.innerWidth;}, 0 );
		int height = EM_ASM_INT( {return window.innerHeight;}, 0 );
		EM_ASM({var el = document.getElementById("message");el.parentNode.removeChild(el);}, 0);
	#else
	    int width = 1024,
             height = 633;
	#endif




	GLFWwindow* pWin = glfwCreateWindow(width, height, "image test", NULL, NULL);

    if (!pWin) {
        complain("glfwCreateWindow() failed");
        exit(-1);
    }

    glfwMakeContextCurrent(pWin);
    glfwSwapInterval(1);

	glewExperimental = true; // Needed for core profile or something
	if (glewInit() != GLEW_OK) {
		complain("Failed to initialize GLEW");
		glfwTerminate();
	}


	int major=0; int minor=0;

	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	cout << "OpenGL version " << major << "."<<minor<<"\n";



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
		#include "shader_program.vert"
	;

	const char * fragment_shader =
		#include "shader_program.frag"
	;

	vert.append(vertex_shader);
	frag.append(fragment_shader);

	GLuint fragshader = compile_shader_from_source_string(GL_FRAGMENT_SHADER, frag);
	GLuint vertshader = compile_shader_from_source_string(GL_VERTEX_SHADER,   vert);

	GLuint program = link_shaders_into_program(vertshader, fragshader);

	glUseProgram(program);
//	glEnable(GL_CULL_FACE);


int n = glGetAttribLocation(program, "vertexColor");

	glViewport(0,0, 1000, 1000);
//glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);

if (GLEW_ARB_debug_output){ // Ta-Dah ! 
	cout<<"ARB yes"<<'\n';
}

	data.shaderProgramHandle = program;

	loaddemodata(data);
	return pWin;
}

#include <vector>
using std::vector;
#include <cmath>

#define POINTS 5

vector<GLfloat> drawStar (GLfloat x, GLfloat y, GLfloat r) {
	vector<GLfloat> verts = {x, y, 0.0};
	for (int i=0; i < 12; i++) {
		verts.push_back(x + cos( (2.0f* PI / 12.0f) * i )*r);
		verts.push_back(y + sin( (2.0f* PI / 12.0f) * i )*r);
		verts.push_back(0.0);
	}
	return verts;
}

void buildRhombicFriend() {
//output format x,y,z (half floats)
	GLfloat vertices[] = {
	 0.0f,  0.0f,  2.0f, 

	 1.0f,  0.0f,  1.0f, 
	 0.0f,  1.0f,  1.0f, 
	-1.0f,  0.0f,  1.0f, 
	 0.0f, -1.0f,  1.0f, 

	 1.0f, -1.0f,  0.0f, 
	 1.0f,  1.0f,  0.0f, 
	-1.0f,  1.0f,  0.0f, 
	-1.0f, -1.0f,  0.0f, 

	 1.0f,  0.0f, -1.0f, 
	 0.0f,  1.0f, -1.0f, 
	-1.0f,  0.0f, -1.0f, 
	 0.0f, -1.0f, -1.0f, 

	 0.0f,  0.0f, -2.0f
	};

	size_t faces[] = {
		0,1,6,2,
		0,2,7,3,
		0,3,8,4,
		0,4,5,1,

		

	}
}

class Vertex {
public:
	Vertex(float _x, float _y, float _z, uint8_t _r, uint8_t _g, uint8_t _b) : x(_x), y(_y), z(_z), r(_r), g(_g), b(_b), a(245)
	 { };
	float x,y,z;
	uint8_t r,g,b,a;
};

void loaddemodata(ContextData &data) {



	GLfloat verts[] = {0.0f, 0.5f, 0.0f,
		-0.5f, -0.5f, 0.0f,
		 0.4f, -0.5f, 0.0f};

	GLushort indices[3] = {0,1,2};

	vector<Vertex> vertx = {Vertex(0.0f, 0.5f, 0.0f, uint8_t(200), uint8_t(200),uint8_t(0)), 
		Vertex(-0.5f, -0.5f, 0.0f, 200, 0,200),
		Vertex(0.4f, -0.5f, 0.0f, 0, 200,200)};
	

	vector<GLfloat> starVerts = drawStar(0.0, 0.0, 0.5);
	GLushort starInds[13] = {0,1,2,3,4,5,5,7,8,9,10,11,12};

	glGenVertexArrays(1, &data.triangleVertexArray);
	glBindVertexArray(    data.triangleVertexArray);

		glGenBuffers(1,              &data.triangleVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, data.triangleVertexBuffer);
uint8_t *bup = (uint8_t*)vertx.data();
bup[12] = 0;
		glBufferData(GL_ARRAY_BUFFER, 4*3*sizeof(GLfloat), vertx.data(), GL_STATIC_DRAW);

		glGenBuffers(1,                      &data.triangleElementIndices);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.triangleElementIndices);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*3, indices, GL_STATIC_DRAW);

		glEnableVertexAttribArray(0);
		glEnableVertexAttribArray(1);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 4*sizeof(GLfloat), (const void*) 0);
		glVertexAttribPointer(1, 4, GL_UNSIGNED_BYTE,  GL_TRUE,  4*sizeof(GLfloat), (const void*) 12);

cout<<"bype "<<std::to_string(bup[12])<<endl;
cout<<"bype "<<std::to_string(bup[13])<<endl;
cout<<"bype "<<std::to_string(bup[14])<<endl;
cout<<"bype "<<std::to_string(bup[15])<<endl;

	// glGenVertexArrays(1, &data.starVertexArray);
	// glBindVertexArray(    data.starVertexArray);

	// 	glGenBuffers(1,              &data.starVertexBuffer);
	// 	glBindBuffer(GL_ARRAY_BUFFER, data.starVertexBuffer);
	// 	glBufferData(GL_ARRAY_BUFFER, 13*3*sizeof(GLfloat), starVerts.data(), GL_STATIC_DRAW);

	// 	glGenBuffers(1,                      &data.starElementIndices);
	// 	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, data.starElementIndices);
	// 	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*13, starInds, GL_STATIC_DRAW);

	// 	glEnableVertexAttribArray(0);

	// 	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3*sizeof(GLfloat), (const void*) 0);

	glBindVertexArray(0);
}

// int compile_shader() {
// 	// copied this from http://www.opengl-tutorial.org/


// }

bool checkglerror(int err, int errnum, string errname, string label) {
    if (err == errnum) {
        cout << "ERROR at label << " << label << ">:" << errnum << " " << errname << endl;
        return true;
    } else
        return false;
}

void check_gl_errors(string label) {
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
            cout<<"UNKNOWN ERROR number "<< err <<" at marker <"<<label<<"> (see https://www.opengl.org/wiki/OpenGL_Error )"<<endl;
    }

    if (!errs)
        cout << "No Errors at marker <"<<label<<">"<<endl;
}

void check_gl_errors() {check_gl_errors("unknown");}
