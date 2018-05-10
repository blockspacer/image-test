#include <iostream>
using std::cout;
using std::endl;

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#ifdef __EMSCRIPTEN__
	#include <emscripten/emscripten.h>

#endif

//#include "globals.h"

#include "configuration.h"
//#include "GlContext.h"
#include "App.h"

GLFWwindow *pWin {0};
GLFWwindow *pWin2 {0};

App app;


static int framenum = 0;

void draw_frame() {
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

	glClear( GL_COLOR_BUFFER_BIT );

//glDrawArrays(GL_TRIANGLES, 0, 3);//-works! and no need to load a GL_ELEMENT_ARRAY_BUFFER if there's no complex geometry
	// glBindVertexArray(app->context.triangleVertexArray);
	// 	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
	// glBindVertexArray(    0);

	app.bubbles.draw(app.glContext, 0);

// cout<<"Hum"<<endl;
// 	glBindVertexArray(    data.starVertexArray);
// //		glDrawArrays(GL_TRIANGLE_FAN, 0, 13);//3, GL_UNSIGNED_SHORT, 0);
// 		glDrawElements(GL_TRIANGLE_FAN, 13, GL_UNSIGNED_SHORT, 0);
// 	glBindVertexArray(    0);


	glfwSwapBuffers(pWin);
//	glClear();

	#ifndef __EMSCRIPTEN__
	    glfwMakeContextCurrent(pWin2);

		glClearColor(0.0f, 0.0f, 0.4f, 1.0f);
		glClear( GL_COLOR_BUFFER_BIT );
		app.bubbles.draw(app.glContext, 1);
		glfwSwapBuffers(pWin2);

		glfwMakeContextCurrent(pWin);
	#endif

}


#ifdef __EMSCRIPTEN__
	extern "C" {
	 	void web_window_size_callback() {
			cout<<"size changed " << EM_ASM_INT(return document.innerHeight, 0) << '\n';
		}
	}
#else
	void native_window_size_callback(GLFWwindow* window, int width, int height) {
		cout<<"size changed " << width <<'\n';
	}
#endif

// EM_ASM({
// 	function back(text){
// 	    alert(Pointer_stringify(text));
// 	}
// 	var pointer = Runtime.addFunction(back);
// 	var call = Module.cwrap('call', 'void', ['pointer']);
// 	call(pointer);
// 	Runtime.removeFunction(pointer);
// });

#include <complex>
using std::complex;

int main() {

    std::cout << BOLD "Hello, World!" << NORMAL '\n';

//	pWin = initialise_glfw_and_compile_shader(app.glContext);

    GlContext &ctx = app.glContext;

    app.glContext.createWindow(complex<float>(100.0f, 10.0f));

	app.bubbles.setupOnFirstContext(app.glContext);
ctx.check_gl_errors("main");

	// BubbleId newb = app.bubbles.createBubble(5.0f,5.0f,5.0f,5.0f);
	// app.bubbles.uploadBubbleVertexDataToContext(app.glContext, newb);
	// BubbleId newb2 = app.bubbles.createBubble(5.0f,5.0f,5.0f,5.0f);
	// app.bubbles.uploadBubbleVertexDataToContext(app.glContext, newb2);
	// BubbleId newb3 = app.bubbles.createBubble(5.0f,5.0f,5.0f,5.0f);
	// app.bubbles.uploadBubbleVertexDataToContext(app.glContext, newb3);
	// BubbleId newb4 = app.bubbles.createBubble(5.0f,5.0f,5.0f,5.0f);
	// app.bubbles.uploadBubbleVertexDataToContext(app.glContext, newb4);


	app.bubbles.createBubble(ctx, -0.5f, -0.0f, 10.0f, 10.0f);
	app.bubbles.createBubble(ctx, 0.0f, -0.3f, 10.0f, 10.0f);

    pWin  = app.glContext.windows[0].glfwHandle;

    #ifndef __EMSCRIPTEN__
	    app.glContext.createWindow(complex<float>(100.0f, 10.0f));
	    pWin2 = app.glContext.windows[1].glfwHandle;
		app.bubbles.setupOnSharedContext(app.glContext, 1);
//	    glfwMakeContextCurrent(pWin);
		ctx.changeWindow(0);
    #endif

	app.bubbles.createBubble(ctx, 0.5f, 0.3f, 10.0f, 10.0f);
	app.bubbles.createBubble(ctx, 0.5f, -0.7f, 10.0f, 10.0f);

//	app.bubbles.uploadBubblePositionDataToContext();


    #ifdef __EMSCRIPTEN__
		read_in_user_settings_web_version();
	#else
		sel::State luaInterpreterState{true};
		luaInterpreterState("os = nil; io = nil; package = nil; debug = nil; require = nil");
		// would be better to start an environment without *any* standard libraries and only add stuff as needed, but unsure how to do that

		read_in_user_settings_native_version(luaInterpreterState);
	#endif



    #ifdef __EMSCRIPTEN__
		EM_ASM(document.getElementById("bod").onresize = Module._web_window_size_callback, 0);

		emscripten_set_main_loop(draw_frame, 0, 1);
	#else
//    	glfwSetWindowSizeCallback(pWin, native_window_size_callback);

		while (!glfwWindowShouldClose(pWin) && !glfwWindowShouldClose(pWin2)) {
			draw_frame();
			bool anim = false;
			if (anim)
				glfwPollEvents();
			else
				glfwWaitEvents();

		}
	#endif

	cout << "terminus est\n"<<std::endl;

	glfwTerminate();

	int n = 23;

	#ifdef __EMSCRIPTEN__
		// Your function
		EM_ASM_({
			Module.print('I received: ' + $0);
			return $0 + 1;
		}, n);
	#endif

	return 0;
}

int foo(int a) {
	cout << a*a;
	return a*a;
}