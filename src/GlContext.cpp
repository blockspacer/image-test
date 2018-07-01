#include "GlContext.h"


float GlContext::getLayerValue(Layer n) {
	return (float) n / (float) Layer::maxLayer;
}

// bubble_shader.vert needs to know these
void GlContext::showLayerValues() {
	cout<<"pb bub "                 << getLayerValue(Layer::PB_bubble)  <<endl;
	cout<<"bub halo "            << getLayerValue(Layer::bubbleHalo) <<endl;
 	cout<<"bub halo highlighted "<< getLayerValue(Layer::bubbleHaloHighlighted) <<endl;

}

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
		exit(-1);
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











#include <cmath>
// the lambda is called with the coords of the next point, and whether it's on the inside (0) or outside (1) of the curve
// the arc is drawing starting near the x-axis and going anticlockwise
void GlContext::drawCurvedOutlineCorner(Point center, Point xAxis, Point yAxis, float innerRadius, float outerRadius, size_t steps, std::function<void(Point v, float io)> func, float startAngle = 0.0f, float stopAngle = 0.0f) {

	float innerAngleStep = PI / (2.0f * steps + 2)
		, outerAngleStep = (PI / 2 - stopAngle - startAngle) / (steps + 1)
		//(stopAngle - startAngle) / (steps + 1)
		, innerAngle = 0.0f
		, outerAngle = startAngle

	;

	Point p = center + innerRadius * xAxis;
	func(p, 0.0f);
	float a = startAngle;

	for (size_t i = 0; i <= steps; ++i) {
		func(center + outerRadius * (float(sin(outerAngle))*yAxis + float(cos(outerAngle))*xAxis), 1.0f);
		innerAngle += innerAngleStep;
		func(center + innerRadius * (float(sin(innerAngle))*yAxis + float(cos(innerAngle))*xAxis), 0.0f);
		outerAngle += outerAngleStep;
	}

	func(center + outerRadius * (float(sin(outerAngle))*yAxis + float(cos(outerAngle))*xAxis), 1.0f);
}

void GlContext::drawCurvedOutlineSide(PointD center, PointD xAxis, PointD yAxis, Point innerEdgeStart, float innerEdgeSide, std::function<void(Point v, float io)> func, size_t steps = 0, double distToCenter=0.0, double radius=0.0, double startAngle = 0.0) {

	if (! steps) return;

	double angle     = -startAngle
		,  angleStep = (2*startAngle) / (steps+1)
	;

	Point innerStep {::x(xAxis), ::y(xAxis)};
	innerStep *= (innerEdgeSide  / (steps + 1));

	Point inner = innerEdgeStart;

	for (size_t i = 0; i < steps; i++) {
		inner += innerStep;
		func(inner, 0.0f);

		angle += angleStep;
		PointD p = center + xAxis * (radius * sin(angle)) + yAxis * (radius * cos(angle));
		func(Point(float(::x(p)), float(::y(p))), 1.0f);
	}
}

void GlContext::drawCurvedOutline(float leftX, float topY, float rightX, float bottomY, float innerCornerRadius, float outerCornerRadius, std::function<void(Point v, float io)> vertexAccumulatorFunction, size_t cornerSteps, size_t sideSteps) {

	float &l = leftX, &t = topY, &r = rightX, &b = bottomY, &in = innerCornerRadius, &out = outerCornerRadius;
	std::function<void(Point v, float io)> &func = vertexAccumulatorFunction;

	if (r-l<2*in) r = l + 2*in;
	if (b-t<2*in) b = t + 2*in;

	float margin = 1.5; // times the corner circle radius
	margin = fmax(1.01, margin);

	float  topLength = 0.0f, sideLength = 0.0f, topAngle = 0.0f, sideAngle = 0.0f;
	double topRadius = 0.0, sideRadius = 0.0, topCenterDist = 0.0, sideCenterDist = 0.0;

	if (sideSteps) {
		 topLength = (r-l-2*in);
		sideLength = (b-t-2*in);

		 topAngle = 2 * atan( ( topLength / (2 * out * (margin - 1))) );
		sideAngle = 2 * atan( (sideLength / (2 * out * (margin - 1))) );

		if  (topAngle > PI/2)  topAngle = PI - topAngle;
		if (sideAngle > PI/2) sideAngle = PI - sideAngle;

		 topAngle = fmin( topAngle, PI/4);
		sideAngle = fmin(sideAngle, PI/4);

		 topRadius = out +  (topLength / (2 * sin (topAngle)));
		sideRadius = out + (sideLength / (2 * sin(sideAngle)));

		 topCenterDist =  topLength / (2 * tan (topAngle));
		sideCenterDist = sideLength / (2 * tan(sideAngle));
	}

	const Point up    { 0, -1};
	const Point down  { 0,  1};
	const Point left  {-1,  0};
	const Point right { 1,  0};

	const PointD upD    { 0, -1};
	const PointD downD  { 0,  1};
	const PointD leftD  {-1,  0};
	const PointD rightD { 1,  0};

	func(Point(r,b-in),0.0f); // double first point so it forms an infinitely thin triangle from the last one on the strip (which was drawing the last bubble)
	drawCurvedOutlineCorner(Point(r-in,b-in), right, down, in, out, cornerSteps, func, sideAngle, topAngle);

	drawCurvedOutlineSide(PointD((r+l)/2.0, b-in-topCenterDist), leftD, downD, Point(r-in, b), topLength, func, sideSteps, topCenterDist, topRadius, topAngle);

	drawCurvedOutlineCorner(Point(l+in,b-in), down, left, in, out, cornerSteps, func, topAngle, sideAngle);

	drawCurvedOutlineSide(PointD(l + in + sideCenterDist, (t+b)/2.0), upD, leftD, Point(l, b-in), sideLength, func, sideSteps, sideCenterDist,  sideRadius, sideAngle);

	drawCurvedOutlineCorner(Point(l+in,t+in), left, up, in, out, cornerSteps, func, sideAngle, topAngle);

	drawCurvedOutlineSide(PointD((r+l)/2.0, t+in+ topCenterDist ), rightD, upD, Point(l+in, t), topLength, func, sideSteps, topCenterDist,  topRadius, topAngle);

	drawCurvedOutlineCorner(Point(r-in,t+in), up, right, in, out, cornerSteps, func, topAngle, sideAngle);

	drawCurvedOutlineSide(PointD(r - in - sideCenterDist, (t+b)/2.0), downD, rightD, Point(r, t+in), sideLength, func, sideSteps, sideCenterDist,  sideRadius, sideAngle);

	func(Point(r,b-in),0.0f);
	func(Point(r-in+out*cos(sideAngle),b-in+out*sin(sideAngle)),1.0f);
	func(Point(r-in+out*cos(sideAngle),b-in+out*sin(sideAngle)),1.0f);
}







































// vector<Window>  GlContext::myWindows;
// vector<Monitor> GlContext::myMonitors;

Point GlContext::getMonitorsInfo() {
	cout<<"Getting monitor specs\n";
	int count;
	GLFWmonitor** ms = glfwGetMonitors(&count);
	myMonitors.clear();
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

		myMonitors.emplace_back(ms[i], complex<float>(widthMM / 10.0f, heightMM/10.0f),
			complex<float>(w, h),
			complex<float>(posX, posY),
			(10.0f * w) / widthMM,
			name);
		cout<< myMonitors[myMonitors.size()-1].screenUnitsPerCM <<endl;
	}

	sort(myMonitors.begin(), myMonitors.end(), [](Monitor a, Monitor b) {
		return ::x(a.position) < ::x(b.position);
	});

	Point largestExtent {0,0};
	for (int i=0; i<count; ++i) {
//		myMonitors[i].print();
		if (::x(myMonitors[i].physicalSize) > ::x(largestExtent))
			largestExtent.real(::x(myMonitors[i].physicalSize));
		if (::y(myMonitors[i].physicalSize) > ::y(largestExtent))
			largestExtent.imag(::y(myMonitors[i].physicalSize));
	}


	return largestExtent;
//	setWorkspaceSize
}

// see if window is now over a different resolution monitor to what it was before
	// if so, redraw it
	// and all other myWindows' panning bars
void GlContext::windowMoved(GLFWwindow* pWin, int xpos, int ypos, RedrawRequests &redrawQueue) {
	Window &win = window(pWin);

	float bestSUpCM = 0.0f;

	for (const auto &mon : myMonitors) {
		// cout<<"\nxpos "<<xpos + win.screenunitWidth()/2<<endl;
		// cout<<"wppp "<<::x(mon.position) + ::x(mon.screenUnitsSize)<<endl;
		// cout<<"rdpi "<<mon.screenUnitsPerCM<<endl;
		if (::x(mon.position) <= xpos + win.screenunitWidth()/2 
			&& xpos + win.screenunitWidth()/2 < ::x(mon.position) + ::x(mon.screenUnitsSize)
			&& ::y(mon.position) <= ypos + win.screenunitHeight()/2 
			&& ypos + win.screenunitHeight()/2 < ::y(mon.position) + ::y(mon.screenUnitsSize)
			) {
			bestSUpCM = mon.screenUnitsPerCM;
		}
	}

	if (bestSUpCM != 0.0f && win.screenunitsPerCM() != bestSUpCM) {
		win.setScreenunitsPerCM(bestSUpCM);

		redrawQueue.redrawAllWindows();
		// todo: mark window for redraw -- it's the same screenunit size, but can see more (or less) of the woskspace
		// mark all panning bars for redraw

	}
}

Window &GlContext::lookupWindow(GLFWwindow* pWin) {
	return (myWindows[size_t(glfwGetWindowUserPointer(pWin))]);
}

void GlContext::forEachWindow(std::function<void(Window& win)> callMe) {
	for (auto win : myWindows)
		callMe(win);
}











void GlContext::changeCurrentContext(GLFWwindow *pWin) {
	if (pCurrentContext == pWin)
		return;
	pCurrentContext = pWin;
	glfwMakeContextCurrent(pCurrentContext);
	myCurrentWindow = (WindowId) glfwGetWindowUserPointer(pWin);

//	cout << "new context context: "<<pWin<<", window: "<<myCurrentWindow<<endl;
}

void GlContext::changeWindow(WindowId id) {
	myCurrentWindow = id;
	changeCurrentContext(myWindows[id].glfwHandle());
}

WindowId GlContext::createWindow(complex<float> center) {
	WindowId newWin {0};

	// first window ever
	if (myWindows.size() == 0) {
		myWindows.emplace_back(newWin);
		myWindows[0].setGlfwHandle(initializeFirstContext());
	}
	// re-use an old slot
	else {
		for (newWin = 0; newWin < myWindows.size(); newWin++) {
			if (myWindows[newWin].isUnused()) {
				myWindows[newWin].setGlfwHandle(setupSharedContext());
				break;
			}
		}
	}

	// add a window to the list
	if (newWin == myWindows.size()) {
		myWindows.emplace_back(newWin);
		myWindows[newWin].setGlfwHandle(setupSharedContext());
	}

	myCurrentWindow = newWin;

	glUseProgram(myShaderProgramHandle);

	glfwSwapInterval(1);

	myWindows[newWin].setupVAOs();
	glfwSetWindowUserPointer(myWindows[newWin].glfwHandle(), (void *) newWin);

	int w, h;
	glfwGetFramebufferSize(myWindows[newWin].glfwHandle(), &w, &h);
	myWindows[newWin].setPixelSize(w, h);

	glfwGetWindowSize(myWindows[newWin].glfwHandle(), &w, &h);
	myWindows[newWin].setScreenunitSize(w, h);


	glEnable(GL_DEPTH_TEST);  
	
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
	WindowId newId = createWindow(myWindows[parent].center());

	int xpos, ypos;
	glfwGetWindowPos(myWindows[newId].glfwHandle(), &xpos, &ypos);
	glfwSetWindowPos(myWindows[newId].glfwHandle(), xpos+50, ypos+50);

	// todo: if new one would be off screen then jump back up to the top

	return newId;
}

GLFWwindow* GlContext::setupSharedContext() {
	GLFWwindow *pWin;
	for (int i = 0; i < myWindows.size(); i++) {
		if ( myWindows[i].inUse() ) {
			pWin = myWindows[i].glfwHandle();
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
		EM_ASM({var el = document.getElementById('message');el.parentNode.removeChild(el);}, 0);
	#else
	    int width = 1024,
             height = 633;
	#endif




	pCurrentContext = glfwCreateWindow(width, height, "image test", NULL, NULL);

    if (!pCurrentContext) {
        complain("glfwCreateWindow() failed");
        exit(-1);
    }

	#ifdef WEB
		// glClearColor(1,1,1,1);
		// glClear(GL_COLOR_BUFFER_BIT);
		 
		// // Turn off rendering to alpha
		// glColorMask(true, true, true, false);
    #endif

    glfwMakeContextCurrent(pCurrentContext);
   	
   	glfwSetWindowUserPointer(pCurrentContext, (void *) 0);



	glewExperimental = true; // Needed for core profile or something
	if (glewInit() != GLEW_OK) {
		complain("Failed to initialize GLEW");
		glfwTerminate();
	}

	#ifdef NATIVE
	   	getMonitorsInfo();
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