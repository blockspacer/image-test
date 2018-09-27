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
		GLint myPositionVarying = glGetAttribLocation(ctx.shaderHandle(), "position");
	glEnableVertexAttribArray(myPositionVarying);
	glVertexAttribPointer(myPositionVarying, 2, GL_FLOAT, GL_FALSE, sizeof(TextSpriteVertex), (const void*) 0);

	GLint myTexCoordVarying = glGetAttribLocation(ctx.shaderHandle(), "texCoord");
	glEnableVertexAttribArray(myTexCoordVarying);
	glVertexAttribPointer(myTexCoordVarying, 2, GL_FLOAT, GL_FALSE, sizeof(TextSpriteVertex), (const void*) (8 * sizeof(GLfloat)));
}

void TextLayout::setupSharedContext(GlContext &ctx) {
	glBindVertexArray(ctx.currentWindow().textVAO);
	glBindBuffer(GL_ARRAY_BUFFER, myVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, myVertexBufferSize * sizeof(TextSpriteVertex), nullptr, GL_DYNAMIC_DRAW);

	myTextAtlas.setupOnContext(ctx);
	sharedSetup(ctx);
check_gl_errors("post texture drawing");
}

TextTextureAtlas TextLayout::myTextAtlas;

void TextLayout::initializeFirstContext(GlContext &ctx) {
	#ifdef NATIVE
		int error = FT_Init_FreeType( &myFreeTypeLibrary );
		if ( error ) {
			cout << BOLD RED "An error occurred during FreeType library initialization, error code: " << error << NORMAL endl;
		}

		FT_Face face;

		error = FT_New_Face(myFreeTypeLibrary, "/usr/share/fonts/TTF/Roboto-Regular.ttf",
			0,
			&face);

		if ( error == FT_Err_Unknown_File_Format )
			cout << BOLD RED "Font file in unknown format" << NORMAL endl;
		else if (error)
			cout << BOLD RED "An error occurred during font loading, error code: " << error << NORMAL endl;

		cout << CYAN "Num glyphs "<<face->size <<NORMAL endl;

	#endif

	myTextAtlas.initOnFirstContext(ctx);

	myTextAtlas.test();
	
	glBindVertexArray(ctx.firstWindow().textVAO);

	glGenBuffers(1, &myVertexBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, myVertexBuffer);
	glBufferData(GL_ARRAY_BUFFER, myVertexBufferSize * sizeof(TextSpriteVertex), nullptr, GL_DYNAMIC_DRAW);

	test(ctx);
	sharedSetup(ctx);
check_gl_errors("post texture test");

}

void TextLayout::draw(GlContext &ctx) {
cout<<"draw text "<<myVertices.size()<<endl;
	glBindVertexArray(ctx.currentWindow().textVAO);
	glDrawArrays(GL_TRIANGLES, 0, 6);
}

