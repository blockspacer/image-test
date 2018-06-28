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

// the x-coordinate of the texture passed to the shader, if negative, tells the shader program to do something other than texture a triangle
// see shaders/bubble_shader.vert
float encodeId(BubbleId id) {
	return -100.0f - float(id);
}

BubbleId Bubbles::createBubble(GlContext &ctx, float x, float y, float w, float h) {
	// run through bubbles and find first unused one
	BubbleId bubbleId  = myBubbles.size();

	for (size_t i=0; i<myBubbles.size(); i++) {
		if (myBubbles[i].unused) {
			bubbleId = i;
			myBubbles[i].unused = false;
			myBubbles[i].x = x;
			myBubbles[i].y = y;
			myBubbles[i].w = w;
			myBubbles[i].h = h;

			break;
		}
	}	

	if (bubbleId == myBubbles.size()) {
		if (bubbleId >= mySpaceAvailable)
			enlargeBubbleBuffers(ctx);

		myBubbles.emplace_back(x,y,w,h,bubbleId);
		myBubbles[bubbleId].unused = false;
	}

	myBubbleVertices.clear();

	myBubbleVertices.emplace_back(0.0f, 0.0f, encodeId(bubbleId));
	float r = 1.0f;
	for (int i = 0; i < VERTICES_PER_BUBBLE - 1; i++) {
		myBubbleVertices.emplace_back(float(r * cos((2.0*PI / VERTICES_PER_BUBBLE) * i)), float(r * sin((2.0*PI / VERTICES_PER_BUBBLE) * i)), encodeId(bubbleId));
	}

	uploadVertexData(ctx, bubbleId);

	myBubblePositions[bubbleId].x = x;
	myBubblePositions[bubbleId].y = y;
	uploadBubblePositions();



	return bubbleId;
}


void Bubbles::uploadVertexData(GlContext &ctx, BubbleId id) {
	
	glBindBuffer(GL_ARRAY_BUFFER, myVertexBuffer);

	glBufferSubData(GL_ARRAY_BUFFER, id * VERTICES_PER_BUBBLE * sizeof(BubbleVertex), VERTICES_PER_BUBBLE * sizeof(BubbleVertex), myBubbleVertices.data());
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



void Bubbles::generateBubbleVertexIndices(size_t first, size_t last) {
	myBubbleIndices.clear();

	int idx = (VERTICES_PER_BUBBLE) * first;
	int firstInBub;

	cout<<"Generating vertex indexes: first vertex "<<first<<"\tlast vertex "<<last<<endl;
	for (size_t i = first; i<last; i++) {
		myBubbleIndices.push_back(idx); // centre of bubble
		++idx;
		firstInBub = idx;
		for (int j = 0; j < VERTICES_PER_BUBBLE - 1; j++) {
			myBubbleIndices.push_back(idx);
			++idx;
		}
		myBubbleIndices.push_back(firstInBub); // last vertex goes over first one
		myBubbleIndices.push_back(0xffff);
	}
}

void Bubbles::enlargeBubbleBuffers(GlContext &ctx) {
	size_t oldSpaceAvailable = mySpaceAvailable;
	mySpaceAvailable *= 2;

	//replace vertex buffer
	ctx.spareHandle =                 myVertexBuffer;
	glBindBuffer(GL_COPY_READ_BUFFER, myVertexBuffer);
	glGenBuffers(1, &myVertexBuffer);
	setupBuffers(ctx);
	glBufferData(GL_ARRAY_BUFFER, mySpaceAvailable * VERTICES_PER_BUBBLE * sizeof(BubbleVertex), nullptr, GL_STATIC_DRAW);
	
	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, oldSpaceAvailable * VERTICES_PER_BUBBLE * sizeof(BubbleVertex));

	glDeleteBuffers(1, &ctx.spareHandle);

	generateBubbleVertexIndices(oldSpaceAvailable, mySpaceAvailable);

	size_t sizeOfOldData = oldSpaceAvailable * (VERTICES_PER_BUBBLE + 2) * sizeof(GLushort);

	ctx.spareHandle =                 myVertexIndices;
	glBindBuffer(GL_COPY_READ_BUFFER, myVertexIndices);

	glGenBuffers(1,                      &myVertexIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myVertexIndices);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeOfOldData, nullptr, GL_STATIC_DRAW);

	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0,0, sizeOfOldData);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeOfOldData, sizeOfOldData, myBubbleIndices.data());
	glDeleteBuffers(1, &ctx.spareHandle);

	setupBuffersInOtherContexts(ctx);
//	create new texture
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
	glBindBuffer(GL_ARRAY_BUFFER, myVertexBuffer);

	glEnableVertexAttribArray(myPositionVarying);
	glVertexAttribPointer(myPositionVarying, 2, GL_FLOAT, GL_FALSE, sizeof(BubbleVertex), (const void*) 0);

	glEnableVertexAttribArray(myBubbleIdVarying);
	glVertexAttribPointer(myBubbleIdVarying, 2,  GL_FLOAT, GL_FALSE, sizeof(BubbleVertex), (const void*) (2*sizeof(GLfloat)));

	glUniform1f(myBubbleInfoTextureWidthUniform, float(mySpaceAvailable * BubblePositionInfoMemberCount));

	//
}

void Bubbles::setupBuffersInOtherContexts(GlContext &ctx) {
	WindowId orig = ctx.currentWindowId();
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myVertexIndices);
	setupBuffers(ctx);

	if (ctx.windowCount() > 1) {
		for (WindowId i = 0; i < ctx.windowCount(); i++) {
			Window &win = ctx.window(i);
			if (! ctx.isCurrentWindow(i) && win.inUse()) {
				ctx.changeWindow(i);

				setupBuffers(ctx);

				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myVertexIndices);
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

	// bubble group data

	glBufferData(GL_ARRAY_BUFFER, mySpaceAvailable * VERTICES_PER_BUBBLE * sizeof(BubbleVertex), nullptr, GL_STATIC_DRAW);
}

void Bubbles::setupSharedContext(GlContext &ctx, WindowId win) {
	glBindVertexArray(ctx.window(win).bubblesVAO);

	setupBuffers(ctx);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myVertexIndices);
	glBindTexture(GL_TEXTURE_2D, myDataTexture);

	commonSetup();
}

void Bubbles::initializeFirstContext(GlContext &ctx) {
	GLuint shader = ctx.shaderHandle();

	//create a dummy bubble positioned off screen, marked unused
	// so the create new bubbles function doesn't have to treat there being none as a special case
	myBubbles.emplace_back(0.0f,0.0f,0.0f,0.0f,0.0f);

	glBindVertexArray(ctx.firstWindow().bubblesVAO);

	glGenBuffers(1, &myVertexBuffer);

	myPositionVarying = glGetAttribLocation(shader, "position");
	myBubbleIdVarying = glGetAttribLocation(shader, "texCoord");

	glGenBuffers(1,                      &myVertexIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myVertexIndices);

	generateBubbleVertexIndices(size_t(0), size_t(1));

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (VERTICES_PER_BUBBLE + 2) * sizeof(GLushort), myBubbleIndices.data(), GL_STATIC_DRAW);

	myBubblePositions.emplace_back(-0.5f, -0.0f);

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



	myBubbleInfoTextureUniform = glGetUniformLocation(shader, "bubbleData");
	myBubbleInfoTextureWidthUniform = glGetUniformLocation(shader, "widthOfBubbleData");
	myDrawDepthUniform = glGetUniformLocation(shader, "drawDepth");
}

void Bubbles::draw(GlContext &ctx, WindowId winId, Workspace& wksp) {
	Window &win = ctx.window(winId);
//	glBindVertexArray(ctx.bubblesVAO);
//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(win.bubblesVAO);
//cout<<"Drawing window "<<win<<endl;
//cout<<"Drawing bubbles with VAO "<<ctx.windows[win].bubblesVAO<<endl;


// what are the top-left and bottom-right corners of the window's viewport?
	// map those to the parts of the window which aren't the panning bar
	// draw all bubbles

	float wh    = win.pixelHeight()
		, pbh   = win.panningBarPixelHeight(wksp)
		, avail = pbh / wh
	;
cout<<"aaa"<<endl;
	Point tl = win.topLeft(wksp)
		, br = win.bottomRight(wksp)
		, c  = win.viewportCenter()
	;

	float w = ::x(br) - ::x(tl)
		, h = ::y(br) - ::y(tl);

	cout<<"W "<<win.viewportPixelSize(wksp)<<endl;
cout<<win.screenunitWidth()<<endl;
cout<<win.pixelWidth()<<endl;

	myTransformationMatrix = mat4(1.0f);
	myTransformationMatrix = translate(myTransformationMatrix, vec3(0.0, -avail, 0.0));

	myTransformationMatrix = scale(myTransformationMatrix, vec3(2.0/w, -(2.0-2*avail)/h, 1));
	myTransformationMatrix = translate(myTransformationMatrix, vec3(-::x(c), -::y(c), 0.0));
	// myTransformationMatrix = translate(myTransformationMatrix, vec3(::x(c), ::y(c), 0.0));

	ctx.setMatrix(myTransformationMatrix);
	//glUniformMatrix4fv(myTransformationUniform, 1, GL_FALSE, glm::value_ptr(myTransformationMatrix));

	glDrawElements(GL_TRIANGLE_FAN, myBubbles.size() * (VERTICES_PER_BUBBLE + 1), GL_UNSIGNED_SHORT, 0);

check_gl_errors("draw");
}
