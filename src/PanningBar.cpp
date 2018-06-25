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
			myPanningBarBubbleVertices[i].r = 0.0f;
			myPanningBarBubbleVertices[i].g = 1.0f;
			myPanningBarBubbleVertices[i].b = 0.0f;
			myPanningBarBubbleVertices[i].a = 1.0f;
		}
		myPanningBarBubbleVertices[0].x = -1000.0f;
		myPanningBarBubbleVertices[0].y = -1000.0f;

		myPanningBarBubbleVertices[1].x = 1000.0f;
		myPanningBarBubbleVertices[1].y = -1000.0f;

		myPanningBarBubbleVertices[2].x = 1000.0f;
		myPanningBarBubbleVertices[2].y = 1000.0f;

		myPanningBarBubbleVertices[3].x = -1000.0f;
		myPanningBarBubbleVertices[3].y = 1000.0f;

		glBufferData(GL_ARRAY_BUFFER, sizeof(ColoredVertex) * 8, &myPanningBarBubbleVertices, GL_STATIC_DRAW);

}


void PanningBar::draw(GlContext &ctx, WindowId winid, Workspace& wksp, Bubbles& bubbles) {
	Window& win = ctx.window(winid);
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
	glVertexAttribPointer(myPosAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) 0);

	glEnableVertexAttribArray(myColAttrib);
	glVertexAttribPointer(myColAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) 8);

	glEnableVertexAttribArray(myTexAttrib);
	glVertexAttribPointer(myTexAttrib, 1, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) (6*4));


/*
scale to fit panning bar in window
position it so origin is at (-1,1)
*/
	float horizontalScale = 2 / wksp.width();
	float pbph = win.panningBarPixelHeight(wksp);


	float	verticalScale = (2 * pbph / win.pixelHeight()) / wksp.height();

	Point s = wksp.size();
	float scaling = 2.0f / ::x(s);
	myTransformationMatrix = mat4(1.0f);
	myTransformationMatrix = translate(myTransformationMatrix, vec3(-1.0, 1.0, 0.0));
	myTransformationMatrix = scale(myTransformationMatrix, vec3(horizontalScale, -verticalScale, 1));

	ctx.setMatrix(myTransformationMatrix);
	glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);


	glBindVertexArray(win.panningBarBubbleVAO);

	glBindBuffer(GL_ARRAY_BUFFER, myPanningBarBubbleVertexBuffer);
	glEnableVertexAttribArray(myTexAttrib);
	glVertexAttribPointer(myTexAttrib, 1, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) (6*4));

	// glEnableVertexAttribArray(myColAttrib);
	// glVertexAttribPointer(myColAttrib, 4, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) 8);

	// glEnableVertexAttribArray(myPosAttrib);
	// glVertexAttribPointer(myPosAttrib, 2, GL_FLOAT, GL_FALSE, sizeof(ColoredVertex), (const void*) 0);

	// glActiveTexture(GL_TEXTURE0);
	// glBindTexture(GL_TEXTURE_2D, bubbles.dataTexture());

cout<<"horiz "<<bubbles.count()<<endl;
	glDrawArraysInstanced(GL_TRIANGLE_FAN, 0, 8, bubbles.count());
	// glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

check_gl_errors("draw");
}

