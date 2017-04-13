/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#version 330 core

in vec3 pos;
in vec3 texCoord;

out vec4 color;

uniform sampler2D soilTexture;

void main()
{
    vec3 white = vec3(255, 255, 255);

    // TODO set as highest from width and height
    float d = 50;
    vec3 rgb = vec3(pos.x / d, pos.y / d, pos.z / d);

    // TODO different soildTextureCoords (2nd arg)
    color = texture(soilTexture, vec2(texCoord.x, texCoord.y));// * vec4(rgb, 1.0f);
}
