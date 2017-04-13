/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#version 330 core

in vec3 pos;
in vec2 texCoord;

out vec4 color;

uniform sampler2D soilTexture;

void main()
{
    // color influenced by axis positions
/*
    uint s = uint(10);
    uint d = uint(30);
    vec3 rgb = vec3(pos.x + s, pos.y + s, pos.z + s);
    rgb = rgb / d;
    color = texture(soilTexture, texCoord); //* vec4(rgb, 1.0f);
*/

    // TODO texture influenced by elevation

    color = texture(soilTexture, texCoord);
}
