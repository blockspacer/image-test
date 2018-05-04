#version 330 core

in vec4 Color;
in vec2 Texcoord;
in float Texunit;

out vec4 outColor;

uniform sampler2D tex0;
uniform sampler2D tex1;
uniform sampler2D tex2;
uniform sampler2D tex3;
uniform sampler2D tex4;
uniform sampler2D tex5;
uniform sampler2D tex6;
uniform sampler2D tex7;
uniform sampler2D tex8;
uniform sampler2D tex9;
uniform sampler2D tex10;
uniform sampler2D tex11;
uniform sampler2D tex12;
uniform sampler2D tex13;
uniform sampler2D tex14;
uniform sampler2D tex15;
uniform sampler2D tex16;
uniform sampler2D tex17;
uniform sampler2D tex18;
uniform sampler2D tex19;
uniform sampler2D tex20;
uniform sampler2D tex21;
uniform sampler2D tex22;
uniform sampler2D tex23;
uniform sampler2D tex24;
uniform sampler2D tex25;
uniform sampler2D tex26;
uniform sampler2D tex27;
uniform sampler2D tex28;
uniform sampler2D tex29;
uniform sampler2D tex30;
uniform sampler2D tex31;


float overlaycalc(float, float);
vec4 overlay(vec4, vec4);

float overlaycalc(float a, float b)
{
	float c;

	if (a < 0.5)
	{
		c = 2*a*b;
	} else {
		c = 1 - 2 * (1-a) * (1-b);
	}
	
	return c;
}

vec4 overlay(vec4 a, vec4 b)
{
	vec4 c;

	c.r = overlaycalc(a.r, b.r);
	c.g = overlaycalc(a.g, b.g);
	c.b = overlaycalc(a.b, b.b);
	c.a = a.a * b.a;

	return c;
}

// http://byteblacksmith.com/improvements-to-the-canonical-one-liner-glsl-rand-for-opengl-es-2-0/
  // return fract(sin(dot(co.xy,vec2(12.9898,78.233))) * 43758.5453);
highp float rand(vec2 co)
{
    highp float a = 12.9898;
    highp float b = 78.233;
    highp float c = 43758.5453;
    highp float dt= dot(co.xy ,vec2(a,b));
    highp float sn= mod(dt,3.14);
    return fract(sin(sn) * c);
}

vec4 dither(vec4 c)
{
	vec4 o;
	float dr;
	float dg;
	float db;

	float rn = rand(gl_FragCoord.xy);

	dr = (rn-0.5)/63.0;

	rn = rand(vec2(gl_FragCoord.x, gl_FragCoord.y+0.013));

	dg = (rn-0.5)/63.0;

	rn = rand(gl_FragCoord.yx);

	db = (rn-0.5)/63.0;

	o.r = clamp(c.r+dr, 0.0, 1.0);
	o.g = clamp(c.g+dg, 0.0, 1.0);
	o.b = clamp(c.b+db, 0.0, 1.0);
	o.a = c.a;

	return o;
}

void main()
{
	switch (int(Texunit))
	{	case 0: outColor = Color; break;
		case 1: outColor = dither(Color); break;
	    case  -1: outColor = overlay(texture(tex0,  Texcoord), Color); break;
	    case  -2: outColor = overlay(texture(tex1,  Texcoord), Color); break;
	    case  -3: outColor = overlay(texture(tex2,  Texcoord), Color); break;
	    case  -4: outColor = overlay(texture(tex3,  Texcoord), Color); break;
	    case  -5: outColor = overlay(texture(tex4,  Texcoord), Color); break;
	    case  -6: outColor = overlay(texture(tex5,  Texcoord), Color); break;
	    case  -7: outColor = overlay(texture(tex6,  Texcoord), Color); break;
	    case  -8: outColor = overlay(texture(tex7,  Texcoord), Color); break;
	    case  -9: outColor = overlay(texture(tex8,  Texcoord), Color); break;
	    case -10: outColor = overlay(texture(tex9,  Texcoord), Color); break;
	    case -11: outColor = overlay(texture(tex10, Texcoord), Color); break;
	    case -12: outColor = overlay(texture(tex11, Texcoord), Color); break;
	    case -13: outColor = overlay(texture(tex12, Texcoord), Color); break;
	    case -14: outColor = overlay(texture(tex13, Texcoord), Color); break;
	    case -15: outColor = overlay(texture(tex14, Texcoord), Color); break;
	    case -16: outColor = overlay(texture(tex15, Texcoord), Color); break;
	    case -17: outColor = overlay(texture(tex16, Texcoord), Color); break;
	    case -18: outColor = overlay(texture(tex17, Texcoord), Color); break;
	    case -19: outColor = overlay(texture(tex18, Texcoord), Color); break;
	    case -20: outColor = overlay(texture(tex19, Texcoord), Color); break;
	    case -21: outColor = overlay(texture(tex20, Texcoord), Color); break;
	    case -22: outColor = overlay(texture(tex21, Texcoord), Color); break;
	    case -23: outColor = overlay(texture(tex22, Texcoord), Color); break;
	    case -24: outColor = overlay(texture(tex23, Texcoord), Color); break;
	    case -25: outColor = overlay(texture(tex24, Texcoord), Color); break;
	    case -26: outColor = overlay(texture(tex25, Texcoord), Color); break;
	    case -27: outColor = overlay(texture(tex26, Texcoord), Color); break;
	    case -28: outColor = overlay(texture(tex27, Texcoord), Color); break;
	    case -29: outColor = overlay(texture(tex28, Texcoord), Color); break;
	    case -30: outColor = overlay(texture(tex29, Texcoord), Color); break;
	    case -31: outColor = overlay(texture(tex30, Texcoord), Color); break;
	    case -32: outColor = overlay(texture(tex31, Texcoord), Color); break;
	}
}