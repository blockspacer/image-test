STRINGIFY(

precision mediump float;

layout(location = 0) in vec4 position;
layout(location = 1) in float bubbleId;

uniform float widthOfBubbleData;
uniform sampler2D allBubbleData;
//uniform float bubbleHaloDepth;
//uniform float bubbleHilightedHaloDepth;

//out vec4 color;

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
	vec2 xDataPositionInTexture = 
		vec2(((bubbleId * bubbleInfoMembers) + bubbleX)/widthOfBubbleData, 0.5);
	vec2 yDataPositionInTexture =
		vec2(((bubbleId * bubbleInfoMembers) + bubbleY)/widthOfBubbleData, 0.5);

	float xOffset = texture(allBubbleData, xDataPositionInTexture).r;
	float yOffset = texture(allBubbleData, yDataPositionInTexture).r;

	gl_Position = position + vec4(xOffset, yOffset, 0.0f, 0.0f);

//	color = vertexColor;

//	    case  -2: outColor = overlay(texture(tex1,  Texcoord), Color); break;


}

)