/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#version 330 core

in vec3 pos;
in vec2 texCoord;

out vec4 color;

uniform sampler2D SoilTex;
uniform sampler2D HighTex;

uniform int _color;

void main()
{
    // color influenced by axis positions
    uint s = uint(10);
    uint d = uint(30);
    vec3 rgb = vec3(pos.x + s, pos.y + s, pos.z + s);
    rgb = rgb / d;
    color = vec4(rgb, 1.0f);

/* FIXME
    // Texture influenced by elevation

    float texOpacity = 0.9;
    float overOpacity = 0.7;

    if (_color == 1)
    {
        vec4 texColor;

        if (pos.y > 0.3)
        {
            texColor = texture(HighTex, texCoord);
        }
        else
        {
            texColor = texture(SoilTex, texCoord);
        }

        texColor.w = texOpacity;
        color = texColor;
    }
    else if (_color == 2)
    {
        color = vec4(255, 255, 20, overOpacity);
    }
*/
}
