#pragma once

#include "globals.h"

class Workspace {
	Point topLeft {0,0},
		  bottomRight{250,50};
public:
	void  setInitialSize(float w, float h) {bottomRight.real(w); bottomRight.imag(h);};
	Point size()   {return bottomRight - topLeft;};
	float width()  {return ::x(size());};
	float height() {return ::y(size());};
	float aspectRatio() {Point s = size(); return ::x(s) / ::y(s);};
	int   pixelHeightForWindowWidth(int w) {return w / aspectRatio();};

};


