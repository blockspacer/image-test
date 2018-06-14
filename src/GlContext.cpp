#include "GlContext.h"

using std::endl;

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

bool GlContext::checkglerror(int err, int errnum, string errname, string label) {
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

void GlContext::check_gl_errors() {check_gl_errors("unknown");}

void GlContext::check_gl_errors(string label) {
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
	    	#ifdef REPORT
				string complaint = "No Errors at marker ";
				complaint.append(label);
				complain(complaint);
	    	#endif
	    }
	   #endif
}
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

void check_gl_errors() {check_gl_errors("unknown");}

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
	    	#ifdef REPORT
				string complaint = "No Errors at marker ";
				complaint.append(label);
				complain(complaint);
	    	#endif
	    }
	   #endif
}



void glfw_error_callback(int error, const char* description) {
	cout << BOLD "GLFW error " << error << NORMAL " : " << description << std::endl;
}


void MessageCallback( GLenum source,
                 GLenum type,
                 GLuint id,
                 GLenum severity,
                 GLsizei length,
                 const GLchar* message,
                 const void* userParam )
{
	if (type != GL_DEBUG_TYPE_ERROR)
		return;
  fprintf( stderr, "GL CALLBACK: %s type = 0x%x, severity = 0x%x, message = %s\n",
           ( type == GL_DEBUG_TYPE_ERROR ? "** GL ERROR **" : "" ),
            type, severity, message );
}

// During init, enable debug output











vector<Window>  GlContext::windows;
vector<Monitor> GlContext::sMonitors;

Point GlContext::getMonitorsInfo() {
	cout<<"Getting monitor specs\n";
	int count;
	GLFWmonitor** ms = glfwGetMonitors(&count);
	sMonitors.clear();
	for (int i=0; i<count; ++i) {
		const GLFWvidmode* mode = glfwGetVideoMode(ms[i]);
		int w = mode->width;
		int h = mode->height;
		int r = mode->refreshRate;
		int posX, posY;
		string name;
		name = glfwGetMonitorName(ms[i]);
		glfwGetMonitorPos(ms[i], &posX, &posY);
		int widthMM, heightMM;
		glfwGetMonitorPhysicalSize(ms[i], &widthMM, &heightMM);
				
		sMonitors.emplace_back(ms[i], complex<float>(widthMM,heightMM),
			complex<float>(w/10.0f, h/10.0f),
			complex<float>(posX, posY),
			(10.0f * w) / widthMM,
			name);
		cout<< sMonitors[sMonitors.size()-1].screenUnitsPerCM <<endl;
	}

	sort(sMonitors.begin(), sMonitors.end(), [](Monitor a, Monitor b) {
		return ::x(a.position) < ::x(b.position);
	});

	Point largestExtent {0,0};
	for (int i=0; i<count; ++i) {
//		sMonitors[i].print();
		if (::x(sMonitors[i].physicalSize) > ::x(largestExtent))
			largestExtent.real(::x(sMonitors[i].physicalSize));
		if (::y(sMonitors[i].physicalSize) > ::y(largestExtent))
			largestExtent.imag(::y(sMonitors[i].physicalSize));
	}


	return largestExtent;
//	setWorkspaceSize
}


Window &GlContext::lookupWindow(GLFWwindow* pWin) {
	return (windows[size_t(glfwGetWindowUserPointer(pWin))]);
}






void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
cout<<".\n";
	if (key==GLFW_KEY_F ) {
#ifdef WEB
		emscripten_resume_main_loop();
#endif
		cout<<"F\n";
	}
}









void GlContext::changeCurrentContext(GLFWwindow *pWin) {
	if (pWin != pCurrentContext) {
		pCurrentContext = pWin;
		glfwMakeContextCurrent(pCurrentContext);
	}
}

void GlContext::changeWindow(WindowId id) {
	myCurrentWindow = id;
	changeCurrentContext(windows[id].glfwHandle);
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{

	cout<<"New framebuffer size "<<width<<" x "<<height<<" pixels\n";

//    glViewport(0, 0, width, height);
}

WindowId GlContext::createWindow(complex<float> center) {
	WindowId newWin {0};

	// first window ever
	if (windows.size() == 0) {
		windows.emplace_back(newWin);
		windows[0].glfwHandle = initializeFirstContext();
	}
	// re-use an old slot
	else {
		for (newWin = 0; newWin < windows.size(); newWin++) {
			if (windows[newWin].unused) {
				windows[newWin].unused = false;
				windows[newWin].glfwHandle = setupSharedContext();
				break;
			}
		}
	}

	// add a window to the list
	if (newWin == windows.size()) {
		windows.emplace_back(newWin);
		windows[newWin].glfwHandle = setupSharedContext();
	}

	myCurrentWindow = newWin;

	glUseProgram(myShaderProgramHandle);

	glfwSwapInterval(1);

glfwSetFramebufferSizeCallback(windows[newWin].glfwHandle, framebuffer_size_callback);
glfwSetKeyCallback(windows[newWin].glfwHandle, key_callback);
glViewport(0,0, 1000, 1000);


	windows[newWin].setupVAOs();
	glfwSetWindowUserPointer(windows[newWin].glfwHandle, (void *) newWin);

	#ifdef __APPLE__
		glEnable(GL_PRIMITIVE_RESTART);
		glPrimitiveRestartIndex(0xffff);
	#else
		#ifndef __EMSCRIPTEN__
			glEnable(GL_PRIMITIVE_RESTART_FIXED_INDEX);
		#endif
	#endif

	return newWin;
}

WindowId GlContext::createWindow(WindowId parent) {
	WindowId newId = createWindow(windows[parent].center());
	return newId;
}

GLFWwindow* GlContext::setupSharedContext() {
	GLFWwindow *pWin;
	for (int i = 0; i < windows.size(); i++) {
		if ( ! windows[i].unused) {
			pWin = windows[i].glfwHandle;
			break;
		}
	}

    int width = 1024,
		height = 633;

	pCurrentContext = glfwCreateWindow(width, height, "image test", NULL, pWin);

	if (! pCurrentContext) {
		complain("Unable to create a new window with shared graphics context");
		exit(-1);
	}

    glfwMakeContextCurrent(pCurrentContext);
		
	return pCurrentContext;
}


GLFWwindow* GlContext::initializeFirstContext() {
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




	pCurrentContext = glfwCreateWindow(width, height, "image test", NULL, NULL);

    if (!pCurrentContext) {
        complain("glfwCreateWindow() failed");
        exit(-1);
    }

    glfwMakeContextCurrent(pCurrentContext);
   	
   	glfwSetWindowUserPointer(pCurrentContext, (void *) 0);

   	getMonitorsInfo();


	glewExperimental = true; // Needed for core profile or something
	if (glewInit() != GLEW_OK) {
		complain("Failed to initialize GLEW");
		glfwTerminate();
	}

	#ifdef NATIVE
		#ifdef DEBUG
			#ifndef __APPLE__
				glEnable              ( GL_DEBUG_OUTPUT );
				glDebugMessageCallback( MessageCallback, 0 );
			#endif
		#endif
	#endif

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
		#include "shaders/bubble_shader.vert"
	;

	const char * fragment_shader =
		#include "shaders/bubble_shader.frag"
	;

	vert.append(vertex_shader);
	frag.append(fragment_shader);

	GLuint fragshader = compileShaderFromSourceString(GL_FRAGMENT_SHADER, frag);
	GLuint vertshader = compileShaderFromSourceString(GL_VERTEX_SHADER,   vert);

	myShaderProgramHandle = linkShadersIntoProgram(vertshader, fragshader);

	myTransformationUniform = glGetUniformLocation(myShaderProgramHandle, "transformation");	

//	glValidateProgram(myShaderProgramHandle);



//	glEnable(GL_CULL_FACE);

	if (GLEW_ARB_debug_output){ 
		cout<<"ARB_debug_output yes"<<'\n';
	}
	else
		cout<<"ARB_debug_output no"<<'\n';


	return pCurrentContext;
}










void GlContext::setMatrix(mat4& m) {
	glUniformMatrix4fv(myTransformationUniform, 1, GL_FALSE, glm::value_ptr(m));
}