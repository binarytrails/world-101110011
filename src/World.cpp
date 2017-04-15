/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#include "World.hpp"

World::World(const uint16_t width, const uint16_t height, const char* name):
    TERRAIN_WIDTH(width), TERRAIN_HEIGHT(height),
    window(new Window(1280, 720, name)),
    camera(new Camera()),
    pitch(0.0f), yaw(45.0f)
{
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glewExperimental = GL_TRUE;
    glewInit();

    glEnable(GL_DEPTH_TEST);

    // TODO add to frame buffer callback
    glViewport(0, 0, this->window->width(), this->window->height());

    // Camera mouse controls
    lastX = this->window->width() / 2;
    lastY = this->window->height() / 2;

    this->shader = new Shader("src/shaders/world.vs",
                              "src/shaders/world.fs");

    this->build();
    this->setRenderMode(GL_TRIANGLES);

    // FIXME hack display
    this->rotate(glm::vec3(5, 0, 0));

    this->updateMVP();

    // sound
    if (!this->bgMusicBuffer.loadFromFile("./assets/sound/amb-forest.ogg"))
    {
        fprintf(stderr, "Failed to load background music");
    }
    else
    {
        this->bgMusic.setBuffer(this->bgMusicBuffer);
        this->bgMusic.setLoop(true);
        this->bgMusic.play();
    }
}

World::~World()
{
    glDeleteVertexArrays(1, &this->vaoId);
    glDeleteBuffers(1, &this->vboId);

    delete this->camera;
    delete this->shader;
    delete this->window;
}

void World::setRenderMode(const GLenum mode)
{
    this->terrain->setRenderMode(mode);
}

Window* World::getWindow()
{
    return this->window;
}

Camera* World::getCamera()
{
    return this->camera;
}

Terrain* World::getTerrain()
{
    return this->terrain;
}

void World::setWindowContext()
{
    callbackContext.world = this;
    glfwSetWindowUserPointer(this->window->get(), &callbackContext);
}

void World::setWindowCallbacks()
{
    // set callbacks
    glfwSetKeyCallback(this->window->get(), keyCallback);

    glfwSetMouseButtonCallback(this->window->get(), mouseKeyCallback);

    glfwSetFramebufferSizeCallback(this->window->get(),
                                   framebufferSizeCallback);

    glfwSetScrollCallback(this->window->get(), mouseScrollCallback);

    // TODO
    glfwSetCursorPosCallback(this->window->get(), mousePositionCallback);
}

void World::updateMVP()
{
    // update states
    this->view = this->camera->view();

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

void World::build()
{
    this->terrain = new Terrain(this->TERRAIN_WIDTH, this->TERRAIN_HEIGHT);

    // Faces for our cubemap.
    std::vector<const GLchar*> faces;
    faces.push_back("cubemap/right.jpg");
    faces.push_back("cubemap/left.jpg");
    faces.push_back("cubemap/top.jpg");
    faces.push_back("cubemap/bottom.jpg");
    faces.push_back("cubemap/back.jpg");
    faces.push_back("cubemap/front.jpg");

    this->skybox = new Skybox(faces);

    this->setWindowContext();
    this->setWindowCallbacks();
}

void World::draw()
{
    while (!glfwWindowShouldClose(this->window->get()))
    {
        glfwPollEvents();
        doMovement(this->window->get());

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->shader->use();

        this->terrain->render(this->window, this->camera,
                              this->view, this->projection);

        this->skybox->renderSkybox(this->window, this->camera,
                                    this->view, this->projection);

        // continuous rotation
        //this->rotate(glm::vec3(0, 0, 0));

        this->updateMVP();

        // swap the screen buffers
        glfwSwapBuffers(this->window->get());

        //this->camera->printView();
    }
}

void World::rotate(const glm::vec3 axes)
{
    glm::vec3 terrainSpin = axes / 20.0f;
    this->terrain->rotate(terrainSpin);
}
