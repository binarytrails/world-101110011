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
#include "Terrain.hpp"

class World
{
    public:
        World(const uint16_t width, const uint16_t height, const char* name);
        ~World();

        void setRenderMode(const GLenum mode);

        Window*  getWindow();
        Camera*  getCamera();
        Terrain* getTerrain();

        void build();
        void draw();

        void rotate(const glm::vec3 axes);

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
        const uint16_t TERRAIN_WIDTH;
        const uint16_t TERRAIN_HEIGHT;
};

// Window context

struct CallbackContext
{
    World *world;
    uint16_t steps;
};

static CallbackContext* getWindowContext(GLFWwindow* w)
{
    return static_cast<CallbackContext*>(glfwGetWindowUserPointer(w));
}

static CallbackContext callbackContext;

// Window callbacks

static void framebufferSizeCallback(GLFWwindow* w, int width, int height)
{
    CallbackContext* cbcPtr = getWindowContext(w);

    cbcPtr->world->getWindow()->width(width);
    cbcPtr->world->getWindow()->height(height);

    glViewport(0, 0, width, height);
}

static void keyCallback(GLFWwindow* w, int key, int scancode, int action, int mode)
{
    //printf("keyboard: %i\n", key);
    CallbackContext* cbcPtr = getWindowContext(w);

    if (key == GLFW_KEY_LEFT)   cbcPtr->world->rotate(glm::vec3(0, 1, 0));
    if (key == GLFW_KEY_RIGHT)  cbcPtr->world->rotate(glm::vec3(0, -1, 0));
    if (key == GLFW_KEY_UP)     cbcPtr->world->rotate(glm::vec3(1, 0, 0));
    if (key == GLFW_KEY_DOWN)   cbcPtr->world->rotate(glm::vec3(-1, 0, 0));

    if (key == GLFW_KEY_W) cbcPtr->world->getCamera()->moveUp();
    if (key == GLFW_KEY_S) cbcPtr->world->getCamera()->moveDown();
    if (key == GLFW_KEY_A) cbcPtr->world->getCamera()->moveLeft();
    if (key == GLFW_KEY_D) cbcPtr->world->getCamera()->moveRight();

    if (key == GLFW_KEY_I) cbcPtr->world->getCamera()->moveForward();
    if (key == GLFW_KEY_O) cbcPtr->world->getCamera()->moveBackward();

    if (key == GLFW_KEY_T) cbcPtr->world->setRenderMode(GL_TRIANGLES);
    if (key == GLFW_KEY_L) cbcPtr->world->setRenderMode(GL_LINES);
    if (key == GLFW_KEY_P) cbcPtr->world->setRenderMode(GL_POINTS);
}

static void mouseKeyCallback(GLFWwindow* w, int key, int action, int mode)
{
    // TODO
}

static void mouseScrollCallback(GLFWwindow *w, double xoffset, double yoffset)
{
    CallbackContext* cbcPtr = getWindowContext(w);
    /* TODO
     *  avance:
     *      1. detect direction
     *      2. if going on positive x or z axis then
     *              advance terrain in this direction
     */

    if (yoffset > 0)
    {
        cbcPtr->world->getTerrain()->advance(true);
        //cbcPtr->world->getCamera()->moveForward();
    }
    else if (yoffset < 0)
    {
        cbcPtr->world->getTerrain()->advance(false);
        //cbcPtr->world->getCamera()->moveBackward();
    }
}

