/**
* @file
* @author Tarik Abou-Saddik
*
*	Basic class definition for our Skybox.
*/

#pragma once

#include <iostream>
#include <string>
#include <algorithm>
#include <vector>

#include <SOIL/SOIL.h>
#include <glm/gtc/type_ptr.hpp>

#include "../Camera.hpp" 
#include "../Window.hpp"
#include "../Shader.hpp"


class Skybox 
{

public:
	Skybox();
	Skybox(std::vector<const GLchar*>& faces);
	~Skybox();

	//! Combines textures into cubemap and returns texture ID.
	void loadCubeMap(std::vector<const GLchar*>& faces);

	//! Renders out our skybox.
	void renderSkybox(Window* window, Camera* camera);

private:

	GLuint skyboxVAO, skyboxVBO, cubeMapID;
	std::vector<const GLchar*> cubeMapFaces;

	Shader* shader;
	Shader* skyboxShader;

};

