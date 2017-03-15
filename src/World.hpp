/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#pragma once

#include <stdio.h>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

class World
{
    public:
        World();
        ~World();

        void draw();

    private:

        Window *window;
        Shader *shader;
        Camera *camera;
        GLuint vboId,
               vaoId,
               eboId;
};
