#include "App.h"

MouseEvents    App::myMouseHandler;
RedrawRequests App::myRedrawQueue;
GlContext 	   App::myGlContext;
PanningBar     App::myPanningBar;
Workspace      App::myWorkspace;

void App::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	myMouseHandler.moved(window, xpos, ypos, myGlContext, myPanningBar, myRedrawQueue);
}

void App::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	myMouseHandler.buttonInput(window, button, action, mods, myGlContext, myRedrawQueue);
}

void App::monitorCallback(GLFWmonitor* monitor, int event) {
	Point largestMonitorExtent = myGlContext.getMonitorsInfo();
	myWorkspace.setSize(largestMonitorExtent);
}

void App::setCallbacks(GLFWwindow* pWin) {
	glfwSetCursorPosCallback(pWin, cursorPositionCallback);
	glfwSetMouseButtonCallback(pWin, mouseButtonCallback);
	glfwSetMonitorCallback(monitorCallback);
}

// the first window is created in the `init` method, all other ones are made here, with the window they were requested from as their parent
void App::createWindow(WindowId parent) {
	WindowId newId = myGlContext.createWindow(parent);
	Window&  win = myGlContext.window(newId);

    text     .setupSharedContext(myGlContext);
	myBubbles.setupSharedContext(myGlContext, newId);
//	glBindVertexArray(win.bubblesVAO);

}

void App::init() {
	GlContext &ctx = myGlContext;

    ctx.createWindow(complex<float>(100.0f, 10.0f));
    setCallbacks(ctx.window(0).glfwHandle);

    text     .initializeFirstContext(ctx);
	myBubbles.initializeFirstContext(ctx);
//	glBindVertexArray(ctx.window(0).bubblesVAO);


	// BubbleId newb = myBubbles.createBubble(5.0f,5.0f,5.0f,5.0f);
	// myBubbles.uploadBubbleVertexDataToContext(myGlContext, newb);
	// BubbleId newb2 = myBubbles.createBubble(5.0f,5.0f,5.0f,5.0f);
	// myBubbles.uploadBubbleVertexDataToContext(myGlContext, newb2);
	// BubbleId newb3 = myBubbles.createBubble(5.0f,5.0f,5.0f,5.0f);
	// myBubbles.uploadBubbleVertexDataToContext(myGlContext, newb3);
	// BubbleId newb4 = myBubbles.createBubble(5.0f,5.0f,5.0f,5.0f);
	// myBubbles.uploadBubbleVertexDataToContext(myGlContext, newb4);


	myBubbles.createBubble(ctx, -0.5f, -0.0f, 10.0f, 10.0f);
	myBubbles.createBubble(ctx, 0.0f, -0.3f, 10.0f, 10.0f);

//   pWin  = myGlContext.windows[0].glfwHandle;

    #ifndef __EMSCRIPTEN__
	 //    myGlContext.createWindow(complex<float>(100.0f, 10.0f));
	 //    pWin2 = myGlContext.windows[1].glfwHandle;
		// myBubbles.setupOnSharedContext(myGlContext, 1);

//	    glfwMakeContextCurrent(pWin);
//		ctx.changeWindow(0);
    #endif

	myBubbles.createBubble(ctx, 0.5f, 0.3f, 10.0f, 10.0f);
	myBubbles.createBubble(ctx, 0.5f, -0.7f, 10.0f, 10.0f);
//ctx.changeWindow(1);
	myBubbles.createBubble(ctx, -0.8f, -0.7f, 10.0f, 10.0f);

	monitorCallback(nullptr,0);
//	myBubbles.uploadBubblePositionDataToContext();

	myBubbles.createBubble(ctx, 15.0f, 13.0f, 20.0f, 20.0f);
	myBubbles.createBubble(ctx, 35.0f, 33.0f, 20.0f, 20.0f);
	myBubbles.createBubble(ctx, 55.0f, 53.0f, 20.0f, 20.0f);
	myBubbles.createBubble(ctx, 0.5f, 0.3f, 10.0f, 10.0f);
}


void App::draw() {
	GlContext &ctx = myGlContext;

	for (int i = 0; i < ctx.windowCount(); ++i) {
		Window& win = ctx.window(i);
		if (myRedrawQueue.doRedrawEverything() || ! win.unused && win.needsRefresh) {
			ctx.changeWindow(i);
			glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
			glClear( GL_COLOR_BUFFER_BIT );
			myBubbles.draw(ctx, i);
			text.draw(ctx);

			myPanningBar.draw(ctx, i, myWorkspace, myBubbles);

			ctx.swapBuffers();
			win.needsRefresh = false;
		}
	}
	myRedrawQueue.haveRedrawnEverything();


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