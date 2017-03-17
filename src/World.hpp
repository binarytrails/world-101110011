/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#pragma once

#include <stdio.h>

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.hpp"
#include "WindowCallback.hpp"

#include "Shader.hpp"
#include "Camera.hpp"

#include "Terrain.hpp"

class World
{
    public:
        World();
        ~World();

        void draw();

    private:
        void setWindowContext();
        void setWindowCallbacks();

        void updateMVP();

        Window *window;
        Shader *shader;
        Camera *camera;

        GLuint vboId,
               vaoId,
               eboId;

        GLenum renderMode;

        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

        Terrain *terrain;
};
