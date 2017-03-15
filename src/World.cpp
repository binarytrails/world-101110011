/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#include "World.hpp"

World::World():
    window(new Window(1280, 720, "Procedural world")),
    camera(new Camera())    //gl_Position = vec4(position, 1.0f);
,
    renderMode(GL_TRIANGLES)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);

    // TODO add to frame buffer callback
    glViewport(0, 0, this->window->width(), this->window->height());

    this->shader = new Shader("src/shaders/world.vs",
                              "src/shaders/world.fs");
}

World::~World()
{
    glDeleteVertexArrays(1, &this->vaoId);
    glDeleteBuffers(1, &this->vboId);

    delete this->camera;
    delete this->shader;
    delete this->window;
}

void World::updateMVP()
{
    // update states
    this->view = glm::translate(this->camera->view(),
                                glm::vec3(0.0f, 0.0f, -3.0f));

    this->projection = glm::perspective(
        45.0f,
        (GLfloat) this->window->width() / (GLfloat) this->window->height(),
        0.1f,
        100.0f);

    // locate in shaders
    GLint modelLoc = glGetUniformLocation(this->shader->programId, "model");
    GLint viewLoc = glGetUniformLocation(this->shader->programId, "view");
    GLint projLoc = glGetUniformLocation(this->shader->programId, "projection");

    // send to shaders
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(this->view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(this->projection));

}

void World::draw()
{
    this->initTerrain();
    this->initTerrainBuffers();
    this->uploadTerrain();

    while (!glfwWindowShouldClose(this->window->get()))
    {
        glfwPollEvents();

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->shader->use();

        this->updateMVP();

        this->drawTerrain();

        // swap the screen buffers
        glfwSwapBuffers(this->window->get());
    }
}

void World::initTerrain()
{
    // square
    this->terrainVertices.push_back(glm::vec3(-1.0f, -1.0f, 0.0f)); // b-l
    this->terrainVertices.push_back(glm::vec3(1.0f, -1.0f, 0.0f));  // b-r
    this->terrainVertices.push_back(glm::vec3(-1.0f, 1.0f, -1.0f)); // t-l
    this->terrainVertices.push_back(glm::vec3(1.0f, 1.0f, -1.0f));  // t-r

    this->terrainVerticesI = {
        0, 1, 2,
        1, 2, 3
    };
}

void World::initTerrainBuffers()
{
    glGenBuffers(1, &this->vboId);
    glGenVertexArrays(1, &this->vaoId);
    glGenBuffers(1, &this->eboId);

    glBindBuffer(GL_ARRAY_BUFFER, this->vboId);
    glBufferData(GL_ARRAY_BUFFER,
                 sizeof(glm::vec3) * this->terrainVertices.size(),
                 &this->terrainVertices[0],
                 GL_STATIC_DRAW);

    // has to be before ebo bind
    glBindVertexArray(this->vaoId);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboId);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                 sizeof(this->terrainVerticesI) * this->terrainVerticesI.size(),
                 &this->terrainVerticesI[0],
                 GL_STATIC_DRAW);

    // enable vao -> vbo pointing
    glEnableVertexAttribArray(0);
    // setup formats of my vao attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), NULL);

    // unbind vbo
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    // unbind vao
    glBindVertexArray(0);
}

void World::uploadTerrain()
{
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->eboId);

        glBufferData(GL_ELEMENT_ARRAY_BUFFER,
                     sizeof(GLushort) * this->terrainVerticesI.size(),
                    &this->terrainVerticesI[0],
                    GL_STATIC_DRAW);

    // don't disconnect to draw
}

void World::drawTerrain()
{
    glBindVertexArray(this->vaoId);
        glDrawElements(this->renderMode,
                       this->terrainVerticesI.size(),
                       GL_UNSIGNED_SHORT, 0);
    glBindVertexArray(0);
}
