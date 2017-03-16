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
#include "Shader.hpp"
#include "Camera.hpp"

class World
{
    public:
        World();
        ~World();

        void draw();

    private:

        void setWindowCallbacks();

        void updateMVP();

        void initTerrain();
        void initTerrainBuffers();
        void uploadTerrain();
        void drawTerrain();

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

        std::vector<glm::vec3> terrainVertices;
        std::vector<uint16_t> terrainVerticesI;
};

// Window callbacks

static void key_callback(GLFWwindow *w, int key,
                         int scancode, int action, int mode);

static void mouse_key_callback(GLFWwindow *w, int key, int action, int mode);

static void mouse_scroll_callback(GLFWwindow *w,
                                  double xoffset, double yoffset);

static void framebuffer_size_callback(GLFWwindow* w, int width, int height);

static Window *window;

static Camera *camera;
