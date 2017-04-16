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
    glewExperimental = GL_TRUE;
    glewInit();

    this->initOpenGL();
    this->initCamera();

    // It has to be after glewInit
    this->shader = new Shader("src/shaders/world.vs",
                              "src/shaders/world.fs");

    this->build();
    this->setRenderMode(GL_TRIANGLES);

    this->updateMVP();
    this->initSound();
}

World::~World()
{
    glDeleteVertexArrays(1, &this->vaoId);
    glDeleteBuffers(1, &this->vboId);

    delete this->camera;
    delete this->shader;
    delete this->window;
}

void World::initOpenGL()
{
    glEnable(GL_DEPTH_TEST);

    // Transparency
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    glViewport(0, 0, this->window->width(), this->window->height());
}

void World::initCamera()
{
    // Mouse controls
    lastX = this->window->width() / 2;
    lastY = this->window->height() / 2;

    // Position at the center
    this->camera->setEye(glm::vec3(
        this->TERRAIN_WIDTH / 2.0f,     // x
        2,                              // y
        this->TERRAIN_HEIGHT / 2.0f));  // z
}

void World::initSound()
{
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

void World::setRenderMode(const GLenum mode)
{
    this->terrain->setRenderMode(mode);
    this->forest->setRenderMode(mode);
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

/*! Binding of callbackContext with a World instance
 *  to the GLFW window held inside it in order to allow
 *  World methods references from the GLFW callbacks
 */
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
    this->forest = new Forest(this->TERRAIN_WIDTH, this->TERRAIN_HEIGHT);

    // FIXME adjusted for current terrain elevation
    this->rotate(glm::vec3(12.5, 0, 0));

    // Faces for our cubemap.
    std::vector<const GLchar*> faces;
    faces.push_back("assets/cubemap/right.jpg");
    faces.push_back("assets/cubemap/left.jpg");
    faces.push_back("assets/cubemap/top.jpg");
    faces.push_back("assets/cubemap/bottom.jpg");
    faces.push_back("assets/cubemap/back.jpg");
    faces.push_back("assets/cubemap/front.jpg");

    // faces.push_back("assets/cubemap/hills_rt.jpg");
    // faces.push_back("assets/cubemap/hills_lf.jpg");
    // faces.push_back("assets/cubemap/hills_up.jpg");
    // faces.push_back("assets/cubemap/hills_dn.jpg");
    // faces.push_back("assets/cubemap/hills_bk.jpg");
    // faces.push_back("assets/cubemap/hills_ft.jpg");

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

        this->skybox->renderSkybox(this->window, this->camera,
                                    this->view, this->projection);

        this->terrain->render(this->window, this->camera,
                              this->view, this->projection);

        this->forest->render(this->window, this->camera,
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
    this->forest->rotate(terrainSpin);
}
