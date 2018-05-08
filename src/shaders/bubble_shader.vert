STRINGIFY(

precision mediump float;

layout(location = 0) in vec4 position;
layout(location = 1) in float bubbleId;

uniform float widthOfBubbleData;
uniform sampler2D allBubbleData;
//uniform float bubbleHaloDepth;
//uniform float bubbleHilightedHaloDepth;

//out vec4 color;

void main() {
	vec2 xDataPositionInTexture = vec2(((bubbleId * 13.0) + 0.5)/widthOfBubbleData, 0.5);
	vec2 yDataPositionInTexture = vec2(((bubbleId * 13.0) + 1.5)/widthOfBubbleData, 0.5);

	float xOffset = texture(allBubbleData, xDataPositionInTexture).r;
	float yOffset = texture(allBubbleData, yDataPositionInTexture).r;

	gl_Position = position + vec4(xOffset, yOffset, 0.0f, 0.0f);

//	color = vertexColor;

//	    case  -2: outColor = overlay(texture(tex1,  Texcoord), Color); break;


}

)