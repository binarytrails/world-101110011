#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec3 color;

out vec4 vertexColor; // Specify a color output to the fragment shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

void main()
{
    gl_Position = projection * view * model * vec4(position, 1.0f);
    vertexColor = vec4(1.0f, 1.0f, 1.0f, 1.0f);
}