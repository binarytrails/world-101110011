/**
* @file
* @author Tarik Abou-Saddik
*
* Although the class implementation was my own, the basic functioning
* of the code was copied from LearnOpenGL's article on "Cubemaps"
* URL: https://learnopengl.com/#!Advanced-OpenGL/Cubemaps
*/

#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include <SOIL/SOIL.h>
#include <glm/gtc/type_ptr.hpp>

#include "Camera.hpp" 
#include "Window.hpp"
#include "Shader.hpp"


class Skybox 
{

public:
	//! Default constructor
	Skybox();
	//! Default destructor
	~Skybox();

	//! Combines textures into cubemap and returns texture ID.
	void loadCubeMap(std::vector<const GLchar*>& faces);
	//! Initializes buffers and sends vertices of skybox to GPU.
	void initBuffers();

	//! Renders out our skybox.
	// void renderSkybox(Window* window, Camera* camera);
	void renderSkybox(Window* window, Camera* camera, 
					const glm::mat4& view,
					const glm::mat4& projection);

private:

	//! GL buffer objects.
	GLuint skyboxVAO, skyboxVBO, cubeMapID;
	//! Vector holding paths of cubemap texture.
	std::vector<const GLchar*> cubeMapFaces;

	//! Shader for our Skybox.
	Shader* skyboxShader;

};

