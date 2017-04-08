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
    vec3 rgb   = vec3(clamp(pos.x+50, 50, 255),
                      clamp(pos.y+50, 50, 255),
                      clamp(pos.z+50, 50, 255));
    color      = vec4(rgb, 1.0f);
}
