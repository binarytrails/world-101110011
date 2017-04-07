/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#version 330 core

in vec3 pos;
out vec4 color;

void main()
{
    vec3 white = vec3(255, 255, 255);
    vec3 rgb   = vec3(clamp(pos.x, 0, 255),
                      clamp(pos.y, 0, 255),
                      clamp(pos.z, 0, 255));
    color      = vec4(rgb, 1.0f);
}
