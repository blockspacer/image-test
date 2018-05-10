#pragma once

#include <string>
using std::string;

#include "GlContext.h"

GLFWwindow* initialise_glfw_and_compile_shader(GlContext& data);
//int compile_shader(string filename);
int compile_shader_from_source_string(string source);

void loaddemodata(GlContext &data);
void demoframe(GLuint program);

void check_gl_errores(string lable);
void check_gl_errores();



