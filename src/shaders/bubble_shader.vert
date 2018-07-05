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
const float bubbleGroupId = 5.5;
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

// defined in GlContext.h
const float PB_bubble_layer             = 0.5;
const float bubbleHaloHighlighted_layer = 0.75;
const float bubbleHalo_layer            = 0.6875;


void main() {
	// it's part of a texture
	if (texCoord.x >= 0.0) {
//		outColor = vec4(0.0, 1.0, 0.0, 1.0);
		gl_Position = transformation * position;
		outTexCoord = texCoord;
		return;
	}
	// -1 => regular colored triangle
	else if (-1.0 == texCoord.x) {
		outColor = color;
		outTexCoord = texCoord;
		vec4 p = position;
		gl_Position = transformation * p;
//		gl_Position.z = 0.5625;
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

		float o = 1.8;
		float sm = min(w, h);
		o = min(o, sm/2.0);
		if 		(0 == gl_VertexID)	gl_Position = transformation * vec4(x+o,   y,     PB_bubble_layer, 1.0);
		else if (1 == gl_VertexID)	gl_Position = transformation * vec4(x,     y+o,   PB_bubble_layer, 1.0);
		else if (2 == gl_VertexID)	gl_Position = transformation * vec4(x,     y+h-o, PB_bubble_layer, 1.0);
		else if (3 == gl_VertexID)	gl_Position = transformation * vec4(x+o,   y+h,   PB_bubble_layer, 1.0);
		else if (4 == gl_VertexID)	gl_Position = transformation * vec4(x+w-o, y+h,   PB_bubble_layer, 1.0);
		else if (5 == gl_VertexID)	gl_Position = transformation * vec4(x+w,   y+h-o, PB_bubble_layer, 1.0);
		else if (6 == gl_VertexID)	gl_Position = transformation * vec4(x+w,   y+o,   PB_bubble_layer, 1.0);
		else if (7 == gl_VertexID)	gl_Position = transformation * vec4(x+w-o, y,     PB_bubble_layer, 1.0);

		outColor = vec4(1.0, 0.8, 1.0, 1.0); // get from bubble group 
		outTexCoord = texCoord;
		return;

	}
	// triangle which fades to opacity where texCoord.y equals 0 or 1, and is fully opaque at 0.5
	else if (-3.0 == texCoord.x) {
		outColor = color;
//		outColor = vec4(0.0,1.0,1.0,1.0);
		outTexCoord = texCoord;
		gl_Position = transformation * position;
		return;
	}
	// it's part of a viewport area bubble
	else {
		float bubbleId = - texCoord.x - 100.0;
		vec2 xDataPositionInTexture = 
			vec2(((bubbleId * bubbleInfoMembers) + bubbleX)/widthOfBubbleData, 0.5);
		vec2 yDataPositionInTexture =
			vec2(((bubbleId * bubbleInfoMembers) + bubbleY)/widthOfBubbleData, 0.5);

		float xOffset = texture(bubbleData, xDataPositionInTexture).r;
		float yOffset = texture(bubbleData, yDataPositionInTexture).r;

		gl_Position = transformation * (position + vec4(xOffset, yOffset, bubbleHalo_layer, 0.0f));
		// gl_Position = transformation * (position + vec4(-15.0, 0.0, 0.0f, 0.0f));

		outColor = vec4(0.0, 1.0, 0.5, 1.0);
		outTexCoord = texCoord;
		return;
	}

}


)