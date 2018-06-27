#include "PanningBar.h"



void PanningBar::initializeFirstContext(GlContext &ctx) {
	GLuint shader = ctx.shaderHandle();

	glGenBuffers(1, &myVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, myVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredVertex) * 4, nullptr, GL_STATIC_DRAW);

		float dim = 0.03f, bright = 0.3f
			, dr = dim, dg = 0.0f, db = dim
			, br = bright, bg = 0.0f, bb = bright
		;

		myBackgroundVertices[0].r = dr;
		myBackgroundVertices[0].g = dg;
		myBackgroundVertices[0].b = db;
		myBackgroundVertices[0].a = 1.0f;

		myBackgroundVertices[2].r = dr;
		myBackgroundVertices[2].g = dg;
		myBackgroundVertices[2].b = db;
		myBackgroundVertices[2].a = 1.0f;

		myBackgroundVertices[1].r = br;
		myBackgroundVertices[1].g = bg;
		myBackgroundVertices[1].b = bb;
		myBackgroundVertices[1].a = 1.0f;
		myBackgroundVertices[3].r = br;
		myBackgroundVertices[3].g = bg;
		myBackgroundVertices[3].b = bb;
		myBackgroundVertices[3].a = 1.0f;

		myBackgroundVertices[0].t = -1.0f;
		myBackgroundVertices[1].t = -1.0f;
		myBackgroundVertices[2].t = -1.0f;
		myBackgroundVertices[3].t = -1.0f;


		glGenBuffers(1, &myPanningBarBubbleVertexBuffer);
		glBindBuffer(GL_ARRAY_BUFFER, myPanningBarBubbleVertexBuffer);

		for (int i = 0; i<8; ++i) {
			myPanningBarBubbleVertices[i].t = - 2.0f;
		}

		glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredVertex) * 8, &myPanningBarBubbleVertices, GL_STATIC_DRAW);

		glGenBuffers(1, &myWindowOutlineBuffer);
		glGenBuffers(1, &myWindowViewAreaBuffer);
}
#include <functional>
#include <cmath>
// PanningBar::
// last argument is a lambda called with the coords of the next point, and whether it's on the inside (0) or outside (1) of the curve
// the arc is drawing starting near the x-axis and going anticlockwise
void drawCornerStrip(Point center, Point xAxis, Point yAxis, float innerRadius, float outerRadius, size_t steps, bool doubleFirstPoint, std::function<void(Point v, float io, bool last)> func, float startAngle = 0.0f, float stopAngle = 0.0f) {

	float innerAngleStep = PI / (2.0f * steps + 2)
		, outerAngleStep = (PI / 2 - stopAngle - startAngle) / (steps + 1)
		//(stopAngle - startAngle) / (steps + 1)
		, innerAngle = 0.0f
		, outerAngle = startAngle

	;

	Point p = center + innerRadius * xAxis;
	func(p, 0.0f,false);
	if (doubleFirstPoint) {
		func(p, 0.0f,false);
		func(p, 0.0f,false);
	}
	float a = startAngle;

	for (size_t i = 0; i <= steps; ++i) {
		func(center + outerRadius * (float(sin(outerAngle))*yAxis + float(cos(outerAngle))*xAxis), 1.0f, false);
		innerAngle += innerAngleStep;
		func(center + innerRadius * (float(sin(innerAngle))*yAxis + float(cos(innerAngle))*xAxis), 0.0f, (i==steps));
		outerAngle += outerAngleStep;
	}

	func(center + outerRadius * (float(sin(outerAngle))*yAxis + float(cos(outerAngle))*xAxis), 1.0f, false);

}


float sq(float x) {return x*x;}

#include "Colors.h"
#include <cmath>

float min(float a, float b) {return a<b ? a : b;}

void drawSideStrip(PointD center, Point xAxis, Point yAxis, Point innerEdgeStart, float innerEdgeSide, std::function<void(Point v, float io, bool last)> func, bool last, size_t steps, double DistToCenter, double radius, double startAngle);

void PanningBar::prepWindowOutline(Window& win) {
	myWindowOutlineVertices.clear();
	myWindowViewAreaVertices.clear();
	Point tl = win.topLeft(),
		  br = win.bottomRight();

	float t  = ::y(tl)
		, l  = ::x(tl)
		, b  = ::y(br)
		, r  = ::x(br)
		, w  = 4.0 // width of border
		, cr = 0.0
		, cg = 0.7
		, cb = 1.0
		// , out = 0.0
		// , in = 1.0
		;
l = 30;
r = l+90;
t = 25;
b=t+50;
Point c {l+r,t+b};
c/=2.0f;

Color col {0.0f, 1.0f, 0.7f, 0.3f};
//
	auto lam = [&](Point p, float io, bool last) {
		myWindowOutlineVertices.emplace_back(::x(p), ::y(p), 0.0f, col.rPremul32F(), col.gPremul32F(), col.bPremul32F(), col.alphaFloat(), -1.0f, io);
	};
//3 6
	float in = 4, out = 20;//8

	float margin = 1.5; // time the corner circle radius

	float topLength  = (r-l-2*in)
		, sideLength = (b-t-2*in)
	;

	float topAngle  = min(PI / 8, 2 * atan( ( topLength / 2 * out * (margin - 1)) ))
		, sideAngle = min(PI / 8, 2 * atan( (sideLength / 2 * out * (margin - 1)) ));

	double topRadius = out +  (topLength / (2 * sin (topAngle)))
		, sideRadius = out + (sideLength / (2 * sin(sideAngle)))

		,  topCenterDist =  topLength / (2 * tan (topAngle))
		, sideCenterDist = sideLength / (2 * tan(sideAngle))
	;

	const Point up    { 0, -1};
	const Point down  { 0,  1};
	const Point left  {-1,  0};
	const Point right { 1,  0};

	drawCornerStrip(Point(r-in,b-in), right, down, in, out, 3, true, lam, sideAngle, topAngle);

	drawSideStrip(PointD((r+l)/2.0, b-in-topCenterDist), left, down, Point(r-in, b), topLength, lam, false, 15, topCenterDist, topRadius, topAngle);

	drawCornerStrip(Point(l+in,b-in), Point(0,1), Point(-1,0), in, out, 3, false, lam, topAngle, sideAngle);

	drawSideStrip(PointD(l + in + sideCenterDist, (t+b)/2.0), up, left, Point(l, b-in), sideLength, lam, false, 15, sideCenterDist,  sideRadius, sideAngle);

	drawCornerStrip(Point(l+in,t+in), left, up, in, out, 3, false, lam, sideAngle, topAngle);

	drawSideStrip(PointD((r+l)/2.0, t+in+ topCenterDist ), right, up, Point(l+in, t), topLength, lam, false, 15, topCenterDist,  topRadius, topAngle);

	drawCornerStrip(Point(r-in,t+in), up, right, in, out, 3, false, lam, topAngle, sideAngle);

	drawSideStrip(PointD(r - in - sideCenterDist, (t+b)/2.0), down, right, Point(r, t+in), sideLength, lam, false, 15, sideCenterDist,  sideRadius, sideAngle);


	lam(Point(r,b-in),0.0f,false);
	lam(Point(r-in+out*cos(sideAngle),b-in+out*sin(sideAngle)),0.0f,false);
	lam(Point(r-in+out*cos(sideAngle),b-in+out*sin(sideAngle)),0.0f,false);
//	myWindowOutlineVertices.emplace_back(r, b-in, 0.0f, cr, cg, cb, 0.0f, -1.0f, 0.0f);
	// myWindowOutlineVertices.emplace_back(r-in+out, b-in, 0.0f, cr, cg, cb, 0.0f, -3.0f, 1.0f);
	// myWindowOutlineVertices.emplace_back(r-in+out, b-in, 0.0f, cr, cg, cb, 0.0f, -3.0f, 1.0f);

	Color col2 = col;
	col2.setAlpha(0.3f);

	auto tint = [&](float x, float y) {
		myWindowViewAreaVertices.emplace_back(x, y, 0.0f, col2.rPremul32F(), col2.gPremul32F(), col2.bPremul32F(), col2.alphaFloat(), -1.0f, 0.0f);
	};


	float mid = in / 2.0f;

	tint(l      , t + mid);
	tint(l      , t + mid); tint(::x(c), ::y(c));
	tint(l + mid, t      ); tint(::x(c), ::y(c));
	tint(r - mid, t      ); tint(::x(c), ::y(c));
	tint(r      , t + mid); tint(::x(c), ::y(c));
	tint(r      , b - mid); tint(::x(c), ::y(c));
	tint(r - mid, b      ); tint(::x(c), ::y(c));
	tint(l + mid, b      ); tint(::x(c), ::y(c));
	tint(l      , b - mid); tint(::x(c), ::y(c));
	tint(l      , t + mid);
	tint(l      , t + mid);

	glBindBuffer(GL_ARRAY_BUFFER, myWindowOutlineBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredVertex) * myWindowOutlineVertices.size(), &(myWindowOutlineVertices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, myWindowViewAreaBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredVertex) * myWindowViewAreaVertices.size(), &(myWindowViewAreaVertices[0]), GL_STATIC_DRAW);
}

void drawSideStrip(PointD center, Point xAxis, Point yAxis, Point innerEdgeStart, float innerEdgeSide, std::function<void(Point v, float io, bool last)> func, bool last = false, size_t steps = 0, double distToCenter=0.0, double radius=0.0, double startAngle = 0.0) {

	// if last, draw final inner point then two outer points

	if (! steps) return;

	double angle     = -startAngle
		,  angleStep = (2*startAngle) / (steps+1)
	;

	Point innerStep = xAxis * (innerEdgeSide  / (steps + 1));
	Point inner = innerEdgeStart;

	PointD c {::x(inner), ::y(inner)}
		, xD {::x(xAxis), ::y(xAxis)}
		, yD {::x(yAxis), ::y(yAxis)}
	;

	c += xD * double(innerEdgeSide / 2);
	c += -yD * distToCenter;
cout<<"cent "<<c<<endl;
	for (size_t i = 0; i < steps; i++) {
		inner += innerStep;
		func(inner, 0.0f, false);

		angle += angleStep;
		PointD p = center + xD * (radius * sin(angle)) + yD * (radius * cos(angle));
		func(Point(float(::x(p)), float(::y(p))), 1.0f, false);
	}

	// draw final point

}

void PanningBar::draw(GlContext &ctx, WindowId winid, Workspace& wksp, Bubbles& bubbles) {
	Window& win = ctx.window(winid);
prepWindowOutline(win);
//	glBindVertexArray(ctx.bubblesVAO);
//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(win.backgroundVAO);

	glBindBuffer(GL_ARRAY_BUFFER, myVertexBuffer);



	if (wksp.sizeChanged()) {
		Point tl = wksp.topLeft()
			, br = wksp.bottomRight();

		myBackgroundVertices[0].x = ::x(br);
		myBackgroundVertices[0].y = ::y(br);
		myBackgroundVertices[1].x = ::x(br);
		myBackgroundVertices[1].y = ::y(tl);
		myBackgroundVertices[2].x = ::x(tl);
		myBackgroundVertices[2].y = ::y(br);
		myBackgroundVertices[3].x = ::x(tl);
		myBackgroundVertices[3].y = ::y(tl);

		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(ColoredVertex)*4, &myBackgroundVertices);

		wksp.sizeHasNotChanged();
	}


	GLuint shader = ctx.shaderHandle();
	int myPosAttrib = glGetAttribLocation(shader, "position");
	int myColAttrib = glGetAttribLocation(shader, "color");
	int myTexAttrib = glGetAttribLocation(shader, "texCoord");

	glEnableVertexAttribArray(myPosAttrib);
	glVertexAttribPointer(myPosAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) 0);

	glEnableVertexAttribArray(myColAttrib);
	glVertexAttribPointer(myColAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) 12);

	glEnableVertexAttribArray(myTexAttrib);
	glVertexAttribPointer(myTexAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) (7*4));


/*
scale to fit panning bar in window
position it so origin is at (-1,1)
*/
	float horizontalScale = 2 / wksp.width();
	float pbph = win.panningBarPixelHeight(wksp);

	float	verticalScale = (2 * pbph / win.pixelHeight()) / wksp.height();
cout<<"hhhtx "<<wksp.width()<<endl;
cout<<"veretx "<<wksp.height()<<endl;

	myTransformationMatrix = mat4(1.0f);
	myTransformationMatrix = translate(myTransformationMatrix, vec3(-1.0, 1.0, 0.0));
	myTransformationMatrix = scale(myTransformationMatrix, vec3(horizontalScale, -verticalScale, 1));

	ctx.setMatrix(myTransformationMatrix);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	glBindVertexArray(win.panningBarBubbleVAO);


	glBindBuffer(GL_ARRAY_BUFFER, myPanningBarBubbleVertexBuffer);
	glEnableVertexAttribArray(myTexAttrib);
	glVertexAttribPointer(myTexAttrib, 1, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) (7*4));

	glEnableVertexAttribArray(myColAttrib);
	glVertexAttribPointer(myColAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) 12);

	glEnableVertexAttribArray(myPosAttrib);
	glVertexAttribPointer(myPosAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) 0);

	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, bubbles.dataTexture());

cout<<"horiz "<<bubbles.count()<<endl;
	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 8, bubbles.count());
	// glDrawArrays(GL_TRIANGLE_FAN, 0, 4);


	glBindBuffer(GL_ARRAY_BUFFER, myWindowOutlineBuffer);

	glEnableVertexAttribArray(myPosAttrib);
	glVertexAttribPointer(myPosAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) 0);


	glEnableVertexAttribArray(myColAttrib);
	glVertexAttribPointer(myColAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) 12);

	glEnableVertexAttribArray(myTexAttrib);
	glVertexAttribPointer(myTexAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) (7*4));


	glEnable (GL_BLEND);
	//glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	// glBlendEquationSeparate(GL_FUNC_ADD,GL_FUNC_ADD);
	// glBlendFuncSeparate(GL_ONE,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_SCISSOR_TEST);
	glScissor(0, win.pixelHeight() - win.panningBarPixelHeight(wksp), win.pixelWidth(), pbph);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, myWindowOutlineVertices.size());
	glDrawArrays(GL_LINE_STRIP, 0, myWindowOutlineVertices.size());








	glBindBuffer(GL_ARRAY_BUFFER, myWindowViewAreaBuffer);

	glEnableVertexAttribArray(myPosAttrib);
	glVertexAttribPointer(myPosAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) 0);


	glEnableVertexAttribArray(myColAttrib);
	glVertexAttribPointer(myColAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) 12);

	glEnableVertexAttribArray(myTexAttrib);
	glVertexAttribPointer(myTexAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) (7*4));

//	glDrawArrays(GL_TRIANGLE_STRIP, 0, myWindowViewAreaVertices.size());
	glDisable(GL_SCISSOR_TEST);







check_gl_errors("draw");
}

