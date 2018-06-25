STRINGIFY(

precision mediump float;
precision mediump sampler2DArray;

// common inputs
layout(location = 0) in vec4 position;
layout(location = 1) in vec4 color;
layout(location = 2) in vec3 texCoord;

// for text sprite texture shader
layout(location = 3) in vec4 bgColor;

// for bubble shader
//layout(location = 4) in float bubbleId;
uniform float drawDepth;

uniform float widthOfBubbleData;
// width of bubble group data
// width of window data
uniform sampler2D bubbleData;
uniform sampler2D bubbleGroupData;
uniform sampler2DArray spriteSheets;
uniform mat4 transformation;

//uniform float bubbleHaloDepth;
//uniform float bubbleHilightedHaloDepth;

out vec4 outColor;
out vec3 outTexCoord;

// no enums or #defines, seriously?
const float bubbleX = 0.5;
const float bubbleY = 1.5;
const float bubbleW = 2.5;
const float bubbleH = 3.5;
const float bubbleMouseOver = 4.5;
const float bubbleInfoMembers = 6.0;

const float bubbleGroupGradientLeft       = 0.5;
const float bubbleGroupGradientRight      = 1.5;
const float bubbleGroupGradientYIntercept = 2.5;
const float bubbleGroupGradientGradient   = 3.5;
const float bubbleGroupGradientLeftRed    = 4.5;
const float bubbleGroupGradientLeftGreen  = 5.5;
const float bubbleGroupGradientLeftBlue   = 6.5;
const float bubbleGroupGradientRightRed   = 7.5;
const float bubbleGroupGradientRightGreen = 8.5;
const float bubbleGroupGradientRightBlue  = 9.5;
const float bubbleGroupInfoMembers        = 10.0;


void main() {
	// it's part of a texture
	if (-0.1 < texCoord.x) {
		outColor = vec4(0.0, 1.0, 0.0, 1.0);
		gl_Position = transformation * position;
		outTexCoord = texCoord;
		return;
	}
	// -1 => regular colored triangle
	else if (-1.0 == texCoord.x) {
		outColor = color;
		outTexCoord = texCoord;
		vec4 p = position;
		p.z = 0.5;
		gl_Position = transformation * p;
		return;
	}
	// -2 => panning bar bubble
	else if (-2.0 == texCoord.x) {
		/*
		InstanceId
		*/

		vec2 xDataPos = 
		 	vec2(((float(gl_InstanceID) * bubbleInfoMembers) + bubbleX)/widthOfBubbleData, 0.5);
		vec2 yDataPos =
			vec2(((float(gl_InstanceID) * bubbleInfoMembers) + bubbleY)/widthOfBubbleData, 0.5);
		vec2 wDataPos =
			vec2(((float(gl_InstanceID) * bubbleInfoMembers) + bubbleW)/widthOfBubbleData, 0.5);
		vec2 hDataPos =
			vec2(((float(gl_InstanceID) * bubbleInfoMembers) + bubbleH)/widthOfBubbleData, 0.5);
		float x = texture(bubbleData, xDataPos).r;
		float y = texture(bubbleData, yDataPos).r;
		float w = texture(bubbleData, wDataPos).r;
		float h = texture(bubbleData, hDataPos).r;
// float x = -1000.0;
// float y=-1000.0;
// float w=2000.0;
// float h=2000.0;
		float o = 1.8;
		// if (0 == gl_VertexID)		gl_Position = transformation * vec4(x, y, 0.1, 1.0);
		// else if (1 == gl_VertexID)	gl_Position = transformation * vec4(x, y+h, 0.1, 1.0);	
		// else if (2 == gl_VertexID)	gl_Position = transformation * vec4(x+w, y+h, 0.1, 1.0);			
		// else if (3 == gl_VertexID)	gl_Position = transformation * vec4(x+w, y, 0.1, 1.0);			
		if 		(0 == gl_VertexID)	gl_Position = transformation * vec4(x+o,   y,     0.1, 1.0);
		else if (1 == gl_VertexID)	gl_Position = transformation * vec4(x,     y+o,   0.1, 1.0);	
		else if (2 == gl_VertexID)	gl_Position = transformation * vec4(x,     y+h-o, 0.1, 1.0);			
		else if (3 == gl_VertexID)	gl_Position = transformation * vec4(x+o,   y+h,   0.1, 1.0);			
		else if (4 == gl_VertexID)	gl_Position = transformation * vec4(x+w-o, y+h,   0.1, 1.0);			
		else if (5 == gl_VertexID)	gl_Position = transformation * vec4(x+w,   y+h-o, 0.1, 1.0);			
		else if (6 == gl_VertexID)	gl_Position = transformation * vec4(x+w,   y+o,   0.1, 1.0);			
		else if (7 == gl_VertexID)	gl_Position = transformation * vec4(x+w-o, y,     0.1, 1.0);			

		outColor = vec4(0.9, 0.9, 0.9, 0.5); // get from bubble group 
		outTexCoord = texCoord;
		return;

	}
	// it's part of a viewport area bubble
	else {
		float bubbleId = - texCoord.x - 3.0;
		vec2 xDataPositionInTexture = 
			vec2(((bubbleId * bubbleInfoMembers) + bubbleX)/widthOfBubbleData, 0.5);
		vec2 yDataPositionInTexture =
			vec2(((bubbleId * bubbleInfoMembers) + bubbleY)/widthOfBubbleData, 0.5);

		float xOffset = texture(bubbleData, xDataPositionInTexture).r;
		float yOffset = texture(bubbleData, yDataPositionInTexture).r;

		gl_Position = transformation * (position + vec4(xOffset, yOffset, 0.0f, 0.0f));
		// gl_Position = transformation * (position + vec4(-15.0, 0.0, 0.0f, 0.0f));

		outColor = vec4(0.0, 1.0, 0.5, 1.0);
		outTexCoord = texCoord;
		return;
	}

}


)