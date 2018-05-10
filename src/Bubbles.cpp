#include <cmath>

#include "Bubbles.h"
#include "globals.h"

#include <iostream>
using std::cout;
using std::endl;

Bubbles::Bubbles() {

}

BubbleId Bubbles::createBubble(GlContext &ctx, float x, float y, float w, float h) {
	// run through bubbles and find first unused one
	BubbleId bubbleId  = mBubbles.size();

	for (size_t i=0; i<mBubbles.size(); i++) {
		if (mBubbles[i].unused) {
			bubbleId = i;
			mBubbles[i].unused = false;
			mBubbles[i].x = x;
			mBubbles[i].y = y;
			mBubbles[i].w = w;
			mBubbles[i].h = h;

			break;
		}
	}	

	if (bubbleId == mBubbles.size()) {
		if (bubbleId >= mySpaceAvailable)
			enlargeBuffers(ctx);

		mBubbles.emplace_back(x,y,w,h,bubbleId);
		mBubbles[bubbleId].unused = false;
	}

	mBubbleVertices.clear();

	mBubbleVertices.emplace_back(0.0f, 0.0f, float(bubbleId));
	float r = 0.2f;
	for (int i = 0; i < VERTICES_PER_BUBBLE - 1; i++) {
		mBubbleVertices.emplace_back(float(r * cos((2.0*PI / VERTICES_PER_BUBBLE) * i)), float(r * sin((2.0*PI / VERTICES_PER_BUBBLE) * i)), float(bubbleId));
	}

	uploadVertexData(ctx, bubbleId);

	mBubblePositions[bubbleId].x = x;
	mBubblePositions[bubbleId].y = y;
	uploadBubblePositions();

	return bubbleId;
}


void Bubbles::uploadVertexData(GlContext &ctx, BubbleId id) {
	
	glBindBuffer(GL_ARRAY_BUFFER, myVertexBuffer);

	glBufferSubData(GL_ARRAY_BUFFER, id * VERTICES_PER_BUBBLE * sizeof(BubbleVertex), VERTICES_PER_BUBBLE * sizeof(BubbleVertex), mBubbleVertices.data());
}

void Bubbles::uploadBubblePositions() {
	glTexSubImage2D(GL_TEXTURE_2D, //target
		0, // mipmap level
		0, // x offset
		0, // y offset
		mySpaceAvailable * BubblePositionInfoMemberCount,// width
		1,		// height
		GL_RED,	// format
		GL_FLOAT,// type
		mBubblePositions.data());

	// glTexImage2D(GL_TEXTURE_2D, // target
	// 	0,		// mipmap level
	// 	GL_R32F,// internal format
	// 	mySpaceAvailable * BubblePositionInfoMemberCount,// width
	// 	1,		// height
	// 	0,		// border -- unused
	// 	GL_RED,	// format
	// 	GL_FLOAT,// type
	// 	mBubblePositions.data());
}



void Bubbles::generateBubbleVertexIndices(size_t first, size_t last) {
	mBubbleIndices.clear();

	int idx = (VERTICES_PER_BUBBLE) * first;
	int firstInBub;

	cout<<"Generating vertex indexes: first vertex "<<first<<"\tlast vertex "<<last<<endl;
	for (size_t i = first; i<last; i++) {
		mBubbleIndices.push_back(idx); // centre of bubble
		++idx;
		firstInBub = idx;
		for (int j = 0; j < VERTICES_PER_BUBBLE - 1; j++) {
			mBubbleIndices.push_back(idx);
			++idx;
		}
		mBubbleIndices.push_back(firstInBub); // last vertex goes over first one
		mBubbleIndices.push_back(0xffff);
	}
}

void Bubbles::enlargeBuffers(GlContext &ctx) {
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
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeOfOldData, sizeOfOldData, mBubbleIndices.data());
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
		0);//mBubblePositions.data());

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

}






















// void Bubbles::createBuffers() {

// }

// void Bubbles::enlargeBuffers() {
	
// }

void Bubbles::setupBuffers(GlContext &ctx) {
cout<<"about"<<endl;
ctx.check_gl_errors("setup same buffers start");

	glBindBuffer(GL_ARRAY_BUFFER, myVertexBuffer);

	glEnableVertexAttribArray(myPositionVarying);
	glVertexAttribPointer(myPositionVarying, 2, GL_FLOAT, GL_FALSE, sizeof(BubbleVertex), (const void*) 0);

	glEnableVertexAttribArray(myBubbleIdVarying);
	glVertexAttribPointer(myBubbleIdVarying, 1,  GL_FLOAT, GL_FALSE, sizeof(BubbleVertex), (const void*) (2*sizeof(GLfloat)));

	glUniform1f(myDataTextureWidthUniform, float(mySpaceAvailable * BubblePositionInfoMemberCount));

ctx.check_gl_errors("setup same buffers end");
}

void Bubbles::setupBuffersInOtherContexts(GlContext &ctx) {
ctx.check_gl_errors("setup other buffers end");
	
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myVertexIndices);
	setupBuffers(ctx);

	if (ctx.windows.size() > 1) {
		for (WindowId i = 0; i < ctx.windows.size(); i++) {
			if (ctx.windows[i].glfwHandle != ctx.pCurrentContext && ! ctx.windows[i].unused) {
				glfwMakeContextCurrent(ctx.windows[i].glfwHandle);

				setupBuffers(ctx);
cout<<"here?"<<endl;
				glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myVertexIndices);
cout<<"gone"<<endl;

			}
		}

		glfwMakeContextCurrent(ctx.pCurrentContext);
cout<<"handl"<<endl;

	}
ctx.check_gl_errors("setup other buffers end");
}

void Bubbles::commonSetup() {
	glUniform1i(mySamplerUniform, 0);
	glUniform1f(myDataTextureWidthUniform, float(mySpaceAvailable * BubblePositionInfoMemberCount));

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, myDataTexture);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	glBufferData(GL_ARRAY_BUFFER, mySpaceAvailable * VERTICES_PER_BUBBLE * sizeof(BubbleVertex), nullptr, GL_STATIC_DRAW);
}

void Bubbles::setupOnSharedContext(GlContext &ctx, WindowId win) {
	glBindVertexArray(ctx.windows[win].bubblesVAO);

	setupBuffers(ctx);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myVertexIndices);
	glBindTexture(GL_TEXTURE_2D, myDataTexture);

	commonSetup();
}

void Bubbles::setupOnFirstContext(GlContext &ctx) {
	//create a dummy bubble positioned off screen, marked unused
	mBubbles.emplace_back(0.0f,0.0f,0.0f,0.0f,0.0f);

	glBindVertexArray(ctx.windows[0].bubblesVAO);

	glGenBuffers(1, &myVertexBuffer);

	myPositionVarying = glGetAttribLocation(ctx.shaderProgramHandle, "position");
	myBubbleIdVarying = glGetAttribLocation(ctx.shaderProgramHandle, "bubbleId");

	glGenBuffers(1,                      &myVertexIndices);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, myVertexIndices);

	generateBubbleVertexIndices(size_t(0), size_t(1));

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (VERTICES_PER_BUBBLE + 2) * sizeof(GLushort), mBubbleIndices.data(), GL_STATIC_DRAW);

	mBubblePositions.emplace_back(-0.5f, -0.0f);

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
		//mBubblePositions.data()
		0);

	setupBuffers(ctx);
	commonSetup();



	mySamplerUniform = glGetUniformLocation(ctx.shaderProgramHandle, "allBubbleData");
	myDataTextureWidthUniform = glGetUniformLocation(ctx.shaderProgramHandle, "widthOfBubbleData");
	
}

void Bubbles::draw(GlContext &ctx, WindowId win) {
//	glBindVertexArray(ctx.bubblesVAO);
//glDrawArrays(GL_TRIANGLES, 0, 3);
	
	glBindVertexArray(ctx.windows[win].bubblesVAO);
//cout<<"Drawing window "<<win<<endl;
//cout<<"Drawing bubbles with VAO "<<ctx.windows[win].bubblesVAO<<endl;
	glDrawElements(GL_TRIANGLE_FAN, mBubbles.size() * (VERTICES_PER_BUBBLE + 2), GL_UNSIGNED_SHORT, 0);
//	glBindVertexArray(0);

ctx.check_gl_errors("draw");
}