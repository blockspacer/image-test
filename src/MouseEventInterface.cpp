#include "MouseEventInterface.h"

bool MouseEventInterface::mouseMoved(Point p, Workspace &wksp, RedrawRequests &redrawReqests) {
	cout << x(p)<<", "<<y(p)<<endl;
}

