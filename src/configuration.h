#pragma once
//#include <iostream>
#include <string>
#include <unordered_map>

//#include "SkColor.h"
#include "globals.h"

#ifdef __EMSCRIPTEN__ // web version uses Fengari.js to execute Lua
	void read_in_user_settings_web_version();
#else	
	// #pragma GCC diagnostic ignored "-Wunused-variable"
	// #pragma GCC diagnostic ignored "-Wunused-parameter"
	// #pragma GCC diagnostic ignored "-Wunknown-pragmas"
	#include "selene.h"
	// turn the warnings back on
	// #pragma GCC diagnostic pop
	// #pragma GCC diagnostic pop
	// #pragma GCC diagnostic pop
	void read_in_user_settings_native_version(sel::State& luaInterpreter);
#endif

//#include <iostream>

// class Configuration {
// 	std::unordered_map<std::string, double>  mNumbers;
// 	std::unordered_map<std::string, SkColor> mColors;
// 	std::unordered_map<std::string, std::string>  mStrings;
// 	std::unordered_map<std::string, bool>    mBooleans;

// 	std::string fallback_empty_string {""};
// //loaders go here...

// public:
// 	Configuration();
// 	~Configuration();

// 	double number(std::string&& name);
// 	SkColor color(std::string&& name);
// 	std::string& string(std::string&& name);
// 	bool boolean(std::string&& name);

// 	void load_user_settings(sel::State& luaInterpreter);
// };





