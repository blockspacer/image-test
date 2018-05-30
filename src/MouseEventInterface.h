#pragma once

#include "globals.h"
#include "RedrawRequests.h"

class MouseEventInterface {

public:
	bool mouseMoved(Point p, RedrawRequests &redrawQueue);
};


