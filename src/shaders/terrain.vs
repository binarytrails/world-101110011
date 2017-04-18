/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#version 330 core

layout (location = 0) in vec3 _pos;
layout (location = 1) in vec2 _texCoord;
layout (location = 2) in vec3 _normal;

out vec3 pos;
out vec2 texCoord;
out vec3 normal;

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(_pos, 1.0f);
    gl_PointSize = 2.0;

    // FIXME #3
    //vec4 vpos = view * model * vec4(pos, 1.0f);
    //pos = vpos.xyz;
    pos = _pos;

    texCoord = _texCoord;
	normal = _normal;
}
