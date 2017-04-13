#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec3 color;
layout (location = 3) in vec3 offset;
  
uniform mat4 transform;
out vec3 fColor;

void main()
{
    gl_Position = transform * vec4(position + offset, 1.0f);
    fColor = color;
} 
