#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.hpp"
#include "Shader.hpp"
// TODO Mesh class

Window* window;

// Callbacks TODO forward declaration

int main()
{
    window = new Window(1280, 720, "Procedural world");

    // TODO callbacks bindings

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, window->width(), window->height());

    //Shader ourShader("src/shaders/default.vs",
    //                 "src/shaders/default.fs");

    GLuint vboId, vaoId, eboId;

    // TODO

    // Game loop
    while (!glfwWindowShouldClose(window->get()))
    {
        glfwPollEvents();

        // Render TODO class

        // clear the colorbuffer
        glClearColor(0.0f, 0.0f, 0.0f, 0.0f); // background color
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // activate shader
        //ourShader.Use();

        // TODO VMP

        // swap the screen buffers
        glfwSwapBuffers(window->get());
    }
    //glDeleteVertexArrays(1, &vaoId);
    //glDeleteBuffers(1, &vboId);

    delete window;
    return 0;
}

// Callbacks TODO
