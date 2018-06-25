#include "Workspace.h"

void Workspace::setSize(Point greatestMonitorExtent) {
	#ifdef WEB
		greatestMonitorExtent.real(30);
		greatestMonitorExtent.imag(20);
	#endif
	myBottomRight.real(::x(greatestMonitorExtent) * 2.0); // twenty times width of widest monitor
	myBottomRight.imag(::y(greatestMonitorExtent) * 0.2);  // twice height of tallest monitor
	cout << "New workspace size: "<<::x(myBottomRight)<<" x "<<::y(myBottomRight)<<" cm"<<endl;
}


