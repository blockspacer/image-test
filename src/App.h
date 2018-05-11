#pragma once

#include <vector>
//using std::vector;

#include "GlContext.h"
#include "Bubbles.h"
#include "TextLayout.h"

struct App {
	GlContext glContext;
	TextLayout text;
	Bubbles   bubbles;
	App();
};


