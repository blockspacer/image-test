#include "PanningBar.h"

void PanningBar::draw(GlContext &ctx, WindowId win, Workspace& wksp, Bubbles& bubbles) {
//	glBindVertexArray(ctx.bubblesVAO);
//glDrawArrays(GL_TRIANGLES, 0, 3);
	glBindVertexArray(ctx.window(win).bubblesVAO);
//cout<<"Drawing window "<<win<<endl;
//cout<<"Drawing bubbles with VAO "<<ctx.windows[win].bubblesVAO<<endl;

/*
scale to fit panning bar in window
position it so origin is at (-1,1)
*/

	Point s = wksp.size();
	float scaling = 2.0f / ::x(s);

	myTransformationMatrix = scale(mat4(1.0f), vec3(scaling, -1, 1));
	myTransformationMatrix = translate(myTransformationMatrix, vec3(0.3, 0.3, 0.0));

	ctx.setMatrix(myTransformationMatrix);
	glDrawElements(GL_TRIANGLE_FAN, bubbles.count() * 30, GL_UNSIGNED_SHORT, 0);
//	glBindVertexArray(0);


check_gl_errors("draw");
}

