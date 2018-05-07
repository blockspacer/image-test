#include <cmath>

#include "Bubbles.h"
#include "globals.h"

#include <iostream>
using std::cout;
using std::endl;

AllBubbles::AllBubbles() {
	//create a dummy bubble positioned off screen, marked unused
	mBubbles.emplace_back(-100.0f, -100.0f, 10.0f, 10.0f, 0);
}

BubbleId AllBubbles::createBubble(float x, float y, float w, float h) {
	// run through bubbles and find first unused one
	
	// if there isn't one, create at end of list
	BubbleId bubbleId = 0;

	// create the vertices for it

	// put info in the data texture

	mBubbleVertices.emplace_back(0.0f, 0.0f, bubbleId);

	float r = 0.5f;
	int n = 10;
	for (int i = 0; i < n; i++) {

		mBubbleVertices.push_back(BubbleVertex(float(r * cos((2*PI / n) * i)), float(r * sin((2*PI / n) * i)), BubbleId(bubbleId)));

	}
	return bubbleId;
}

void AllBubbles::uploadBubbleVertexDataToContext(GlContext &ctx, BubbleId id) {
//check that buffer is large enough, if not then double its size
	glBufferData(GL_ARRAY_BUFFER, 3*sizeof(BubbleVertex), mBubbleVertices.data(), GL_STATIC_DRAW);
}

void AllBubbles::uploadBubblePositionDataToContext(GlContext &ctx, BubbleId id) {

}


void AllBubbles::setupContext(GlContext &ctx) {
	// compile program here? has to be redone for every context...

	glGenVertexArrays(1, &ctx.bubblesVAO);
	glBindVertexArray(    ctx.bubblesVAO);

	glGenBuffers(1,              &ctx.bubblesVertexBufferHandle);
	glBindBuffer(GL_ARRAY_BUFFER, ctx.bubblesVertexBufferHandle);

	glGenBuffers(1,                      &ctx.bubblesVertexIndexBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.bubblesVertexIndexBufferHandle);

//bubblesDataTextureHandle

	int positionAttribLocation     = glGetAttribLocation(ctx.shaderProgramHandle, "position");
	int bubbleIndexAttribLocatione = glGetAttribLocation(ctx.shaderProgramHandle, "bubbleId");

	glEnableVertexAttribArray(positionAttribLocation);
//	glEnableVertexAttribArray(bubbleIndexAttribLocatione);

	glVertexAttribPointer(positionAttribLocation, 2, GL_FLOAT, GL_FALSE, sizeof(BubbleVertex), (const void*) 0);


}

void AllBubbles::draw(GlContext &ctx) {

//	glBindVertexArray(ctx.bubblesVAO);
glDrawArrays(GL_TRIANGLES, 0, 3);
//		glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_SHORT, 0);
//	glBindVertexArray(0);


}