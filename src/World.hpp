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

// Sound library for the Sound Engine
#include <SFML/Audio.hpp>

#include "Window.hpp"
#include "Shader.hpp"
#include "Camera.hpp"

#include "Terrain.hpp"
#include "Skybox.h"
#include "Forest.h"
#include "Cloud.h"
#include "Wind.h"
#include "GUI.h"

//! Class to contain & control all of the procedural world elements
class World
{
    public:
        World(const uint16_t width, const uint16_t height, const char* name);
        ~World();

        void setRenderMode(const GLenum mode);

        Window*  getWindow();
        Camera*  getCamera();
        Terrain* getTerrain();

        void initOpenGL();
        void initCamera();

        void initSound();
        void toggleSound();

        void build();

        //! GLFW draw loop
        void draw();

        void rotate(const glm::vec3 axes);

        //! For camera to calculate offset movement for mouse position
        GLfloat lastX, lastY;
        bool mousePressed;
        //! For camera controls
        GLfloat pitch;
        GLfloat yaw;
        //! Centering the camera on the terrain
        void centerCamera();
        //! Output text with instructions on screen.
        void outputUI();
        //! Prevent camera from falling below y = 0
        void controlCamPos();

        GLboolean displayText;

    private:
        void setWindowContext();
        void setWindowCallbacks();

        void updateMVP();

        Window *window;
        Shader *shader;
        Camera *camera;
        Skybox *skybox;
        Cloud  *cloud;
        Wind   *wind;
        GUI    *gui;

        GLuint vboId,
               vaoId,
               eboId;

        GLenum renderMode;

        //! Homogeneous coordinates matrices
        glm::mat4 model;
        glm::mat4 view;
        glm::mat4 projection;

        //! infinite terrain camera lock
        glm::mat4 centerView;

        Terrain *terrain;
        Forest* forest;
        const uint16_t TERRAIN_WIDTH;
        const uint16_t TERRAIN_HEIGHT;

        //! Sound Engine
        sf::Sound       bgMusic;
        sf::SoundBuffer bgMusicBuffer;
};

//! GLFW Window context to allow the usage of class instances in callbacks
struct CallbackContext
{
    World *world;
    uint16_t steps;
};

//! Returning a CallbackContext attached to a GLFW window
static CallbackContext* getWindowContext(GLFWwindow* w)
{
    return static_cast<CallbackContext*>(glfwGetWindowUserPointer(w));
}

static CallbackContext callbackContext;

//! GLFW window framebuffer callback
static void framebufferSizeCallback(GLFWwindow* w, int width, int height)
{
    CallbackContext* cbcPtr = getWindowContext(w);

    cbcPtr->world->getWindow()->width(width);
    cbcPtr->world->getWindow()->height(height);

    glViewport(0, 0, width, height);
}

//! Storing window pressed keys for faster processing
static bool keys[256];
//! Applying window keys bindings
static void applyKeyboardBindings(GLFWwindow* w);

//! Only registering as boolean if a key is pressed for faster processing
static void keyCallback(GLFWwindow* w, int key, int scancode, int action, int mode)
{
    if (action == GLFW_PRESS)    keys[key] = true;
    if (action == GLFW_RELEASE)  keys[key] = false;

    // Slower but less sensible to be clicked many times of the keys actions
    CallbackContext* cbcPtr = getWindowContext(w);

    if (keys[GLFW_KEY_T])       cbcPtr->world->setRenderMode(GL_TRIANGLES);
    if (keys[GLFW_KEY_L])       cbcPtr->world->setRenderMode(GL_LINES);
    if (keys[GLFW_KEY_P])       cbcPtr->world->setRenderMode(GL_POINTS);

    if (keys[GLFW_KEY_H])
        cbcPtr->world->displayText = cbcPtr->world->displayText ? false : true;

    if (keys[GLFW_KEY_M])       cbcPtr->world->toggleSound();
}

//! Faster rendering of the keys actions
static void applyKeyboardBindings(GLFWwindow* w)
{
    CallbackContext* cbcPtr = getWindowContext(w);

    if(keys[GLFW_KEY_ESCAPE])   glfwSetWindowShouldClose(w, GL_TRUE);
/*
    if (keys[GLFW_KEY_LEFT])    cbcPtr->world->rotate(glm::vec3(0, 1, 0));
    if (keys[GLFW_KEY_RIGHT])   cbcPtr->world->rotate(glm::vec3(0, -1, 0));
    if (keys[GLFW_KEY_UP])      cbcPtr->world->rotate(glm::vec3(1, 0, 0));
    if (keys[GLFW_KEY_DOWN])    cbcPtr->world->rotate(glm::vec3(-1, 0, 0));
*/
    if (keys[GLFW_KEY_W])       cbcPtr->world->getCamera()->moveUp();
    if (keys[GLFW_KEY_S])       cbcPtr->world->getCamera()->moveDown();
    if (keys[GLFW_KEY_D])       cbcPtr->world->getCamera()->moveLeft();
    if (keys[GLFW_KEY_A])       cbcPtr->world->getCamera()->moveRight();

    if (keys[GLFW_KEY_I])       cbcPtr->world->getCamera()->moveForward();
    if (keys[GLFW_KEY_O])       cbcPtr->world->getCamera()->moveBackward();
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

// FIXME #2 Mouse camera resets to default angle after each new look
static void mousePositionCallback(GLFWwindow* w, double xpos, double ypos)
{
    CallbackContext* cbcPtr = getWindowContext(w);

    if (cbcPtr->world->mousePressed)
    {
        // Delta of position along x-axis.
        GLfloat xoff = xpos - cbcPtr->world->lastX;

        // Delta of positon along y-axis.
        GLfloat yoff = ypos - cbcPtr->world->lastY;

        // Reset our x and y positions for the frame.
        cbcPtr->world->lastX = xpos;
        cbcPtr->world->lastY = ypos;

        // In order to lessen the jerk of mouse movement,
        // we add this sensitivity to offsets.
        GLfloat sensitivity = 0.05f;
        xoff *= sensitivity;
        yoff *= sensitivity;

        cbcPtr->world->yaw += xoff;
        cbcPtr->world->pitch += yoff;

        // Make sure that pitch is in the domain [-89.0, 89.0]
        // (not exactly 90 due to cosine).
        if(cbcPtr->world->pitch > 89.0f)
            cbcPtr->world->pitch = 89.0f;
        if(cbcPtr->world->pitch < -89.0f)
            cbcPtr->world->pitch = -89.0f;

        glm::vec3 at;
        GLfloat pitch = cbcPtr->world->pitch;
        GLfloat yaw = cbcPtr->world->yaw;

        at.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
        at.y = sin(glm::radians(pitch));
        at.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));

        cbcPtr->world->getCamera()->setAt(glm::normalize(at));
    }
}

static void mouseScrollCallback(GLFWwindow *w, double xoff, double yoff)
{
    CallbackContext* cbcPtr = getWindowContext(w);
    /* TODO
     *  Terrain::advance
     *      1. detect direction
     *      2. if going on positive x or z axis then
     *              advance terrain in this direction
     */
    if (yoff > 0)
    {
        //cbcPtr->world->getTerrain()->advance(true);
        cbcPtr->world->getCamera()->moveForward();
    }
    else if (yoff < 0)
    {
        //cbcPtr->world->getTerrain()->advance(false);
        cbcPtr->world->getCamera()->moveBackward();
    }
}
