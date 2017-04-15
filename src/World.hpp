/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#pragma once

#include <stdio.h>
#include <math.h>

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//! Sound library
#include <SFML/Audio.hpp>

#include "Window.hpp"
#include "Shader.hpp"
#include "Camera.hpp"
#include "Terrain.hpp"
#include "Skybox.h"

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

        //! For camera to calculate offset movement for mouse position
        GLfloat lastX, lastY;
        bool mousePressed;

        //! For camera controls
        GLfloat pitch;
        GLfloat yaw;

    private:
        void setWindowContext();
        void setWindowCallbacks();

        void updateMVP();

        Window *window;
        Shader *shader;
        Camera *camera;
        Skybox *skybox;

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

        sf::Sound       bgMusic;
        sf::SoundBuffer bgMusicBuffer;
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

/*
    Note: In order to ensure a smooth input handling from our application,
    a boolean array is created to hold GLFW key values. If a key's particular
    index is 0 (i.e. false), then it is not pressed or has been released by the
    user. If a key's particular index is 1 (i.e true), then the key is pressed.

    This array is then used in conjunction with function doMovement(), which
    effectively carries out the neccesary camera system adjustments based on input.

    This code was taken from www.learnopengl.com.

    TODO: Possible adjustments to camera speed.
*/
static bool keys[256];
static void doMovement(GLFWwindow* w);

// END OF LEARNOPENGL CODE.

static void keyCallback(GLFWwindow* w, int key, int scancode, int action, int mode)
{
    if(action == GLFW_PRESS)
        keys[key] = true;
    else if(action == GLFW_RELEASE)
        keys[key] = false;
}

static void doMovement(GLFWwindow* w)
{
    CallbackContext* cbcPtr = getWindowContext(w);

    if (keys[GLFW_KEY_LEFT])   cbcPtr->world->rotate(glm::vec3(0, 1, 0));
    if (keys[GLFW_KEY_RIGHT])  cbcPtr->world->rotate(glm::vec3(0, -1, 0));
    if (keys[GLFW_KEY_UP])     cbcPtr->world->rotate(glm::vec3(1, 0, 0));
    if (keys[GLFW_KEY_DOWN])   cbcPtr->world->rotate(glm::vec3(-1, 0, 0));

    if (keys[GLFW_KEY_W]) cbcPtr->world->getCamera()->moveUp();
    if (keys[GLFW_KEY_S]) cbcPtr->world->getCamera()->moveDown();
    if (keys[GLFW_KEY_D]) cbcPtr->world->getCamera()->moveLeft();
    if (keys[GLFW_KEY_A]) cbcPtr->world->getCamera()->moveRight();

    if (keys[GLFW_KEY_I]) cbcPtr->world->getCamera()->moveForward();
    if (keys[GLFW_KEY_O]) cbcPtr->world->getCamera()->moveBackward();

    if (keys[GLFW_KEY_T]) cbcPtr->world->setRenderMode(GL_TRIANGLES);
    if (keys[GLFW_KEY_L]) cbcPtr->world->setRenderMode(GL_LINES);
    if (keys[GLFW_KEY_P]) cbcPtr->world->setRenderMode(GL_POINTS);

    // Close window
    if(keys[GLFW_KEY_ESCAPE]) glfwSetWindowShouldClose(w, GL_TRUE);
}

static void mouseKeyCallback(GLFWwindow* w, int key, int action, int mode)
{
    CallbackContext* cbcPtr = getWindowContext(w);

    if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        cbcPtr->world->mousePressed = true;
        cbcPtr->world->getWindow()->cursor(false);
    }
    else if (key == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        cbcPtr->world->mousePressed = false;
        cbcPtr->world->getWindow()->cursor(true);
    }
}

// TODO : Implement mouse control.

static void mousePositionCallback(GLFWwindow* w, double xpos, double ypos)
{
    CallbackContext* cbcPtr = getWindowContext(w);

    if (cbcPtr->world->mousePressed)
    {
        // Delta of position along x-axis.
        GLfloat xoffset = xpos - cbcPtr->world->lastX;

        // Delta of positon along y-axis.
        GLfloat yoffset = ypos - cbcPtr->world->lastY;

        // Reset our x and y positions for the frame.
        cbcPtr->world->lastX = xpos;
        cbcPtr->world->lastY = ypos;

        // In order to lessen the jerk of mouse movement, we add this sensitivity to offsets.
        GLfloat sensitivity = 0.05f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        cbcPtr->world->yaw += xoffset;
        cbcPtr->world->pitch += yoffset;

        // Make sure that pitch is in the domain [-89.0, 89.0] (not exactly 90 due to cosine).
        if(cbcPtr->world->pitch > 89.0f)
            cbcPtr->world->pitch = 89.0f;
        if(cbcPtr->world->pitch < -89.0f)
            cbcPtr->world->pitch = -89.0f;

        GLfloat final_pitch = cbcPtr->world->pitch;
        GLfloat final_yaw = cbcPtr->world->yaw;

        glm::vec3 viewDirection;

        viewDirection.x = cos(glm::radians(final_pitch)) * cos(glm::radians(final_yaw));
        viewDirection.y = sin(glm::radians(final_pitch));
        viewDirection.z = cos(glm::radians(final_pitch)) * sin(glm::radians(final_yaw));

        glm::vec3 normalized =  glm::normalize(viewDirection);

        cbcPtr->world->getCamera()->setAt(normalized);
    }
}

static void mouseScrollCallback(GLFWwindow *w, double xoffset, double yoffset)
{
    uint16_t step = 0;
    const uint16_t step_size = 1;

    CallbackContext* cbcPtr = getWindowContext(w);
    Terrain* terrain = cbcPtr->world->getTerrain();

    /* TODO
     *      1. detect direction
     *      2. if going on positive x or z axis then
     *              advance terrain in this direction
     */

    if (yoffset > 0)
    {
        cbcPtr->steps++;
        step = cbcPtr->steps * step_size;

        //terrain->advance(glm::ivec3(step, 0, step));
        cbcPtr->world->getCamera()->moveForward();
    }
    else if (yoffset < 0)
    {
        cbcPtr->steps--;
        step = cbcPtr->steps * step_size * -1;

        //terrain->advance(glm::ivec3(step, 0, step));
        cbcPtr->world->getCamera()->moveBackward();
    }
}

