/*
* @file 
* @author
* Although the class implementation was my own, the basic functioning
* of the code was copied from LearnOpenGL's article on "Rendering Text"
* URL: https://learnopengl.com/#!In-Practice/Text-Rendering
*/

#pragma once 

#include <freetype2/ft2build.h>
#include FT_FREETYPE_H
#include <stdio.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Shader.hpp"
#include <map>
#include <string>
#include "Window.hpp"

class GUI
{

public:

	GUI();
	~GUI();

	void renderText(std::string text, GLfloat x,
		GLfloat y, GLfloat scale, glm::vec3& color,
		Window* w);

private:

	FT_Library ft;
	FT_Face face;
	glm::mat4 ortho_projection;
	Shader* guiShader;

	GLuint guiVBO, guiVAO;

	struct Character
	{
		GLuint textureID;
		glm::ivec2 Size;
		glm::ivec2 Bearing;
		GLuint Advance;
	};

	std::map<GLchar, Character> Characters;

	void generateCharTextures();
	void initBuffers();

};


