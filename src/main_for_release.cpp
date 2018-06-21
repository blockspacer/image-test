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

App gApp;

#ifdef WEB
	extern "C" {
		void resize_web_canvas(int wh, int hh) {
			gApp.webCanvasResize(wh, hh);
		}
	}
#endif

static int framenum = 0;

void draw_frame() {
	gApp.draw();
#ifdef WEB
	emscripten_pause_main_loop();
#endif
}


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

//	pWin = initialise_glfw_and_compile_shader(gApp.glContext);

    gApp.init();

    #ifdef __EMSCRIPTEN__
		read_in_user_settings_web_version();
	#else
		sel::State luaInterpreterState{true};
		luaInterpreterState("os = nil; io = nil; package = nil; debug = nil; require = nil");
		// would be better to start an environment without *any* standard libraries and only add stuff as needed, but unsure how to do that

		read_in_user_settings_native_version(luaInterpreterState);
	#endif



    #ifdef __EMSCRIPTEN__
//		EM_ASM({window.onResize = alert("SIZ");}//Module._web_window_size_callback
//			, 0);

		emscripten_set_main_loop(draw_frame, 0, 1);
	#else
//    	glfwSetWindowSizeCallback(pWin, native_window_size_callback);

		while (!glfwWindowShouldClose(gApp.glContext().window(0).glfwHandle()) ){// && !glfwWindowShouldClose(pWin2)) {
			RedrawRequests &rdq = gApp.redrawQueue();
			if (rdq.newWindowRequested()) {
				gApp.createWindow( rdq.newWindowParentId() );
				rdq.newWindowCreated();
			}
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