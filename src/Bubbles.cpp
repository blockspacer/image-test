#include <cmath>

#include "globals.h"
#include "Bubbles.h"
#include "Workspace.h"

#include <iostream>
using std::cout;
using std::endl;


Bubbles::Bubbles() {

#ifdef DEBUG
	cout<<"DEBUG defined\n";
#else
	cout<<"DEBUG undefined\n";
#endif

}

// the texture x-coordinate passed to the shader, if negative, tells the shader program to do something other than texture a triangle
// see shaders/bubble_shader.vert
float encodeId(BubbleId id) {
	return -100.0f - float(id);
}

float Bubbles::cornerAngle(float length, float cornerRadius, float bulge) {
	bulge = fmax(1.01, bulge);

	float angle = 2 * atan( ( length / (2 * cornerRadius * (bulge - 1))) );

	if (angle > PI/2)
		angle = PI - angle;

	return fmin(angle, PI/4);
}

double Bubbles::sideRadius(float outerCornerRadius, float length, float angle) {
	 return outerCornerRadius +  (length / (2 * sin (angle)));
}

double Bubbles::centerDistance(float length, float angle) {
	return length / (2 * tan (angle));
}

float Bubbles::shrinkInnerCornerRadius(float inner, float l, float t, float r, float b) {
	float in = inner;
	if (r-l < 2*in)
		in = fmin(in, (r - l) / 2.0f);
	if (b-t < 2*in)
		in = fmin(in, (b - t) / 2.0f);
	return in;
}


BubbleId Bubbles::createBubble(GlContext &ctx, float x, float y, float w, float h) {
	// run through bubbles and find first unused one
	BubbleId bubbleId  = myBubbles.size();


	for (size_t i=0; i<myBubbles.size(); i++) {
		if (myBubbles[i].unused) {
			myBubbles[i].unused = false;
			bubbleId = i;
			myBubbles[i].x = x;
			myBubbles[i].y = y;
			myBubbles[i].w = w;
			myBubbles[i].h = h;
			myBubbles[i].groupId = i;

			break;
		}
	}	

	if (bubbleId == myBubbles.size()) {
		if (bubbleId >= mySpaceAvailable)
			enlargeBubbleBuffers(ctx);

		myBubbles.emplace_back(x,y,w,h,bubbleId);
		myBubbles[bubbleId].unused = false;
	}


	Color col {0.0f, 0.6f, 1.0f, 1.0f};

	float in = INNER_CORNER, out = OUTER_CORNER;//8

	float l = 0, t = 0, r = w, b = h;

	float oout = out - in;
	in = shrinkInnerCornerRadius(in, l, t, r, b);
	out = oout + in;

	float  topLength = (r-l-2*in);
	float sideLength = (b-t-2*in);

	float  topAngle = cornerAngle( topLength, out, SIDE_BULGE);
	float sideAngle = cornerAngle(sideLength, out, SIDE_BULGE);
	
	double  topRad = sideRadius(out,  topLength,  topAngle);
	double sideRad = sideRadius(out, sideLength, sideAngle);

	double  topCenterDist = centerDistance( topLength,  topAngle);
	double sideCenterDist = centerDistance(sideLength, sideAngle);

	myBubbleVertices.clear();
	auto lam = [&](Point p, float io) {
		myBubbleVertices.emplace_back(::x(p), ::y(p), encodeId(bubbleId), io);
	};

	GlContext::drawCurvedOutline(0, 0, w, h, in, out, lam, STEPS_PER_BUBBLE_CORNER, STEPS_PER_BUBBLE_SIDE, topLength, sideLength, topAngle, sideAngle, topRad, sideRad, topCenterDist, sideCenterDist);

	myBubbleHalos.upload(bubbleId, ctx, myBubbleVertices.data());

	myBubblePositions[bubbleId].x = x;
	myBubblePositions[bubbleId].y = y;
	myBubblePositions[bubbleId].w = w;
	myBubblePositions[bubbleId].h = h;
	myBubblePositions[bubbleId].bubbleId = bubbleId;
	myBubblePositions[bubbleId].groupId = bubbleId;
	uploadBubblePositions();

	return bubbleId;
}

void Bubbles::uploadBubblePositions() {
	glActiveTexture(GL_TEXTURE0);

	glTexSubImage2D(GL_TEXTURE_2D, //target
		0, // mipmap level
		0, // x offset
		0, // y offset
		mySpaceAvailable * BubblePositionInfoMemberCount,// width
		1,		// height
		GL_RED,	// format
		GL_FLOAT,// type
		myBubblePositions.data());

	// glTexImage2D(GL_TEXTURE_2D, // target
	// 	0,		// mipmap level
	// 	GL_R32F,// internal format
	// 	mySpaceAvailable * BubblePositionInfoMemberCount,// width
	// 	1,		// height
	// 	0,		// border -- unused
	// 	GL_RED,	// format
	// 	GL_FLOAT,// type
	// 	myBubblePositions.data());
}

void Bubbles::setGroupGradient(BubbleGroupId id, Point topLeft, Point bottomRight, Color topLeftColor, Color bottomRightColor) {
	myBubbleGroupInfoTexture[id].gradientLeftRed   = topLeftColor.  redFloat();
	myBubbleGroupInfoTexture[id].gradientLeftGreen = topLeftColor.greenFloat();
	myBubbleGroupInfoTexture[id].gradientLeftBlue  = topLeftColor. blueFloat();

cout<<"rgb "<<topLeftColor.  redFloat()<<", "<<topLeftColor.greenFloat()<<", "<<topLeftColor. blueFloat()<<", "<<endl;
}


void Bubbles::enlargeBubbleBuffers(GlContext &ctx) {
	size_t oldSpaceAvailable = mySpaceAvailable;
	mySpaceAvailable *= 2;

	//replace vertex buffer
/*	ctx.spareHandle =                 myVertexBuffer;
	glBindBuffer(GL_COPY_READ_BUFFER, myVertexBuffer);
	glGenBuffers(1, &myVertexBuffer);
	setupBuffers(ctx);
	glBufferData(GL_ARRAY_BUFFER, mySpaceAvailable * VERTICES_PER_BUBBLE * sizeof(BubbleVertex), nullptr, GL_STATIC_DRAW);
	
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, oldSpaceAvailable * VERTICES_PER_BUBBLE * sizeof(BubbleVertex));

	glDeleteBuffers(1, &ctx.spareHandle);
*/


//	generateBubbleVertexIndices(oldSpaceAvailable, mySpaceAvailable);

/*
	size_t sizeOfOldData = oldSpaceAvailable * (VERTICES_PER_BUBBLE + 2) * sizeof(GLushort);

	ctx.spareHandle =                 myVertexIndices;
	glBindBuffer(GL_COPY_READ_BUFFER, myVertexIndices);

	glGenBuffers(1,                      &myVertexIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myVertexIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeOfOldData, nullptr, GL_STATIC_DRAW);

	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0,0, sizeOfOldData);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeOfOldData, sizeOfOldData, myBubbleIndices.data());
	glDeleteBuffers(1, &ctx.spareHandle);
*/

	setupBuffersInOtherContexts(ctx);
//	create new texture
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myDataTexture);
	glTexImage2D(GL_TEXTURE_2D, // target
		0,		// mipmap level
		GL_R32F,// internal format
		mySpaceAvailable * BubblePositionInfoMemberCount,// width
		1,		// height
		0,		// border -- unused
		GL_RED,	// format
		GL_FLOAT,// type
		0);//myBubblePositions.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	myBubblePositions.resize(mySpaceAvailable);
}






















// void Bubbles::createBuffers() {

// }

// void Bubbles::enlargeBubbleBuffers() {
	
// }

void Bubbles::setupBuffers(GlContext &ctx) {
//	glBindBuffer(GL_ARRAY_BUFFER, myVertexBuffer);

//	glEnableVertexAttribArray(myPositionVarying);
//	glVertexAttribPointer(myPositionVarying, 2, GL_FLOAT, GL_FALSE, sizeof(BubbleVertex), (const void*) 0);

//	glEnableVertexAttribArray(myBubbleIdVarying);
//	glVertexAttribPointer(myBubbleIdVarying, 2,  GL_FLOAT, GL_FALSE, sizeof(BubbleVertex), (const void*) (2*sizeof(GLfloat)));

	glUniform1f(myBubbleInfoTextureWidthUniform, float(mySpaceAvailable * BubblePositionInfoMemberCount));


	glUniform1i(myBubbleInfoTextureUniform, 0);
	glUniform1i(myBubbleGroupInfoTextureUniform, 1);

	myBubbleGroupInfoTexture.setWidthUniform();

	//
}

void Bubbles::setupBuffersInOtherContexts(GlContext &ctx) {
	WindowId orig = ctx.currentWindowId();
	
	setupBuffers(ctx);

	if (ctx.windowCount() > 1) {
		for (WindowId i = 0; i < ctx.windowCount(); i++) {
			Window &win = ctx.window(i);
			if (! ctx.isCurrentWindow(i) && win.inUse()) {
				ctx.changeWindow(i);

				setupBuffers(ctx);
			}
		}

		ctx.changeWindow(orig);
	}
}

void Bubbles::commonSetup() {

	glUniform1i(myBubbleInfoTextureUniform, 0);
	glUniform1f(myBubbleInfoTextureWidthUniform, float(mySpaceAvailable * BubblePositionInfoMemberCount));

	// bubble data
	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myDataTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBufferData(GL_ARRAY_BUFFER, mySpaceAvailable * VERTICES_PER_BUBBLE * sizeof(BubbleVertex), nullptr, GL_STATIC_DRAW);

	myBubbleGroupInfoTexture.setupSubsequentContexts();
	glUniform1i(myBubbleGroupInfoTextureUniform, 1);


	// bubble group data
}

void Bubbles::setupSharedContext(GlContext &ctx, WindowId winid) {

	myBubbleHalos.setup(ctx.window(winid));

	glBindVertexArray(ctx.window(winid).bubblesVAO);

	setupBuffers(ctx);

	glBindTexture(GL_TEXTURE_2D, myDataTexture);

	commonSetup();
}

void Bubbles::initializeFirstContext(GlContext &ctx) {

	GLuint shader = ctx.shaderHandle();


	//create a dummy bubble positioned off screen, marked unused
	// so the function to create new bubbles doesn't have to treat there being none as a special case
	myBubbles.emplace_back(0.0f,0.0f,0.0f,0.0f,0.0f);

	myPositionVarying = glGetAttribLocation(shader, "position");
	myBubbleIdVarying = glGetAttribLocation(shader, "texCoord");

	myBubbleInfoTextureUniform = glGetUniformLocation(shader, "bubbleData");
	myBubbleInfoTextureWidthUniform = glGetUniformLocation(shader, "widthOfBubbleData");
	myDrawDepthUniform      = glGetUniformLocation(shader, "drawDepth");
	myHighlightedBubbleId   = glGetUniformLocation(shader, "highlightedBubbleId");
	myHighlightedBubbleArea = glGetUniformLocation(shader, "highlightedBubbleArea");

	myBubbleGroupInfoTexture.initFirstTime(1, glGetUniformLocation(shader, "widthOfGroupData"));

	myBubbleGroupInfoTextureUniform = glGetUniformLocation(shader, "bubbleGroupData");


	auto bubbleHaloSetupFunc = [&](Window& win) {
		myBubbleHalos.bindBuffer();
		glBindVertexArray(win.bubblesVAO);

		glEnableVertexAttribArray(myPositionVarying);
		glVertexAttribPointer(myPositionVarying, 2, GL_FLOAT, GL_FALSE, sizeof(BubbleVertex), (const void*) 0);

		glEnableVertexAttribArray(myBubbleIdVarying);
		glVertexAttribPointer(myBubbleIdVarying, 2,  GL_FLOAT, GL_FALSE, sizeof(BubbleVertex), (const void*) (2*sizeof(GLfloat)));
	};

	myBubbleHalos.init(bubbleHaloSetupFunc);
	myBubbleHalos.setup(ctx.firstWindow());

	myBubblePositions.emplace_back();

	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1,            &myDataTexture);

	glBindTexture(GL_TEXTURE_2D, myDataTexture);

	glTexImage2D(GL_TEXTURE_2D, // target
		0,		// mipmap level
		GL_R32F,// internal format
		mySpaceAvailable * BubblePositionInfoMemberCount, // width
		1,		// height
		0,		// border -- unused
		GL_RED,	// format
		GL_FLOAT,// type
		//myBubblePositions.data()
		0);

	setupBuffers(ctx);

	commonSetup();
}

void Bubbles::draw(GlContext &ctx, WindowId winId, Workspace& wksp) {
	Window &win = ctx.window(winId);
cout<<"val "<<myBubblePositions[1].groupId<<endl;
	float wh    = win.pixelHeight()
		, pbh   = win.panningBarPixelHeight(wksp)
		, avail = pbh / wh ;

	Point tl = win.topLeft(wksp)
		, br = win.bottomRight(wksp)
		, c  = win.viewportCenter() ;

	float w = ::x(br) - ::x(tl)
		, h = ::y(br) - ::y(tl);

	myTransformationMatrix = mat4(1.0f);
	myTransformationMatrix = translate(myTransformationMatrix, vec3(0.0, -avail, 0.0));

	myTransformationMatrix = scale(myTransformationMatrix, vec3(2.0/w, -(1-avail)*2.0/h, 1));
	myTransformationMatrix = translate(myTransformationMatrix, vec3(-::x(c), -::y(c), 0.0));

	ctx.setMatrix(myTransformationMatrix);

	// if this window has the focus and the mouse is over a bubble halo, 

	glUniform1f(myHighlightedBubbleId, myHighlightedBubble);

	myBubbleGroupInfoTexture.upload();

	cout<<" g "<<myBubbleGroupInfoTexture[0].gradientLeftGreen<<endl;
	cout<<" g "<<myBubbleGroupInfoTexture[1].gradientLeftGreen<<endl;

	cout<<myBubblePositions[0].groupId<<endl;
	cout<<myBubblePositions[1].groupId<<endl;

	glBindVertexArray(win.bubblesVAO);

myBubbleGroupInfoTexture.bind();

	glDrawArrays(GL_TRIANGLE_STRIP, 0, myBubbleHalos.count() * VERTICES_PER_BUBBLE);

	check_gl_errors("Bubble draw.");
}







bool contains(const float x, const float min, const float max, float margin) {
	return min - margin <= x && x < max + margin;
}

bool contains(const Point &p, const Point &topLeft, const Point &bottomRight, float margin) {
	return contains(::x(p), ::x(topLeft), ::x(bottomRight), margin)
		&& contains(::y(p), ::y(topLeft), ::y(bottomRight), margin);
}

bool contains(const Point &p, float left, float top, float right, float bottom, float margin) {
	return contains(::x(p), left, right , margin)
		&& contains(::y(p), top , bottom, margin);
}


bool mouseOverBubbleContent(Point p, Bubble& bub) {
	// todo: inner corners?
	return contains(p, bub.x, bub.y, bub.x+bub.w, bub.y+bub.h);
}

bool mousePossiblyOverBubbleHalo(Point p, Bubble& bub) {
	return contains(p, bub.x, bub.y, bub.x+bub.w, bub.y+bub.h, SIDE_BULGE*OUTER_CORNER);
}

bool mouseOverBubbleHalo(Point p, Bubble& bub) {
	// if it's over one of the four cardinal directions, compute distance
	// if it's over a corner, compute angle
		// if it's still in he domain of one of the sides, compute distance
		// elsewise, calc distance from corner

	// consistency is the main thing
}






bool Bubbles::mouseMotion(Point pos, GlContext &ctx, Workspace &wksp, RedrawRequests &redrawReqests) {
	redrawReqests.redrawAllWindows();

	// if a menu is open, highlight selected option if it's changed, or mouse has moved over menu

	// if it's over bubble content, check if it's still over it
		// pass on motion to contents controller

	// if it's over halo
		// check if it's over contents now
		// if bubble is not part of group, check if it's still over halo

	// go through all bubble groups, check for presence (may be over multiple)
		// for each bubble in group, check if in possible contact
			// if over halo, record bubble and distance

	// if situation has changed, update changes, request redraw

	myHighlightedBubble = -1;
	for (auto bub : myBubbles) {
		if ( ! mousePossiblyOverBubbleHalo(pos, bub))
			continue;

		if (contains(pos, bub.x, bub.y, bub.x+bub.w, bub.y+bub.h)) {
			myHighlightedBubble = bub.id;
			cout<<bub.id<<endl;
		}

	}

	// if a menu is open, highlight selected option if it's changed, or mouse has moved over menu

	// if it's over bubble content, check if it's still over it
		// pass on motion to contents controller

	// if it's over halo
		// check if it's over contents now
		// if bubble is not part of group, check if it's still over halo

	// go through all bubble groups, check for presence (may be over multiple)
		// for each bubble in group, check if in possible contact

			// if over halo, record bubble and distance

	// if situation has changed, update changes, request redraw

	return true;

}





