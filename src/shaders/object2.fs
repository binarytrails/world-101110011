/*
 * @file
 * source: https://learnopengl.com/code_viewer.php?code=lighting/basic_lighting&type=fragment
*/

#version 330 core
out vec4 color;

in vec3 pos;
  
in vec3 norm;    
uniform vec3 lightPos; 
uniform vec3 viewPos;
uniform vec3 lightColor;
in vec3 col;
in float alp;

void main()
{
    // Ambient
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
        
    vec3 result = (ambient + diffuse + specular) * col;
    color = vec4(result, alp);
} 