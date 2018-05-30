#include "App.h"

#include "globals.h"


MouseEvents    App::mouseHandler;
RedrawRequests App::redrawQueue;
GlContext 	   App::glContext;

void App::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos) {
	mouseHandler.moved(window, xpos, ypos, glContext, redrawQueue);
}

void App::mouseButtonCallback(GLFWwindow* window, int button, int action, int mods) {
	mouseHandler.buttonInput(window, button, action, mods, glContext, redrawQueue);
}

void App::setCallbacks(GLFWwindow* pWin) {
	glfwSetCursorPosCallback(pWin, cursorPositionCallback);
	glfwSetMouseButtonCallback(pWin, mouseButtonCallback);
}

void App::createWindow(WindowId parent) {
	WindowId newId = glContext.createWindow(parent);
	Window&  win = glContext.window(newId);

    text.setupOnSharedContext    (glContext);
	bubbles.setupOnSharedContext(glContext, newId);
//	glBindVertexArray(win.bubblesVAO);

}

void App::init() {
	GlContext &ctx = glContext;

    ctx.createWindow(complex<float>(100.0f, 10.0f));
    setCallbacks(ctx.window(0).glfwHandle);

    text.initOnFirstContext(glContext);
	bubbles.setupOnFirstContext(glContext);
//	glBindVertexArray(ctx.window(0).bubblesVAO);


	// BubbleId newb = bubbles.createBubble(5.0f,5.0f,5.0f,5.0f);
	// bubbles.uploadBubbleVertexDataToContext(glContext, newb);
	// BubbleId newb2 = bubbles.createBubble(5.0f,5.0f,5.0f,5.0f);
	// bubbles.uploadBubbleVertexDataToContext(glContext, newb2);
	// BubbleId newb3 = bubbles.createBubble(5.0f,5.0f,5.0f,5.0f);
	// bubbles.uploadBubbleVertexDataToContext(glContext, newb3);
	// BubbleId newb4 = bubbles.createBubble(5.0f,5.0f,5.0f,5.0f);
	// bubbles.uploadBubbleVertexDataToContext(glContext, newb4);


	bubbles.createBubble(ctx, -0.5f, -0.0f, 10.0f, 10.0f);
	bubbles.createBubble(ctx, 0.0f, -0.3f, 10.0f, 10.0f);

//   pWin  = glContext.windows[0].glfwHandle;

    #ifndef __EMSCRIPTEN__
	 //    glContext.createWindow(complex<float>(100.0f, 10.0f));
	 //    pWin2 = glContext.windows[1].glfwHandle;
		// bubbles.setupOnSharedContext(glContext, 1);

//	    glfwMakeContextCurrent(pWin);
//		ctx.changeWindow(0);
    #endif

	bubbles.createBubble(ctx, 0.5f, 0.3f, 10.0f, 10.0f);
	bubbles.createBubble(ctx, 0.5f, -0.7f, 10.0f, 10.0f);
//ctx.changeWindow(1);
	bubbles.createBubble(ctx, -0.8f, -0.7f, 10.0f, 10.0f);

//	bubbles.uploadBubblePositionDataToContext();

}


void App::draw() {
	GlContext &ctx = glContext;
cout<<"drawl "<<redrawQueue.doRedrawEverything()<<endl;
	for (int i = 0; i < glContext.windowCount(); ++i) {
		Window& win = glContext.window(i);
		if (redrawQueue.doRedrawEverything() || ! win.unused && win.needsRefresh) {
cout<<"drawing window "<<i<<endl;
			ctx.changeWindow(i);
			glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
			glClear( GL_COLOR_BUFFER_BIT );
			bubbles.draw(glContext, i);
			text.draw(glContext);

			ctx.swapBuffers();
			win.needsRefresh = false;
		}
	}
	redrawQueue.haveRedrawnEverything();


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
		// gApp.bubbles.draw(gApp.glContext, 1);
		// glfwSwapBuffers(pWin2);

		// glfwMakeContextCurrent(pWin);
	#endif

}