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


GLFWwindow *pWin;


static int framenum = 0;
void draw_frame() {
//	cout<<"Drew frame " << ++framenum << '\n';
	glClearColor(0.0f, 0.0f, 0.4f, 1.0f);

	glClear( GL_COLOR_BUFFER_BIT );
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

	pWin = initialise_glfw_and_compile_shader();

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
    	glfwSetWindowSizeCallback(pWin, native_window_size_callback);

		while (!glfwWindowShouldClose(pWin)) {
				draw_frame();
			bool anim = true;
			if (anim)
				glfwPollEvents();
			else
				glfwPollEvents();

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