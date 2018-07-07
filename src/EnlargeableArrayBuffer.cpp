#include "EnlargeableArrayBuffer.h"



void EnlargeableArrayBuffer::upload(size_t elementIdx, GlContext &ctx, void *data) {
	if (elementIdx >= myElementCount) {
		size_t newSize = myElementCount * 2;
		while (elementIdx >= newSize)
			newSize *= 2;

		mySpareBufferHandle =             myBuffer;
		glBindBuffer(GL_COPY_READ_BUFFER, myBuffer);
		glGenBuffers(1, &myBuffer);
		bindBuffer();

		glBufferData(GL_ARRAY_BUFFER, myElementSize * newSize, nullptr, GL_STATIC_DRAW);
	
		glCopyBufferSubData(GL_COPY_READ_BUFFER, GL_ARRAY_BUFFER, 0, 0, myElementSize * myElementCount);
		myElementCount = newSize;

		glDeleteBuffers(1, &mySpareBufferHandle);

		ctx.callFromEachContext(myVAOsetupFunc);
	}
	else
		bindBuffer();

	glBufferSubData(GL_ARRAY_BUFFER, elementIdx * myElementSize, myElementSize, data);

	myHighestInUse = elementIdx;


}



