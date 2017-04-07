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
    color = vec4(white, 1.0f);
}
