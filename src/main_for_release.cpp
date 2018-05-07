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
#include "opengl_setup.h"
//#include "GlContext.h"
#include "App.h"

GLFWwindow *pWin;

App app;


static int framenum = 0;

void draw_frame() {
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

	glClear( GL_COLOR_BUFFER_BIT );

//glDrawArrays(GL_TRIANGLES, 0, 3);//-works! and no need to load a GL_ELEMENT_ARRAY_BUFFER if there's no complex geometry
	// glBindVertexArray(app->context.triangleVertexArray);
	// 	glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
	// glBindVertexArray(    0);

	app.bubbles.draw(app.glContext);



// cout<<"Hum"<<endl;
// 	glBindVertexArray(    data.starVertexArray);
// //		glDrawArrays(GL_TRIANGLE_FAN, 0, 13);//3, GL_UNSIGNED_SHORT, 0);
// 		glDrawElements(GL_TRIANGLE_FAN, 13, GL_UNSIGNED_SHORT, 0);
// 	glBindVertexArray(    0);


	glfwSwapBuffers(pWin);
//	glClear();
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



int main() {

    std::cout << BOLD "Hello, World!" << NORMAL '\n';

	pWin = initialise_glfw_and_compile_shader(app.glContext);
	app.bubbles.setupContext(app.glContext);
	BubbleId newb = app.bubbles.createBubble(5.0f,5.0f,5.0f,5.0f);
	app.bubbles.uploadBubbleVertexDataToContext(app.glContext, newb);


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

		while (!glfwWindowShouldClose(pWin)) {
			draw_frame(&app);
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