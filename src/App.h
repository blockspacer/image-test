#pragma once

#include <vector>
//using std::vector;

#include "GlContext.h"
#include "Bubbles.h"

struct App {
	GlContext glContext;
	Bubbles   bubbles;
	App();
};


