#include "Workspace.h"

void Workspace::setSize(Point greatestMonitorExtent) {
	// #ifdef WEB
	// 	greatestMonitorExtent.real(30);
	// 	greatestMonitorExtent.imag(20);
	// #endif

//20
	myBottomRight.real(::x(greatestMonitorExtent) * 6); // twenty times width of widest monitor
	myBottomRight.imag(::y(greatestMonitorExtent) * 2);  // twice height of tallest monitor
	cout << "New workspace size: "<<::x(myBottomRight)<<" x "<<::y(myBottomRight)<<" cm"<<endl;
}


