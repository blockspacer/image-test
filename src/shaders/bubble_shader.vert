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
uniform float widthOfBubbleData;
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
	if (texCoord.x < -0.1) {
		float bubbleId = -texCoord.x - 1.0;
		vec2 xDataPositionInTexture = 
			vec2(((bubbleId * bubbleInfoMembers) + bubbleX)/widthOfBubbleData, 0.5);
		vec2 yDataPositionInTexture =
			vec2(((bubbleId * bubbleInfoMembers) + bubbleY)/widthOfBubbleData, 0.5);

		float xOffset = texture(bubbleData, xDataPositionInTexture).r;
		float yOffset = texture(bubbleData, yDataPositionInTexture).r;

//output degenerate triangle if bubble is entirely offscreen?
		gl_Position = transformation * (position + vec4(xOffset, yOffset, 0.0f, 0.0f));

		outColor = vec4(0.5,0.0,0.5,1.0);
		outTexCoord = vec3(-1.0, 0.0, 0.0);
		return;
	}
	else {
		outColor = vec4(0.67,0.0,0.0, 1.0);
		gl_Position = transformation * position;
		outTexCoord = texCoord;
	}

//	color = vertexColor;

//	    case  -2: outColor = overlay(texture(tex1,  Texcoord), Color); break;


}

)