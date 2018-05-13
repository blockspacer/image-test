#include "App.h"

void App::init() {
	GlContext &ctx = glContext;

    glContext.createWindow(complex<float>(100.0f, 10.0f));

ctx.check_gl_errors("main1");
	bubbles.setupOnFirstContext(glContext);
ctx.check_gl_errors("main2");
    text.initOnFirstContext(glContext);
	glBindVertexArray(ctx.windows[0].bubblesVAO);

ctx.check_gl_errors("main3");

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

	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

	glClear( GL_COLOR_BUFFER_BIT );

//glDrawArrays(GL_TRIANGLES, 0, 3);//-works! and no need to load a GL_ELEMENT_ARRAY_BUFFER if there's no complex geometry
	// glBindVertexArray(gApp->context.triangleVertexArray);
	// 	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
	// glBindVertexArray(    0);

	bubbles.draw(glContext, 0);

	text.draw(glContext);

// cout<<"Hum"<<endl;
// 	glBindVertexArray(    data.starVertexArray);
// //		glDrawArrays(GL_TRIANGLE_FAN, 0, 13);//3, GL_UNSIGNED_SHORT, 0);
// 		glDrawElements(GL_TRIANGLE_FAN, 13, GL_UNSIGNED_SHORT, 0);
// 	glBindVertexArray(    0);


	glfwSwapBuffers(ctx.pCurrentContext);
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