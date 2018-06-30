#pragma once

#include "globals.h"
#include "RedrawRequests.h"
#include "Workspace.h"

class MouseEventInterface {

public:
	bool mouseMoved(Point p, Workspace &wksp, RedrawRequests &redrawQueue);
};


