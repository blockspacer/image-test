#include "PanningBar.h"

void PanningBar::setupAttribArray(GLuint vaoHandle, GLuint bufferHandle) {
	if (bufferHandle)
		glBindBuffer(GL_ARRAY_BUFFER, bufferHandle);

	glBindVertexArray(vaoHandle);

	glEnableVertexAttribArray(myPosAttrib);
	glVertexAttribPointer(myPosAttrib, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) 0);

	glEnableVertexAttribArray(myColAttrib);
	glVertexAttribPointer(myColAttrib, 4, GL_UNSIGNED_BYTE, GL_TRUE, sizeof(Vertex), (const void*) 12);

	glEnableVertexAttribArray(myTexAttrib);
	glVertexAttribPointer(myTexAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (const void*) 16);
}

void PanningBar::setupSharedContext(GlContext &ctx, WindowId newId) {
	Window &win = ctx.window(newId);

	setupAttribArray(win.panningBarWindowOutlineVAO, myWindowOutlineBuffer);
	setupAttribArray(win.panningBarWindowViewAreaVAO, myWindowViewAreaBuffer);
	setupAttribArray(win.panningBarBubbleVAO, myPanningBarBubbleVertexBuffer);
	setupAttribArray(win.panningBarBackgroundVAO, myVertexBuffer);

//	if (myWindowOutlineVertices.size())
}

void PanningBar::initializeFirstContext(GlContext &ctx) {
	GLuint shader = ctx.shaderHandle();

	myPosAttrib = glGetAttribLocation(shader, "position");
	myColAttrib = glGetAttribLocation(shader, "color");
	myTexAttrib = glGetAttribLocation(shader, "texCoord");

	Window &win = ctx.firstWindow();

	glGenBuffers(4, &myVertexBuffer);
	// glGenBuffers(1, &myWindowOutlineBuffer);
	// glGenBuffers(1, &myWindowViewAreaBuffer);
	// glGenBuffers(1, &myPanningBarBubbleVertexBuffer);

	setupSharedContext(ctx, win.id());

	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 4, nullptr, GL_STATIC_DRAW);

	Color dimc   {0.03f,  0.0f, 0.03f}
		, lightc {0.3f,   0.0f, 0.3f };

	myBackgroundVertices[0].setColor(dimc);
	myBackgroundVertices[2].setColor(dimc);
	myBackgroundVertices[1].setColor(lightc);
	myBackgroundVertices[3].setColor(lightc);

	for (int i = 0; i < 4; ++i) {
		myBackgroundVertices[i].t = -1.0f;
		myBackgroundVertices[i].z = GlContext::getLayerValue(Layer::PB_background);
	}

	for (int i = 0; i<8; ++i) {
		myPanningBarBubbleVertices[i].t = - 2.0f;
	}

	glBindBuffer(GL_ARRAY_BUFFER, myPanningBarBubbleVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * 8, &myPanningBarBubbleVertices, GL_STATIC_DRAW);
}


void PanningBar::drawWindowOutline(Window& win, Workspace& wksp, const Color &col, float outlineLayer, float viewAreaLayer) {
	Point tl = win.topLeft(wksp),
		  br = win.bottomRight(wksp);
	
	drawWindowOutline(tl, br, win, wksp, col, outlineLayer, viewAreaLayer);
}

void PanningBar::drawWindowOutline(const Point &tl, const Point &br, const Window &win, Workspace& wksp, const Color &col, float outlineLayer, float viewAreaLayer) {

	float t  = ::y(tl), l  = ::x(tl), b  = ::y(br), r  = ::x(br) ;

	Point c {l+r,t+b};
	c/=2.0f;

	float depth = outlineLayer;

	auto lam = [&](Point p, float io) {
		myWindowOutlineVertices.emplace_back(::x(p), ::y(p), depth, col, -3.0f, io);
	};

	myWindowOutlineVertices.clear();
	float in = 3, out = 6;
	GlContext::drawCurvedOutline(l, t, r, b, in, out, lam, 5);

	Color col2 = col;
	col2.setAlpha(0.4f);

	auto tint = [&](float x, float y) {
		myWindowViewAreaVertices.emplace_back(x, y, viewAreaLayer, col2, -1.0f, 0.0f);
	};


	float mid = in / 2.0f;

	myWindowViewAreaVertices.clear();
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

	glBindBuffer(GL_ARRAY_BUFFER, myWindowViewAreaBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * myWindowViewAreaVertices.size(), &(myWindowViewAreaVertices[0]), GL_STATIC_DRAW);

	glBindBuffer(GL_ARRAY_BUFFER, myWindowOutlineBuffer);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * myWindowOutlineVertices.size(), &(myWindowOutlineVertices[0]), GL_STATIC_DRAW);

	glBindVertexArray(win.panningBarWindowViewAreaVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, myWindowViewAreaVertices.size());

	glBindVertexArray(win.panningBarWindowOutlineVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, myWindowOutlineVertices.size());
}

bool contains(const float x, const float min, const float max) {
	return min <= x && x < max;
}

bool contains(const Point &p, const Point &topLeft, const Point &bottomRight) {
	return contains(::x(p), ::x(topLeft), ::x(bottomRight))
		&& contains(::y(p), ::y(topLeft), ::y(bottomRight));
}

bool contains(const Point &p, Window &win, Workspace &wksp) {
	return contains(p, win.topLeft(wksp), win. bottomRight(wksp));
}

void PanningBar::draw(GlContext &ctx, WindowId winid, Workspace& wksp, Bubbles& bubbles) {
	Window& win = ctx.window(winid);

	if (wksp.sizeChangedForPanningBar()) {
		Point tl = wksp.topLeft()
			, br = wksp.bottomRight();

		float l = ::x(tl), r = ::x(br), t = ::y(tl), b = ::y(br);

		myBackgroundVertices[0].setXY(r, b);
		myBackgroundVertices[1].setXY(r, t);
		myBackgroundVertices[2].setXY(l, b);
		myBackgroundVertices[3].setXY(l, t);

		glBindBuffer(GL_ARRAY_BUFFER, myVertexBuffer);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(Vertex)*4, &myBackgroundVertices);

		wksp.sizeHasNotChangedForPanningBar();
	}


	float horizontalScale = 2 / wksp.width();
	float pbph = win.panningBarPixelHeight(wksp);

	float	verticalScale = (2 * pbph / win.pixelHeight()) / wksp.height();

	myTransformationMatrix = mat4(1.0f);
	myTransformationMatrix = translate(myTransformationMatrix, vec3(-1.0, 1.0, 0.0));
	myTransformationMatrix = scale(myTransformationMatrix, vec3(horizontalScale, -verticalScale, 1));

	ctx.setMatrix(myTransformationMatrix);

	glBindVertexArray(win.panningBarBackgroundVAO);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);

	glBindVertexArray(win.panningBarBubbleVAO);
	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 8, bubbles.count());
	// glDrawArrays(GL_TRIANGLE_FAN, 0, 4);



	glEnable (GL_BLEND);
	//glBlendFunc (GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	glBlendFunc(GL_ONE, GL_ONE_MINUS_SRC_ALPHA);
	// glBlendEquationSeparate(GL_FUNC_ADD,GL_FUNC_ADD);
	// glBlendFuncSeparate(GL_ONE,GL_ONE_MINUS_SRC_ALPHA,GL_ONE,GL_ONE_MINUS_SRC_ALPHA);

	glEnable(GL_SCISSOR_TEST);
	glScissor(0, win.pixelHeight() - win.panningBarPixelHeight(wksp), win.pixelWidth(), pbph);

	// draw all open windows other than this window
	// draw this window
	// if mouse isn't over this window, draw a marker around the mouse
	auto drawOthers = [&](Window& otherWin) {
		if (otherWin.id() == win.id() || otherWin.isUnused())
			return;

		drawWindowOutline(otherWin, wksp, Color {0.5f}
			, GlContext::getLayerValue(Layer::PB_otherWindowOutline)
			, GlContext::getLayerValue(Layer::PB_otherWindowViewArea)
			);
	};

	ctx.forEachWindow(drawOthers);

	Color col {0.0f, 0.6f, 1.0f, 1.0f};
	// if window has input focus then brighten // turn green?

	bool mouseOverThisWindow = GlContext::mouseOverWindow(win.id());

	if (mouseOverThisWindow)
		col = Color {0.3f, 0.8f, 1.0f, 1.0f};

	drawWindowOutline(win, wksp, col
		, GlContext::getLayerValue(Layer::PB_currentWindowOutline)
		, GlContext::getLayerValue(Layer::PB_currentWindowViewArea)
		);

	if (mouseOverThisWindow && myMouseOverMe && ! contains(myMousePos, win, wksp)) {
		col = Color {0.7f};
		Point s = win.viewportCMsize(wksp);

		drawWindowOutline(myMousePos - s / 2.0f, myMousePos + s / 2.0f, win, wksp, col
			, GlContext::getLayerValue(Layer::PB_potentialWindowOutline)
			, GlContext::getLayerValue(Layer::PB_potentialWindowViewArea)
			);

	}
	else {
		cout<<"no future"<<endl;
	}

	glDisable(GL_SCISSOR_TEST);


check_gl_errors("draw");
}
















bool operator< (const Point &a, const Point &b) {return ::x(a) <::x(b)  && ::y(a) <::y(b);}
bool operator> (const Point &a, const Point &b) {return ::x(a) >::x(b)  && ::y(a) >::y(b);}
bool operator<=(const Point &a, const Point &b) {return ::x(a) <=::x(b) && ::y(a) <=::y(b);}
bool operator>=(const Point &a, const Point &b) {return ::x(a) >=::x(b) && ::y(a) >=::y(b);}
bool operator==(const Point &a, const Point &b) {return ::x(a) ==::x(b) && ::y(a) ==::y(b);}
bool operator!=(const Point &a, const Point &b) {return ::x(a) !=::x(b) || ::y(a) !=::y(b);}



bool PanningBar::mouseMotion(Point pos, Window &win, GlContext &ctx, Workspace &wksp, Bubbles &bubls, RedrawRequests &redrawReqests) {
	myMousePos = pos;
	redrawReqests.redrawPanningBar(win.id());

	if (myMouseButtonDown) {
		// if mouse is being dragged, move viewport
		win.setViewportCenter(myMousePos + myDragOffset);
	}
	else {
		if (contains(pos, win, wksp)) {
			// mouse is over 
			myMouseOverWindowOutline = true;
			myMouseOverWindowId = win.id();
		}
		else {
			bool foundOne = false;
			auto lambda = [&](Window& potentialWin) {
				if (potentialWin.id() != win.id() 
					&& contains(pos, potentialWin, wksp)) {
					foundOne = true;
				}
			};
			ctx.forEachWindow(lambda);
			myMouseOverWindowOutline = foundOne;
		}
	}

	return true;
}

void PanningBar::mouseButtonInput(Window &win, int button, int action, int mods, GlContext &ctx, PanningBar &pBar, Workspace &wksp, RedrawRequests &redrawQueue) {
	if (action == GLFW_PRESS) {
		if ( ! myMouseButtonDown) {
		// if mouse over self window, work out offset
			if (contains(myMousePos, win, wksp)) {

				myDragOffset = win.viewportCenter() - myMousePos;
				myMouseButtonDown = true;
			}
			else {
				win.setViewportCenter(myMousePos);
				myDragOffset = Point{0,0};
				redrawQueue.redrawAllWindows();
				myMouseButtonDown = true;
			}

		}

	}
	else {
		myMouseButtonDown = false;
		cout<<"release!"<<endl;
	}

}
















