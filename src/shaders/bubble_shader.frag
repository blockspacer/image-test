STRINGIFY(

precision mediump float;
precision mediump sampler2DArray;

in vec4 outColor;
in vec3 outTexCoord;
uniform sampler2DArray spriteSheets;

// in vec2 Texcoord;
// in float Texunit;

out vec4 col;

void main()
{
	// textured triangle
	if (-0.1 < outTexCoord.x) {
//yyyyyy;oi		col = vec4(1.0,0.0,0.0,1.0);
		col = texture(spriteSheets,  outTexCoord);
		return;
	}

	// regular colored triangle
//	if (-1.0 == outTexCoord.x) {
		col = outColor;
		return;
//	}


	// 
//	    case  -2: outColor = overlay(texture(tex1,  Texcoord), Color); break;
//	    case  -3: outColor = overlay(texture(tex2,  Texcoord), Color); break;
//	    case  -4: outColor = overlay(texture(tex3,  Texcoord), Color); break;
//	    case  -5: outColor = overlay(texture(tex4,  Texcoord), Color); break;
//	    case  -6: outColor = overlay(texture(tex5,  Texcoord), Color); break;
//	    case  -7: outColor = overlay(texture(tex6,  Texcoord), Color); break;
//	    case  -8: outColor = overlay(texture(tex7,  Texcoord), Color); break;
//	    case  -9: outColor = overlay(texture(tex8,  Texcoord), Color); break;
//	    case -10: outColor = overlay(texture(tex9,  Texcoord), Color); break;
//	    case -11: outColor = overlay(texture(tex10, Texcoord), Color); break;
//	    case -12: outColor = overlay(texture(tex11, Texcoord), Color); break;
//	    case -13: outColor = overlay(texture(tex12, Texcoord), Color); break;
//	    case -14: outColor = overlay(texture(tex13, Texcoord), Color); break;
//	    case -15: outColor = overlay(texture(tex14, Texcoord), Color); break;
//	    case -16: outColor = overlay(texture(tex15, Texcoord), Color); break;
//	    case -17: outColor = overlay(texture(tex16, Texcoord), Color); break;
//	    case -18: outColor = overlay(texture(tex17, Texcoord), Color); break;
//	    case -19: outColor = overlay(texture(tex18, Texcoord), Color); break;
//	    case -20: outColor = overlay(texture(tex19, Texcoord), Color); break;
//	    case -21: outColor = overlay(texture(tex20, Texcoord), Color); break;
//	    case -22: outColor = overlay(texture(tex21, Texcoord), Color); break;
//	    case -23: outColor = overlay(texture(tex22, Texcoord), Color); break;
//	    case -24: outColor = overlay(texture(tex23, Texcoord), Color); break;
//	    case -25: outColor = overlay(texture(tex24, Texcoord), Color); break;
//	    case -26: outColor = overlay(texture(tex25, Texcoord), Color); break;
//	    case -27: outColor = overlay(texture(tex26, Texcoord), Color); break;
//	    case -28: outColor = overlay(texture(tex27, Texcoord), Color); break;
//	    case -29: outColor = overlay(texture(tex28, Texcoord), Color); break;
//	    case -30: outColor = overlay(texture(tex29, Texcoord), Color); break;
//	    case -31: outColor = overlay(texture(tex30, Texcoord), Color); break;
//	    case -32: outColor = overlay(texture(tex31, Texcoord), Color); break;
//	}
}

)