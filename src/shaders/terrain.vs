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

// 2D Random
float random (in vec2 st)
{
    return fract(sin(dot(st.xy, vec2(12.9898,78.233))) * 43758.5453123);
}

// 2D Noise based on Morgan McGuire @morgan3d
// https://www.shadertoy.com/view/4dS3Wd
float noise (in vec2 st)
{
    vec2 i = floor(st);
    vec2 f = fract(st);

    // Four corners in 2D of a tile
    float a = random(i);
    float b = random(i + vec2(1.0, 0.0));
    float c = random(i + vec2(0.0, 1.0));
    float d = random(i + vec2(1.0, 1.0));

    // Smooth Interpolation

    // Cubic Hermine Curve.  Same as SmoothStep()
    vec2 u = f*f*(3.0-2.0*f);
    // u = smoothstep(0.,1.,f);

    // Mix 4 coorners porcentages
    return mix(a, b, u.x) + (c - a)* u.y * (1.0 - u.x) + (d - b) * u.x * u.y;
}

void main()
{
    gl_PointSize = 2.0;

    // noise is applied on non-normalized for default behavior
    vec3 p = _pos;
    p.y = noise(vec2(_pos.x, _pos.z));
    vec4 pos_mvp = projection * view * model * vec4(p, 1.0f);

    gl_Position = pos_mvp;

    pos = _pos;
    texCoord = _texCoord;
	normal = _normal;
}
