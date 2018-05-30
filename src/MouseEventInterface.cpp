#include "MouseEventInterface.h"

bool MouseEventInterface::mouseMoved(Point p, RedrawRequests &redrawReqests) {
	cout << x(p)<<", "<<y(p)<<endl;
}

