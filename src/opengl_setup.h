#pragma once

#include <string>
using std::string;



GLFWwindow* initialise_glfw_and_compile_shader();
//int compile_shader(string filename);
int compile_shader_from_source_string(string source);

