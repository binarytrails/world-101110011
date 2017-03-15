/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#include "World.hpp"

World::World(): 
    window(new Window(1280, 720, "Procedural world")),
//    shader(new Shader("src/shaders/world.vs",
//                      "src/shaders/world.fs")),
    camera(new Camera())
{
    glewExperimental = GL_TRUE;
    glewInit();
    glEnable(GL_DEPTH_TEST);
    glViewport(0, 0, this->window->width(), this->window->height());
}

World::~World()
{
    glDeleteVertexArrays(1, &this->vaoId);
    glDeleteBuffers(1, &this->vboId);

    delete this->camera;
    delete this->shader;
    delete this->window;
}

void World::draw()
{
    while (!glfwWindowShouldClose(this->window->get()))
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //this->view = this->camera.view();

        //this->shader->use();

        // TODO MVP matrices

        // swap the screen buffers
        glfwSwapBuffers(this->window->get());
    }
}
