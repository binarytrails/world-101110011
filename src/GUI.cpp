/*
* @file
* @author Tarik Abou-Saddik
*/

#include "GUI.h"

GUI::GUI()
{
	if(FT_Init_FreeType(&this->ft))
		printf("ERROR::FreeType could not init FreeType library.\n");

	if(FT_New_Face(this->ft, "assets/fonts/Roboto-Regular.ttf", 0, &face))
		printf("ERROR::FreeType failed to load font.\n");

	//! Define font size
	// Width is dynamically 
	FT_Set_Pixel_Sizes(face,
						 0,  // Width
						 48  // Height
	); 

	this->guiShader = new Shader(
			"src/shaders/gui.vs",  // Vertex shader
			"src/shaders/gui.frag" // Fragment shader.
		);

	this->generateCharTextures();

	this->initBuffers();
}

GUI::~GUI()
{	
	delete guiShader;
	guiShader = nullptr;
}

void GUI::generateCharTextures()
{	
	//! Disable byte alignment restriction.
	glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

	for(GLubyte c = 0; c < 128; ++c)
	{
		// Load character glyph
		if(FT_Load_Char(this->face, c, FT_LOAD_RENDER))
		{
			printf("ERROR::FreeType failed to load Glpyh\n");
			continue;
		}
		// Generate Texture
		GLuint texture;
		glGenTextures(1, &texture);
		glBindTexture(GL_TEXTURE_2D, texture);
		glTexImage2D(
			GL_TEXTURE_2D,
			0,
			GL_RED,
			this->face->glyph->bitmap.width,
			this->face->glyph->bitmap.rows,
			0,
			GL_RED,
			GL_UNSIGNED_BYTE,
			face->glyph->bitmap.buffer
		);	

		// Set texture options
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// Store character in Map for later use.
		Character character = {
			texture,
			glm::ivec2(this->face->glyph->bitmap.width, face->glyph->bitmap.rows),
			glm::ivec2(this->face->glyph->bitmap_left, face->glyph->bitmap_top),
			static_cast<GLuint>(face->glyph->advance.x)
		};
		Characters.insert(std::pair<GLchar, Character>(c, character));
	}

	FT_Done_Face(this->face);
	FT_Done_FreeType(this->ft);
}

void GUI::renderText(std::string text, GLfloat x,
		GLfloat y, GLfloat scale, glm::vec3& color,
		Window* w)
{

	ortho_projection = glm::ortho(0.0f, static_cast<GLfloat>(w->width()), 
			0.0f, static_cast<GLfloat>(w->height()));

	this->guiShader->use();
	glUniformMatrix4fv(glGetUniformLocation(guiShader->programId, "projection"), 1, GL_FALSE, glm::value_ptr(ortho_projection));
	glUniform3f(glGetUniformLocation(guiShader->programId, "textColor"), color.x,
		color.y, color.z);
	glActiveTexture(GL_TEXTURE0);
	glBindVertexArray(guiVAO);

	// Iterate through all characters
	std::string::const_iterator c;
	for(c = text.begin(); c != text.end(); c++)
	{
		Character ch = Characters[*c];

		GLfloat xpos = x + ch.Bearing.x * scale;
		GLfloat ypos = y - (ch.Size.y - ch.Bearing.y) * scale;

		GLfloat w = ch.Size.x * scale;
		GLfloat h = ch.Size.y * scale;
		// Update VBO for each character
		GLfloat vertices[6][4] = 
		{
			{xpos,		ypos+h,	0.0, 0.0},
			{xpos, 		ypos, 	0.0, 1.0},
			{xpos + w, 	ypos, 	1.0, 1.0},

			{xpos, 		ypos+h, 0.0, 0.0},
			{xpos + w, 	ypos, 	1.0, 1.0},
			{xpos + w, 	ypos+h,	1.0, 0.0}
		};

		// Render glyph texture over quad.
		glBindTexture(GL_TEXTURE_2D, ch.textureID);
		// Update content of VBO memory.
		glBindBuffer(GL_ARRAY_BUFFER, this->guiVBO);
		glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		// Render quad
		glDrawArrays(GL_TRIANGLES, 0, 6);
		// Now advance cursors for next glyph
		x += (ch.Advance >> 6) * scale;
	}

	glBindVertexArray(0);
	glBindTexture(GL_TEXTURE_2D, 0);
}

void GUI::initBuffers()
{
	glGenVertexArrays(1, &this->guiVAO);
	glGenBuffers(1, &this->guiVBO);
	glBindVertexArray(this->guiVAO);
		glBindBuffer(GL_ARRAY_BUFFER, guiVBO);
		glBufferData(GL_ARRAY_BUFFER, sizeof(GL_FLOAT) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), 0);
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
}