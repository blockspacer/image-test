#version 330 core


in vec3  position;
in float texunit;
in vec2  texcoord;
in vec4  color;

out vec4  Color;
out vec2  Texcoord;
out float Texunit;

uniform mat4 trans;
// uniform vec2 fborigin;
// uniform float viewscale;

void main()
{
    Color = color;
    Texcoord = texcoord;
    Texunit = texunit;

    gl_Position = trans * vec4(position.x, position.y, position.z, 1.0);

//    gl_Position = vec4((position.x + fborigin.x) * viewscale * 2.0 / fbsize.x - 1.0,
//                       (position.y + fborigin.y) * viewscale * -2.0 / fbsize.y + 1.0,
//                       position.z / 32768.0, 1.0);
}