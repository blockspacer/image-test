#include <cmath>

#include "Bubbles.h"
#include "globals.h"

#include <iostream>
using std::cout;
using std::endl;

AllBubbles::AllBubbles() {
	//create a dummy bubble positioned off screen, marked unused
	mBubbles.emplace_back(0.0f,0.0f,0.0f,0.0f,0.0f);
//	createBubble(-100, -100,10,10);
cout<<"CREATING BUBBLES"<<endl;


	#ifdef DEBUG
cout<<"debug defined\n";
	#else
cout<<"debug undefined\n";
	#endif


}

BubbleId AllBubbles::createBubble(float x, float y, float w, float h) {
	// run through bubbles and find first unused one
	BubbleId bubbleId  = mBubbles.size();

	for (int i=0; i<mBubbles.size(); i++) {
		if (mBubbles[i].unused) {
			bubbleId = i;
			mBubbles[i].unused = false;
			mBubbles[i].x = x;
			mBubbles[i].y = y;
			mBubbles[i].w = w;
			mBubbles[i].h = h;

			cout<<"filling empty bubble slot "<<bubbleId<<endl;
			break;
		}
	}	

	if (bubbleId == mBubbles.size()) {
		mBubbles.emplace_back(x,y,w,h,bubbleId);
		mBubbles[bubbleId].unused = false;
		cout<<"adding another bubble "<<bubbleId<<endl;
	}

	mBubbleVertices.clear();

	mBubbleVertices.emplace_back(0.0f, 0.0f, float(bubbleId));

	float r = 0.2f;
	for (int i = 0; i < VERTICES_PER_BUBBLE - 1; i++) {

		mBubbleVertices.emplace_back(float(r * cos((2.0*PI / VERTICES_PER_BUBBLE) * i)), float(r * sin((2.0*PI / VERTICES_PER_BUBBLE) * i)), float(bubbleId));

	}

	// bubbleId = 1;

	// mBubbleVertices.emplace_back(0.0f, 0.0f, float(bubbleId));

	// for (int i = 0; i < VERTICES_PER_BUBBLE; i++) {

	// 	mBubbleVertices.emplace_back(float(r * cos((2*PI / VERTICES_PER_BUBBLE) * i)), float(r * sin((2*PI / VERTICES_PER_BUBBLE) * i)), float(bubbleId));
	// }


	return bubbleId;
}



void AllBubbles::uploadBubbleVertexDataToContext(GlContext &ctx, BubbleId id) {
//check that buffer is large enough, if not then double its size
	ctx.check_gl_errors("upl");

cout<<"bbcc "<<ctx.bubbleBufferContentsCount<<endl;
cout<<"id   "<<id<<endl;
	if (id + 1 >= ctx.bubbleBufferContentsCount)
		enlargeVertexBuffer(ctx);

//	int offset = 
	glBufferSubData(GL_ARRAY_BUFFER, id * VERTICES_PER_BUBBLE * sizeof(BubbleVertex), VERTICES_PER_BUBBLE * sizeof(BubbleVertex), mBubbleVertices.data());
}

void AllBubbles::uploadBubblePositionDataToContext() {
}



void AllBubbles::setupVertexBuffer(GlContext &ctx, size_t bubbleCount) {

	glBindBuffer(GL_ARRAY_BUFFER, ctx.bubblesVertexBufferHandle);
	glBufferData(GL_ARRAY_BUFFER, bubbleCount * VERTICES_PER_BUBBLE * sizeof(BubbleVertex), nullptr, GL_STATIC_DRAW);

	glEnableVertexAttribArray(ctx.positionAttribLoc);
	glVertexAttribPointer(ctx.positionAttribLoc, 2, GL_FLOAT, GL_FALSE, sizeof(BubbleVertex), (const void*) 0);

	glEnableVertexAttribArray(ctx.bubbleIndexAttribLoc);
	glVertexAttribPointer(ctx.bubbleIndexAttribLoc, 1,  GL_FLOAT, GL_FALSE, sizeof(BubbleVertex), (const void*) (2*sizeof(GLfloat)));
ctx.check_gl_errors("sss");

}

void AllBubbles::generateBubbleVertexIndices(size_t first, size_t last) {
	mBubbleIndices.clear();

	int idx = (VERTICES_PER_BUBBLE) * first;
	int firstInBub;

	cout<<"gen first "<<first<<endl;
	cout<<"gen last "<<last<<endl;
cout<<"v first "<<idx<<endl;
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
cout<<"vertex laaaaaaaaaaaaaaaaaaaaaaast "<<mBubbleIndices.size()<<endl;
	}

}

void AllBubbles::enlargeVertexBuffer(GlContext &ctx) {
	ctx.spareHandle =                 ctx.bubblesVertexBufferHandle;
	glBindBuffer(GL_COPY_READ_BUFFER, ctx.bubblesVertexBufferHandle);

	glGenBuffers(1,              &ctx.bubblesVertexBufferHandle);
	setupVertexBuffer(ctx, ctx.bubbleBufferContentsCount * 2);

	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, ctx.bubbleBufferContentsCount * VERTICES_PER_BUBBLE * sizeof(BubbleVertex));

	glDeleteBuffers(1, &ctx.spareHandle);

	generateBubbleVertexIndices(ctx.bubbleBufferContentsCount, ctx.bubbleBufferContentsCount*2);

	size_t sizeOfOldData = ctx.bubbleBufferContentsCount * (VERTICES_PER_BUBBLE + 2) * sizeof(GLushort);

	ctx.spareHandle =                 ctx.bubblesVertexIndexBufferHandle;
	glBindBuffer(GL_COPY_READ_BUFFER, ctx.bubblesVertexIndexBufferHandle);

	glGenBuffers(1,                      &ctx.bubblesVertexIndexBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.bubblesVertexIndexBufferHandle);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, 2 * sizeOfOldData, nullptr, GL_STATIC_DRAW);

	glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ELEMENT_ARRAY_BUFFER, 0,0, sizeOfOldData);
	glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, sizeOfOldData, sizeOfOldData, mBubbleIndices.data());
	glDeleteBuffers(1, &ctx.spareHandle);

	ctx.bubbleBufferContentsCount *= 2;
}

void AllBubbles::setupSharedContext(GlContext &ctx) {

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.bubblesVertexIndexBufferHandle);
	glBindTexture(GL_TEXTURE_2D, ctx.bubblesDataTextureHandle);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ctx.bubblesDataTextureHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	GLint sampler = glGetUniformLocation(ctx.shaderProgramHandle, "allBubbleData");

	glUniform1i(sampler, 0);
	GLuint textureWidth = glGetUniformLocation(ctx.shaderProgramHandle, "widthOfBubbleData");
	glUniform1f(textureWidth, 60.0f);

	setupVertexBuffer(ctx, ctx.bubbleBufferContentsCount);


}

void AllBubbles::setupContext(GlContext &ctx) {
	// compile program here? has to be redone for every context...

	glGenBuffers(1,              &ctx.bubblesVertexBufferHandle);

	setupVertexBuffer(ctx, 1);

	ctx.positionAttribLoc    = glGetAttribLocation(ctx.shaderProgramHandle, "position");
	ctx.bubbleIndexAttribLoc = glGetAttribLocation(ctx.shaderProgramHandle, "bubbleId");

	glGenBuffers(1,                      &ctx.bubblesVertexIndexBufferHandle);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ctx.bubblesVertexIndexBufferHandle);

	generateBubbleVertexIndices(size_t(0), size_t(1));

	glBufferData(GL_ELEMENT_ARRAY_BUFFER, (VERTICES_PER_BUBBLE + 2) * sizeof(GLushort), mBubbleIndices.data(), GL_STATIC_DRAW);




//	glBufferSubData(GL_ARRAY_BUFFER, 0, VERTICES_PER_BUBBLE * sizeof(BubbleVertex), mBubbleVertices.data());

	ctx.bubbleBufferContentsCount = 1;

	mBubblePositions.emplace_back(-0.5f, -0.0f, -0.0f, -0.0f, 
		0.0f, -1.0f, 1.0f, 
		0.0f, -1.0f,
		0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f);
	mBubblePositions.emplace_back(0.0f, -0.3f, 0.0f, 0.0f, 
		0.0f, -1.0f, 1.0f, 
		0.0f, -1.0f,
		0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f);
	mBubblePositions.emplace_back(0.5f, 0.3f, 0.0f, 0.0f, 
		0.0f, -1.0f, 1.0f, 
		0.0f, -1.0f,
		0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f);
	mBubblePositions.emplace_back(0.5f, -0.7f, 0.0f, 0.0f, 
		0.0f, -1.0f, 1.0f, 
		0.0f, -1.0f,
		0.0f, 0.0f, 0.0f, 
		0.0f, 0.0f, 0.0f);

	glGenTextures(1,            &ctx.bubblesDataTextureHandle);
	glBindTexture(GL_TEXTURE_2D, ctx.bubblesDataTextureHandle);
	glTexImage2D( GL_TEXTURE_2D, 0, GL_R32F, 60, 1, 0, GL_RED, GL_FLOAT, mBubblePositions.data());

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, ctx.bubblesDataTextureHandle);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	GLint sampler = glGetUniformLocation(ctx.shaderProgramHandle, "allBubbleData");
	glUniform1i(sampler, 0);

	GLuint textureWidth = glGetUniformLocation(ctx.shaderProgramHandle, "widthOfBubbleData");

	glUniform1f(textureWidth, 60.0f);

}

void AllBubbles::draw(GlContext &ctx) {
cout<<"Drawing context with VAO "<<ctx.bubblesVAO<<endl;
//	glBindVertexArray(ctx.bubblesVAO);
//glDrawArrays(GL_TRIANGLES, 0, 3);
		glDrawElements(GL_TRIANGLE_FAN, mBubbles.size() * (VERTICES_PER_BUBBLE + 2), GL_UNSIGNED_SHORT, 0);
//	glBindVertexArray(0);

ctx.check_gl_errors("draw");
}