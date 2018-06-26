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
void drawCornerStrip(Point center, Point xAxis, Point yAxis, float innerRadius, float outerRadius, size_t steps, bool doubleFirstPoint, std::function<void(Point v, float io)> func, float startAngle = 0.0f, float stopAngle = PI / 2.0f) {

	float innerAngleStep = PI / (2.0f * steps + 2)
		, outerAngleStep = (stopAngle - startAngle) / (steps + 1)
		, innerAngle = 0.0f
		, outerAngle = startAngle

	;

	Point p = center + innerRadius * xAxis;
	func(p, 0.0f);
	if (doubleFirstPoint) {
		func(p, 0.0f);
func(p, 0.0f);
}
	float a = startAngle;

	for (size_t i = 0; i <= steps; ++i) {
		func(center + outerRadius * (float(sin(outerAngle))*yAxis + float(cos(outerAngle))*xAxis), 1.0f);
		innerAngle += innerAngleStep;
		func(center + innerRadius * (float(sin(innerAngle))*yAxis + float(cos(innerAngle))*xAxis), 0.0f);
		outerAngle += outerAngleStep;
	}

	func(center + outerRadius * (float(sin(outerAngle))*yAxis + float(cos(outerAngle))*xAxis), 1.0f);

}

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
r = l+40;
t = 20;
b=t+50;
Point c {l,t};
c+=Point(r,b);
c/=2.0f;
	auto lam = [&](Point p, float io) {
		myWindowOutlineVertices.emplace_back(::x(p), ::y(p), 0.0f, cr, cg, cb, 1.0f, -3.0f, io);
		if (io == 0.0f) {
			myWindowViewAreaVertices.emplace_back(::x(p), ::y(p), 0.0f, cr, cg, cb, 0.3f, -1.0f, io);
			myWindowViewAreaVertices.emplace_back(::x(c), ::y(c), 0.0f, cr, cg, cb, 0.3f, -1.0f, io);

		}
	};

	float in = 3, out = 6;

	drawCornerStrip(Point(r-in,b-in), Point(1,0), Point(0,1), in, out, 3, true, lam);
//	myWindowOutlineVertices.emplace_back(r, b, 0.0f, cr, cg, cb, 0.0f, -3.0f, 0.0f);
//	myWindowOutlineVertices.emplace_back(l, b+6, 0.0f, cr, cg, cb, 0.0f, -3.0f, 1.0f);
	drawCornerStrip(Point(l+in,b-in), Point(0,1), Point(-1,0), in, out, 3, false, lam);

	drawCornerStrip(Point(l+in,t+in), Point(-1,0), Point(0,-1), in, out, 3, false, lam);

	drawCornerStrip(Point(r-in,t+in), Point(0,-1), Point(1,0), in, out, 3, false, lam);

	myWindowOutlineVertices.emplace_back(r, b-in, 0.0f, cr, cg, cb, 0.0f, -3.0f, 0.0f);
	myWindowOutlineVertices.emplace_back(r-in+out, b-in, 0.0f, cr, cg, cb, 0.0f, -3.0f, 1.0f);

	myWindowOutlineVertices.emplace_back(r-in+out, b-in, 0.0f, cr, cg, cb, 0.0f, -3.0f, 1.0f);
	myWindowOutlineVertices.emplace_back(r-in+out, b-in, 0.0f, cr, cg, cb, 0.0f, -3.0f, 1.0f);




l = 250;
r = l+40;
t =45;
b=t+50;
c=Point(l,t);
c+=Point(r,b);
c/=2.0f;

	auto lam2 = [&](Point p, float io) {
		myWindowOutlineVertices.emplace_back(::x(p), ::y(p), 0.0f, cr, cg, cb, 1.0f, -3.0f, io);
		if (io == 0.0f) {
			myWindowViewAreaVertices.emplace_back(::x(p), ::y(p), 0.0f, cr, cg, cb, 0.3f, -1.0f, io);
			myWindowViewAreaVertices.emplace_back(::x(c), ::y(c), 0.0f, cr, cg, cb, 0.3f, -1.0f, io);

		}
	};


	drawCornerStrip(Point(r-in,b-in), Point(1,0), Point(0,1), in, out, 3, true, lam2);
//	myWindowOutlineVertices.emplace_back(r, b, 0.0f, cr, cg, cb, 0.0f, -3.0f, 0.0f);
//	myWindowOutlineVertices.emplace_back(l, b+6, 0.0f, cr, cg, cb, 0.0f, -3.0f, 1.0f);
	drawCornerStrip(Point(l+in,b-in), Point(0,1), Point(-1,0), in, out, 3, false, lam2);

	drawCornerStrip(Point(l+in,t+in), Point(-1,0), Point(0,-1), in, out, 3, false, lam2);

	drawCornerStrip(Point(r-in,t+in), Point(0,-1), Point(1,0), in, out, 3, false, lam2);

	myWindowOutlineVertices.emplace_back(r, b-in, 0.0f, cr, cg, cb, 0.0f, -3.0f, 0.0f);
	myWindowOutlineVertices.emplace_back(r-in+out, b-in, 0.0f, cr, cg, cb, 0.0f, -3.0f, 1.0f);

	myWindowOutlineVertices.emplace_back(r-in+out, b-in, 0.0f, cr, cg, cb, 0.0f, -3.0f, 1.0f);




//	ColoredVertex v {t - w, l - w, 0.0f, cr, cg, cb, 1.0, 3.0, out};

//	myWindowOutline.push_back(v);
	glBindBuffer(GL_ARRAY_BUFFER, myWindowOutlineBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredVertex) * myWindowOutlineVertices.size(), &(myWindowOutlineVertices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, myWindowViewAreaBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredVertex) * myWindowViewAreaVertices.size(), &(myWindowViewAreaVertices[0]), GL_STATIC_DRAW);


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

	glEnable (GL_BLEND); glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_TRIANGLE_STRIP, 0, myWindowOutlineVertices.size());








	glBindBuffer(GL_ARRAY_BUFFER, myWindowViewAreaBuffer);

	glEnableVertexAttribArray(myPosAttrib);
	glVertexAttribPointer(myPosAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) 0);


	glEnableVertexAttribArray(myColAttrib);
	glVertexAttribPointer(myColAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) 12);

	glEnableVertexAttribArray(myTexAttrib);
	glVertexAttribPointer(myTexAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) (7*4));

	glDrawArrays(GL_TRIANGLE_STRIP, 0, myWindowViewAreaVertices.size());







check_gl_errors("draw");
}

