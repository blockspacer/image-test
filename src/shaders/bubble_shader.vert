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
uniform sampler2D allBubbleData;
uniform sampler2DArray spriteSheet;

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
const float bubbleGradientLeft       = 5.5;
const float bubbleGradientRight      = 6.5;
const float bubbleGradientYIntercept = 7.5;
const float bubbleGradientGradient   = 8.5;
const float bubbleGradientLeftRed   = 9.5;
const float bubbleGradientLeftGreen = 10.5;
const float bubbleGradientLeftBlue  = 11.5;
const float bubbleGradientRightRed   = 12.5;
const float bubbleGradientRightGreen = 13.5;
const float bubbleGradientRightBlue  = 14.5;
const float bubbleInfoMembers = 15.0;

void main() {
	if (texCoord.x < -0.1) {
		float bubbleId = -texCoord.x - 1.0;
		vec2 xDataPositionInTexture = 
			vec2(((bubbleId * bubbleInfoMembers) + bubbleX)/widthOfBubbleData, 0.5);
		vec2 yDataPositionInTexture =
			vec2(((bubbleId * bubbleInfoMembers) + bubbleY)/widthOfBubbleData, 0.5);

		float xOffset = texture(allBubbleData, xDataPositionInTexture).r;
		float yOffset = texture(allBubbleData, yDataPositionInTexture).r;

		gl_Position = position + vec4(xOffset, yOffset, 0.0f, 0.0f);

		outColor = vec4(0.5,0.0,0.5,1.0);
		outTexCoord = vec3(-1.0, 0.0, 0.0);
		return;
	}
	else {
		outColor = vec4(0.67,0.0,0.0, 1.0);
		gl_Position = position;
		outTexCoord = texCoord;
	}

//	color = vertexColor;

//	    case  -2: outColor = overlay(texture(tex1,  Texcoord), Color); break;


}

)