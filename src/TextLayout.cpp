#include "TextLayout.h"

void TextLayout::test(GlContext &ctx) {
	float px = 0.5f,
			nx = -px,
			py = 0.5f,
			ny = -py,
			z = 0.0f,
			o = 1.0f;

	myVertices.emplace_back(nx, py, z, z);
	myVertices.emplace_back(nx, ny, z, o);
	myVertices.emplace_back(px, py, o, z);

	myVertices.emplace_back(px, py, o, z);
	myVertices.emplace_back(nx, ny, z, o);
	myVertices.emplace_back(px, ny, o, o);

for (int i=0; i<myVertices.size();i++) {
	cout<<myVertices[i].texU<<", "<<myVertices[i].texV<<endl;
}

	uploadVertexData(ctx);
}

void TextLayout::uploadVertexData(GlContext &ctx) {
	if (myVertices.size() > myVertexBufferSize) {
		myVertexBufferSize = 2 * myVertices.size();
		glBufferData(GL_ARRAY_BUFFER, myVertexBufferSize * sizeof(TextSpriteVertex), nullptr, GL_DYNAMIC_DRAW);
	}
	glBufferSubData(GL_ARRAY_BUFFER, 0, myVertices.size() * sizeof(TextSpriteVertex), myVertices.data());
check_gl_errors("text vert upload");
cout<<"tvu "<<myVertexBufferSize<<", "<<myVertices.size()<<endl;
}

void TextLayout::sharedSetup(GlContext &ctx) {
		GLint myPositionVarying = glGetAttribLocation(ctx.shaderProgramHandle, "position");
	glEnableVertexAttribArray(myPositionVarying);
	glVertexAttribPointer(myPositionVarying, 2, GL_FLOAT, GL_FALSE, sizeof(TextSpriteVertex), (const void*) 0);

	GLint myTexCoordVarying = glGetAttribLocation(ctx.shaderProgramHandle, "texCoord");
	glEnableVertexAttribArray(myTexCoordVarying);
	glVertexAttribPointer(myTexCoordVarying, 2, GL_FLOAT, GL_FALSE, sizeof(TextSpriteVertex), (const void*) (8 * sizeof(GLfloat)));
}

void TextLayout::setupOnSharedContext(GlContext &ctx) {
	glBindVertexArray(ctx.currentWindow().textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, myVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, myVertexBufferSize * sizeof(TextSpriteVertex), nullptr, GL_DYNAMIC_DRAW);

	sharedSetup(ctx);
check_gl_errors("post texture drawing");
}

TextTextureAtlas TextLayout::myTextAtlas;

void TextLayout::initOnFirstContext(GlContext &ctx) {
	myTextAtlas.initOnFirstContext(ctx);
	myTextAtlas.createTextureAtlas(ctx);
	myTextAtlas.test();
	
	glBindVertexArray(ctx.window(0).textVAO);

	glGenBuffers(1, &myVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, myVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, myVertexBufferSize * sizeof(TextSpriteVertex), nullptr, GL_DYNAMIC_DRAW);

	test(ctx);
	sharedSetup();
check_gl_errors("post texture test");

}

void TextLayout::draw(GlContext &ctx) {
cout<<"draw text "<<myVertices.size()<<endl;
	glBindVertexArray(ctx.currentWindow().textVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

