STRINGIFY(

layout(location = 0) in vec4 vPosition;
layout(location = 1) in vec4 vertexColor;

out vec4 fragmentColor;

void main() {
	gl_Position = vPosition;
	fragmentColor = vertexColor;
}

)