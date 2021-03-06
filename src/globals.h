#pragma once

#include <map>
#include <string>
#include <iostream>
#include <algorithm>
#include <unordered_map>

using std::cout;

// from https://stackoverflow.com/questions/3378560/how-to-disable-gcc-warnings-for-a-few-lines-of-code
#define DIAG_STR(s) #s
#define DIAG_JOINSTR(x,y) DIAG_STR(x ## y)

#ifdef _MSC_VER
	#define DIAG_DO_PRAGMA(x) __pragma (#x)
	#define DIAG_PRAGMA(compiler,x) DIAG_DO_PRAGMA(warning(x))
#else
	#define DIAG_DO_PRAGMA(x) _Pragma (#x)
	#define DIAG_PRAGMA(compiler,x) DIAG_DO_PRAGMA(compiler diagnostic x)
#endif

#if defined(__clang__)
	# define DISABLE_WARNING(gcc_unused,clang_option,msvc_unused) DIAG_PRAGMA(clang,push) DIAG_PRAGMA(clang,ignored DIAG_JOINSTR(-W,clang_option))
	# define ENABLE_WARNING(gcc_unused,clang_option,msvc_unused) DIAG_PRAGMA(clang,pop)
#elif defined(_MSC_VER)
	# define DISABLE_WARNING(gcc_unused,clang_unused,msvc_errorcode) DIAG_PRAGMA(msvc,push) DIAG_DO_PRAGMA(warning(disable:##msvc_errorcode))
	# define ENABLE_WARNING(gcc_unused,clang_unused,msvc_errorcode) DIAG_PRAGMA(msvc,pop)
#elif defined(__GNUC__)
	#if ((__GNUC__ * 100) + __GNUC_MINOR__) >= 406
		# define DISABLE_WARNING(gcc_option,clang_unused,msvc_unused) DIAG_PRAGMA(GCC,push) DIAG_PRAGMA(GCC,ignored DIAG_JOINSTR(-W,gcc_option))
		# define ENABLE_WARNING(gcc_option,clang_unused,msvc_unused) DIAG_PRAGMA(GCC,pop)
	#else
		# define DISABLE_WARNING(gcc_option,clang_unused,msvc_unused) DIAG_PRAGMA(GCC,ignored DIAG_JOINSTR(-W,gcc_option))
		# define ENABLE_WARNING(gcc_option,clang_option,msvc_unused) DIAG_PRAGMA(GCC,warning DIAG_JOINSTR(-W,gcc_option))
	#endif
#endif

DISABLE_WARNING(unused-function, unused-function, unused-function) 

DISABLE_WARNING(class-memaccess,class-memaccess,class-memaccess)
#include "tinyutf8.h"
ENABLE_WARNING(class-memaccess,class-memaccess,class-memaccess)

DISABLE_WARNING(pragmas, pragmas, pragmas)
	// far too tedious to wrap every use of EM_ASM_ et al in these
	// i know turning off a warning globally is a v bad idea, but this one seems so obscure it can't hurt
	DISABLE_WARNING(dollar-in-identifier-extension, dollar-in-identifier-extension, dollar-in-identifier-extension)
	DISABLE_WARNING(unused-variable,unused-variable,unused-variable)
	DISABLE_WARNING(unused-parameter,unused-parameter,unused-parameter)
ENABLE_WARNING(pragmas, pragmas, pragmas)
	DISABLE_WARNING(reorder, reorder, reorder) 

//#include "include/selene.h"
//#include "SkColor.h"
//#include "Filesystem.h"

#include "Colors.h"

extern int controlKeysDown;

#define DEBUG 1
//#define REPORT 1

#ifdef __EMSCRIPTEN__
	#define EM 1
	#define WEB 1
#else
	#define NOTEM 1
	#define NATIVE 1
#endif

#include <GL/glew.h>

#include <GLFW/glfw3.h>

#include <vector>
using std::vector;

#include <iostream>
using std::cout;
using std::endl;

using BubbleGroupId = size_t;
using BubbleId = GLint;
using WindowId = size_t;

static std::vector<GLubyte> gPixelMemory;

enum gNumberNames {version_number, second_thing, maximumNumber};
static double gNums[maximumNumber];

enum gStringNames {workspace_menu_notes, maximumString};
static utf8_string gStrings[maximumString];

enum gColorNames {bupl, maximumColor};
static Color gColors[maximumColor];

enum gBoolNames {bob, maximumBoolean};
static bool gBools[maximumBoolean];

#include <utility>

#include <complex>
using std::complex;
using Point = complex<float>;
using PointD = complex<double>;
// float x(Point);
// float y(Point);
static float x(Point p) {return real(p);}
static float y(Point p) {return imag(p);}
static float x(PointD p) {return real(p);}
static float y(PointD p) {return imag(p);}



//const double PI  = 3.141592653589793238463;

#define PI 3.14159265

/* the following is from http://stackoverflow.com/questions/81870/is-it-possible-to-print-a-variables-type-in-standard-c */

/* Item 4 of Effective Modern C++ by Scott Meyers suggests using Boost.TypeIndex and type_id_with_cvr<decltype(NAME_OF_THING_GOES_HERE)>().pretty_name() for the same purpose, but i found this first */

// use it like:
//    SHOW_TYPE( thing )
// or 
//    cout << "thing's type is: "<< type_name<decltype( thing )>() << '\n';

#include <type_traits>
#include <typeinfo>
#ifndef _MSC_VER
	#include <cxxabi.h>
#endif
#include <memory>
#include <string>
#include <cstdlib>

template <class T>
std::string type_name() {
	typedef typename std::remove_reference<T>::type TR;
	std::unique_ptr<char, void(*)(void*)> own
		   (
#ifndef _MSC_VER
				abi::__cxa_demangle(typeid(TR).name(), nullptr,
										   nullptr, nullptr),
#else
				nullptr,
#endif
				std::free
		   );
	std::string r = own != nullptr ? own.get() : typeid(TR).name();
	if (std::is_const<TR>::value)
		r += " const";
	if (std::is_volatile<TR>::value)
		r += " volatile";
	if (std::is_lvalue_reference<T>::value)
		r += "&";
	else if (std::is_rvalue_reference<T>::value)
		r += "&&";
	return r;
}

// make terminal text more colorful using special ANSI escape character control codes
// https://en.wikipedia.org/wiki/ANSI_escape_code
#ifdef __EMSCRIPTEN__
	#define BOLD      "" << 
	#define ITALIC    "" << 
	#define UNDERLINE "" << 
	#define NORMAL    "" << 

	#define BOLDSTR      "" 
	#define ITALICSTR    "" 
	#define UNDERLINESTR "" 
	#define NORMALSTR    "" 

	#define BLACK   "" <<
	#define RED     "" <<
	#define GREEN   "" <<
	#define YELLOW  "" <<
	#define BLUE    "" <<
	#define MAGENTA "" <<
	#define CYAN    "" <<
	#define WHITE   "" <<
	#define BLACK_BG   "" <<
	#define RED_BG     "" <<
	#define GREEN_BG   "" <<
	#define YELLOW_BG  "" <<
	#define BLUE_BG    "" <<
	#define MAGENTA_BG "" <<
	#define CYAN_BG    "" <<
	#define WHITE_BG   "" <<
#else
	#define BOLD      "\033[1m" << 
	#define ITALIC    "\033[3m" << 
	#define UNDERLINE "\033[4m" << 
	#define NORMAL    "\033[0m" << 

	#define BOLDSTR      "\033[1m" 
	#define ITALICSTR    "\033[3m" 
	#define UNDERLINESTR "\033[4m" 
	#define NORMALSTR    "\033[0m" 

	#define BLACK   "\033[30m" <<
	#define RED     "\033[31m" <<
	#define GREEN   "\033[32m" <<
	#define YELLOW  "\033[33m" <<
	#define BLUE    "\033[34m" <<
	#define MAGENTA "\033[35m" <<
	#define CYAN    "\033[36m" <<
	#define WHITE   "\033[37m" <<
	#define BLACK_BG   "\033[40m" <<
	#define RED_BG     "\033[41m" <<
	#define GREEN_BG   "\033[42m" <<
	#define YELLOW_BG  "\033[43m" <<
	#define BLUE_BG    "\033[44m" <<
	#define MAGENTA_BG "\033[45m" <<
	#define CYAN_BG    "\033[46m" <<
	#define WHITE_BG   "\033[47m" <<

#endif

#pragma GCC diagnostic push
	#pragma GCC diagnostic ignored "-Waddress"

	#define SHOW_TYPE(...) std::cout << ITALIC "Variable " << NORMAL BOLD UNDERLINE #__VA_ARGS__ << NORMAL ITALIC " is of type " << NORMAL BOLD UNDERLINE type_name<decltype( __VA_ARGS__ )>() << NORMAL std::endl;
#pragma GCC diagnostic pop


#ifdef __EMSCRIPTEN__
	DISABLE_WARNING(dollar-in-identifier-extension, dollar-in-identifier-extension, dollar-in-identifier-extension)
#endif

#ifdef __EMSCRIPTEN__
	#include <emscripten/emscripten.h>

#endif

static int wemadeit_counter = 0;

DISABLE_WARNING(unused-function, unused-function, unused-function)
static void wemadeit() {
	cout << "WE MADE IT! " << ++wemadeit_counter << '\n';
}

#include <string>
using std::string;

static void complain(string s) {
	#ifdef __EMSCRIPTEN__
		// EM_ASM({
		// 	var str = Pointer_stringify($0);
		// 	var txt = document.createTextNode(str);
		// 	var sp = document.createElement('span');
		// 	sp.appendChild(txt);
		// 	var bod = document.getElementById('canvas').parentNode;
		// 	bod.insertBefore(sp, bod.childNodes[0]); 

		// }, s.c_str());
	#endif
	cout << s << std::endl;
}

static void complain(string s, char *c_s) {
	s.append(c_s);
	complain(s);
}

ENABLE_WARNING(unused-function, unused-function, unused-function)

DISABLE_WARNING(unused-parameter, unused-parameter, unused-parameter)

