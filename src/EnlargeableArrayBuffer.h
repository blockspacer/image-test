#pragma once

#include <functional>

#include "GlContext.h"
#include "Window.h"

#include "globals.h"

#include <GL/glew.h>

#include <GLFW/glfw3.h>

class EnlargeableArrayBuffer{
	GLuint myBuffer {0}
		,  mySpareBufferHandle;
	size_t myElementCount {1}
		,  myElementSize
		,  myHighestInUse {0};
	bool   myFirstUse {true};
	std::function<void(Window& win)> myVAOsetupFunc;


public:
	EnlargeableArrayBuffer(size_t elementByteSize) : myElementSize(elementByteSize) {}
	void init(std::function<void(Window &win)> VAOsetupFunc) {
		myVAOsetupFunc = VAOsetupFunc;
		glGenBuffers(1, &myBuffer);
		bindBuffer();
		glBufferData(GL_ARRAY_BUFFER, myElementSize * myElementCount, nullptr, GL_STATIC_DRAW);
	};
	~EnlargeableArrayBuffer() { glDeleteBuffers(1, &myBuffer); };
	void bindBuffer() { glBindBuffer(GL_ARRAY_BUFFER, myBuffer); 
		cout<<"mybuffer "<<myBuffer<<endl;
	};
	void upload(size_t element, GlContext &ctx, void *data);
	size_t count()       { return myElementCount; };
	size_t elementSize() { return myElementSize ; };
	void setup(Window &win) { myVAOsetupFunc(win);};

};


template <typename T>
class EnlargeableDataTexture {
	// shader variable to tell it how wide it is
	// 
	vector<T> myElements;
	GLint  myTextureUnit;
	GLuint myTextureHandle;

		// glActiveTexture(GL_TEXTURE0);

public:
	void create(GLint tu) {
		myTextureUnit = tu;

		glActiveTexture(myTextureUnit);

		glGenTextures(1, &myTextureHandle);
		glBindTexture(GL_TEXTURE_2D, myTextureHandle);
	}
	EnlargeableDataTexture() {
		myElements.resize(1);
	};
	~EnlargeableDataTexture() {};
	void ensureSize(size_t idx) {
		if (idx >= myElements.capacity()) {
			size_t newSize = myElements.capacity() * 2;
			while (idx >= newSize)
				newSize *= 2;
			myElements.resize(newSize);

			glActiveTexture(myTextureUnit);
			glTexImage2D(GL_TEXTURE_2D, // target
				0,		// mipmap level
				GL_R32F,// internal format
				newSize * sizeof(T)/sizeof(float),// width
				1,		// height
				0,		// border -- unused
				GL_RED,	// format
				GL_FLOAT,// type
				0);//myBubblePositions.data());
		}
	};
	void set(size_t idx, T val) {
		ensureSize(idx);
		myElements[idx] = val;
	};
/*	T &operator[] (size_t i) {
cout<<"REF operator[]"<<endl;
		ensureSize(i);
		return myElements[i];
	};
	const T &operator[] (size_t i) const {
cout<<"NON-ref ?? operator[]"<<endl;
//		ensureSize(i);
		return myElements[i];
	};
*/
	T& at(size_t i) {
		ensureSize(i);
		return myElements[i];}
	      T& operator[](size_t idx)       { ensureSize(idx); return myElements[idx]; }
	const T& operator[](size_t idx) const { ensureSize(idx); return myElements[idx]; }

	size_t capacity() { return myElements.capacity();}
	size_t elementSize() { return sizeof(T);}

	void upload() {
		glActiveTexture(myTextureUnit);
		glTexSubImage2D(GL_TEXTURE_2D, //target
			0, // mipmap level
			0, // x offset
			0, // y offset
			myElements.capacity() * sizeof(T)/sizeof(float),// width
			1,		// height
			GL_RED,	// format
			GL_FLOAT,// type
			myElements.data());
	};
};
/**/