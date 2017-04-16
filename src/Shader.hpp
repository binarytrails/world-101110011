/*
 * @file
 * @author Vsevolod (Seva) Ivanov
 * @brief Adapted from http://learnopengl.com
*/

#pragma once

#include <string>
#include <fstream>
#include <sstream>

#include <stdio.h>
  
#include <GL/glew.h>

//! Class to facilitate the shaders integration by wrapping their operations
class Shader
{
    public:
        Shader(const GLchar* vertexPath,
               const GLchar* fragmentPath);
        
        void use();

        GLuint programId;
};
