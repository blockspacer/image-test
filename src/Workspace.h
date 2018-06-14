#pragma once

#include "globals.h"

class Workspace {
	Point myTopLeft {0,0},
		  myBottomRight{250,50};
public:
	void  setSize(Point extent);
	Point topLeft() {return myTopLeft;};
	Point size()    {return myBottomRight - myTopLeft;};
	float width()   {return ::x(size());};
	float height()  {return ::y(size());};
	float aspectRatio() {Point s = size(); return ::x(s) / ::y(s);};
	int   pixelHeightForWindowWidth(int w) {return w / aspectRatio();};

};


