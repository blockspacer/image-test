#include "RedrawRequests.h"

void RedrawRequests::newWindow(WindowId parent) {
	myNewWindowParentId = parent;
	myNewWindowFlag = true;
}

void RedrawRequests::newWindowCreated() {
	myNewWindowFlag = false;
}

