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

    // TODO set as highest from width and height
    float d = 50;
    vec3 rgb = vec3(pos.x / d, pos.y / d, pos.z / d);

    color = vec4(rgb, 1.0f);
}
