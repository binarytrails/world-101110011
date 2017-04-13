/*
 * @file
 * @author Vsevolod (Seva) Ivanov
 * Illumnation source: https://learnopengl.com/code_viewer.php?code=lighting/basic_lighting&type=fragment
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
	vec4 textureColor = texture(soilTexture, texCoord);
	
	float ambientStrength = 0.1f;
    vec3 ambient = ambientStrength * lightColor;
  	
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - pos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;
    
    // Specular
    float specularStrength = 0.5f;
    vec3 viewDir = normalize(viewPos - pos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;  
        
    vec4 result;
	result.x = (ambient + diffuse + specular) * textureColor.x;
	result.y = (ambient + diffuse + specular) * textureColor.y;
	result.z = (ambient + diffuse + specular) * textureColor.z;
	result.w = textureColor.w;
	
	
    color = result;
}
