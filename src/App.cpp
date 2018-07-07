#include "App.h"

MouseEvents    App::myMouseHandler;
RedrawRequests App::myRedrawQueue;
PanningBar     App::myPanningBar;
Workspace      App::myWorkspace;
GlContext      App::myGlContext;
TextLayout     App::myText;
Bubbles        App::myBubbles;

void App::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	myMouseHandler.moved(window, xpos, ypos, myGlContext, myWorkspace, myPanningBar, myBubbles, myRedrawQueue);
}

void App::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	myMouseHandler.buttonInput(window, button, action, mods, myGlContext, myPanningBar, myWorkspace, myRedrawQueue);
}

void App::monitorCallback(GLFWmonitor* monitor, int event) {
	Point largestMonitorExtent = myGlContext.getMonitorsInfo();
	#ifdef NATIVE
	#else // guess how big the web user's screen is based on pixel size of the canvas
		Window &win = myGlContext.firstWindow();

		float assumed_dpi = 96.0f
			, dpCM = assumed_dpi / 2.54; // centimetres per inch
		
		int w, h;
		glfwGetFramebufferSize(win.glfwHandle(), &w, &h);
		
		largestMonitorExtent.real(w / dpCM);
		largestMonitorExtent.imag(h / dpCM);
	#endif
	myWorkspace.setSize(largestMonitorExtent);
}

// - may get called when a window moves to a higher dpi screen
// - currently called when a webgl window changes size
void App::framebufferSizeCallback(GLFWwindow* pWin, int w, int h) {
	Window &win = myGlContext.window(pWin);

	win.setPixelSize(w, h);

	// todo what about high-res displays
	#ifdef WEB
		win.setScreenunitSize(w, h);
	#endif
	
	myGlContext.changeCurrentContext(pWin);
	glViewport(0, 0, w, h);
	myRedrawQueue.redrawAllWindows();

	#ifdef WEB
		emscripten_resume_main_loop();
	#endif
}

void App::windowSizeCallback(GLFWwindow* pWin, int w, int h) {
	Window &win = myGlContext.window(pWin);

	win.setScreenunitSize(w, h);

//		myGlContext.changeCurrentContext(pWin);

//		int fbw, fbh;
//		glfwGetFramebufferSize(window, &fbw, &fbh);

//		if (myGlContext.currentWindow().screenUnitsSizeChanged(w,h)) {
//		    glViewport(0, 0, w, h);

//		glViewport(0, 0, width, height);
//		myRedrawQueue.redrawAllWindows();
		
}

void App::webCanvasResize(int w, int h) {
	framebufferSizeCallback(myGlContext.currentContext(), w, h);
}

void App::redrawCallback(GLFWwindow* pWin) {
	draw();};

void App::keyCallback(GLFWwindow* pWin, int key, int scancode, int action, int mods) {
	Window &win = myGlContext.window(pWin);
	if (key==GLFW_KEY_J && action == GLFW_PRESS) {
		WindowId parent = win.id();
		// SHOW_TYPE(parent)
		myRedrawQueue.newWindow(parent);

	#ifdef WEB
			emscripten_resume_main_loop();
	#endif
	
		cout<<"J\n";
	}
	else if (key==GLFW_KEY_Q)
		exit(-1);
}

void App::windowPosCallback(GLFWwindow* pWin, int xpos, int ypos) {
	myGlContext.windowMoved(pWin, xpos, ypos, myRedrawQueue);
}

void App::windowFocusCallback(GLFWwindow* window, int focused)
{
	if (focused)
	{
		cout<<"focused "<<endl;
		// The window gained input focus

		// cout<<GLFW_PRESS<<endl;
		// int s = glfwGetMouseButton 	(window, GLFW_MOUSE_BUTTON_1 );cout<<(int)s<<endl;;
		//     s = glfwGetMouseButton 	(window, GLFW_MOUSE_BUTTON_2 );cout<<(int)s<<endl;;
		//     s = glfwGetMouseButton 	(window, GLFW_MOUSE_BUTTON_3 );cout<<(int)s<<endl;;
		//     s = glfwGetMouseButton 	(window, GLFW_MOUSE_BUTTON_4 );cout<<(int)s<<endl;;
	}
	else {
		// The window lost input focus
	}
}


void App::setCallbacks(GLFWwindow* pWin) {
	glfwSetCursorPosCallback(pWin, cursorPositionCallback);
	glfwSetMouseButtonCallback(pWin, mouseButtonCallback);
	glfwSetMonitorCallback(monitorCallback);
	glfwSetFramebufferSizeCallback(pWin, framebufferSizeCallback);
	glfwSetWindowSizeCallback(pWin, windowSizeCallback);
	#ifdef __APPLE__
		glfwSetWindowRefreshCallback(pWin, redrawCallback);
	#endif
	glfwSetKeyCallback(pWin, keyCallback);
	glfwSetWindowPosCallback(pWin, windowPosCallback);
	glfwSetWindowFocusCallback(pWin, windowFocusCallback);
}

// the first window is created in the `init` method, all other ones are made here, with the window they were requested from as their parent
void App::createWindow(WindowId parent) {
	WindowId newId = myGlContext.createWindow(parent);
	Window&  win = myGlContext.window(newId);

	int xpos, ypos;
	glfwGetWindowPos(win.glfwHandle(), &xpos, &ypos);
	myGlContext.windowMoved(win.glfwHandle(), xpos, ypos, myRedrawQueue);	

    myText      .setupSharedContext(myGlContext);
	myBubbles   .setupSharedContext(myGlContext, newId);
	myPanningBar.setupSharedContext(myGlContext, newId);
//	glBindVertexArray(win.bubblesVAO);
	setCallbacks(win.glfwHandle());

//		glViewport(0, 0, w, h);
cout<<"ntesiroa "<<win.screenunitWidth()<<endl;
cout<<"ntesiroa "<<win.screenunitHeight()<<endl;
//		glViewport(0, 0, w, h);

	myRedrawQueue.redrawAllWindows();
}

void App::init() {
	GlContext &ctx = myGlContext;

    ctx.createWindow(complex<float>(100.0f, 10.0f));
    setCallbacks(ctx.firstWindow().glfwHandle());

    myText      .initializeFirstContext(ctx);
	myBubbles   .initializeFirstContext(ctx);
	myPanningBar.initializeFirstContext(ctx);
//	glBindVertexArray(ctx.window(0).bubblesVAO);

	myBubbles.createBubble(ctx, 15.0f, 10.0f, 20.0f, 2.0f);

	myBubbles.createBubble(ctx, 15.0f, 20.0f, 20.0f, 2.0f);
	// myBubbles.createBubble(ctx, 10.0f, 20.0f, 20.0f, 20.0f);



//   pWin  = myGlContext.windows[0].glfwHandle;

    #ifndef __EMSCRIPTEN__
	 //    myGlContext.createWindow(complex<float>(100.0f, 10.0f));
	 //    pWin2 = myGlContext.windows[1].glfwHandle;
		// myBubbles.setupOnSharedContext(myGlContext, 1);

//	    glfwMakeContextCurrent(pWin);
//		ctx.changeWindow(0);
    #endif

//ctx.changeWindow(1);

	monitorCallback(nullptr,0);

	int xpos, ypos;
	GLFWwindow* pWin = ctx.firstWindow().glfwHandle();
	glfwGetWindowPos(pWin, &xpos, &ypos);
	ctx.windowMoved(pWin, xpos, ypos, myRedrawQueue);
//	myBubbles.uploadBubblePositionDataToContext();

}


// panning bar changed
// panning bar changed size

double lastframe = 0.0;

void App::draw() {

	double time = glfwGetTime();

	if (1.0 / (time - lastframe) > 75) {
//		return;
	}
	lastframe = time;

	bool anythingDrawn = false;
	GlContext &ctx = myGlContext;

	for (int i = 0; i < ctx.windowCount(); ++i) {
		Window& win = ctx.window(i);
		if (myRedrawQueue.doRedrawEverything() || ( win.inUse() && win.needsRefresh() )) {
			ctx.changeWindow(i);
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			myPanningBar.draw(ctx, i, myWorkspace, myBubbles);

			myBubbles.draw(ctx, i, myWorkspace);

anythingDrawn = true;
// now use panning bar to draw background

//			text.draw(ctx);

			ctx.swapBuffers();
			win.markRefreshed();
		}
	}
	myRedrawQueue.haveRedrawnEverything();

if (anythingDrawn) {
}

//glDrawArrays(GL_TRIANGLES, 0, 3);//-works! and no need to load a GL_ELEMENT_ARRAY_BUFFER if there's no complex geometry
	// glBindVertexArray(gApp->context.triangleVertexArray);
	// 	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
	// glBindVertexArray(    0);


// cout<<"Hum"<<endl;
// 	glBindVertexArray(    data.starVertexArray);
// //		glDrawArrays(GL_TRIANGLE_FAN, 0, 13);//3, GL_UNSIGNED_SHORT, 0);
// 		glDrawElements(GL_TRIANGLE_FAN, 13, GL_UNSIGNED_SHORT, 0);
// 	glBindVertexArray(    0);


//	glClear();
	#ifndef __EMSCRIPTEN__
	 //    glfwMakeContextCurrent(pWin2);

		// glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
		// glClear( GL_COLOR_BUFFER_BIT );
		// gApp.myBubbles.draw(gApp.myGlContext, 1);
		// glfwSwapBuffers(pWin2);

		// glfwMakeContextCurrent(pWin);
	#endif

}