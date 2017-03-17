/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#include "Window.hpp"
#include "Camera.hpp"

// Window context

struct CallbackContext
{
    Window *window;
    Camera *camera;
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

    cbcPtr->window->width(width);
    cbcPtr->window->height(height);

    glViewport(0, 0, width, height);
}

static void keyCallback(GLFWwindow* w, int key, int scancode, int action, int mode)
{
    //printf("keyboard: %i\n", key);
    CallbackContext* cbcPtr = getWindowContext(w);

/*
    if (key == GLFW_KEY_LEFT)   mesh->rotate(glm::vec3(0, 1, 0));
    if (key == GLFW_KEY_RIGHT)  mesh->rotate(glm::vec3(0, -1, 0));
    if (key == GLFW_KEY_UP)     mesh->rotate(glm::vec3(1, 0, 0));
    if (key == GLFW_KEY_DOWN)   mesh->rotate(glm::vec3(-1, 0, 0));
*/

    if (key == GLFW_KEY_W) cbcPtr->camera->moveDown();
    if (key == GLFW_KEY_S) cbcPtr->camera->moveUp();
    if (key == GLFW_KEY_A) cbcPtr->camera->moveLeft();
    if (key == GLFW_KEY_D) cbcPtr->camera->moveRight();
}

static void mouseKeyCallback(GLFWwindow* w, int key, int action, int mode)
{
    // TODO
}

static void mouseScrollCallback(GLFWwindow *w, double xoffset, double yoffset)
{
    CallbackContext* cbcPtr = getWindowContext(w);

    if (yoffset > 0)        cbcPtr->camera->moveForward();
    else if (yoffset < 0)   cbcPtr->camera->moveBackward();
}
