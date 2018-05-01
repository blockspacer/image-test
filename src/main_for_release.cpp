#include <iostream>
using std::cout;
using std::endl;

//#include "GLES3/gl3.h"

//#define GLFW_INCLUDE_ES3

#include "GL/glew.h"
#include "GLFW/glfw3.h"

#ifdef __EMSCRIPTEN__
	#include <emscripten/emscripten.h>

#endif

//#include "globals.h"

#include "configuration.h"

void glfw_error_callback(int error, const char* description) {
	cout << BOLD "GLFW error " << error << NORMAL " : " << description << std::endl;
}

GLFWwindow *pWin;


static int framenum = 0;
void draw_frame() {
	cout<<"Drew frame " << ++framenum << '\n';
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
    std::cout << "Hello, World!\n";

	if (!glfwInit()) {
		complain("GLFW init failed :(");
		exit(-1);
    // Initialization failed
	}

	glfwSetErrorCallback(glfw_error_callback);

	glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);

	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
	glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); 
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 1024,
             height = 633;

	#ifdef __EMSCRIPTEN__
			// Your function
			width = EM_ASM_INT( {return window.innerWidth;}, 0 );
			height = EM_ASM_INT( {return window.innerHeight;}, 0 );
			EM_ASM({var el = document.getElementById("message");el.parentNode.removeChild(el);}, 0);
	#endif

	pWin = glfwCreateWindow(width, height, "image test", NULL, NULL);
    if (!pWin) {
        complain("glfwCreateWindow() failed");
        exit(-1);
    }

    read_in_global_configuration();

    glfwMakeContextCurrent(pWin);
    glfwSwapInterval(1);

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