/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#include "World.hpp"

World::World(const uint16_t width, const uint16_t height, const char* name):
    TERRAIN_WIDTH(width), TERRAIN_HEIGHT(height),
    window(new Window(1280, 720, name)),
    camera(new Camera()),
    pitch(0.0f), yaw(45.0f), displayText(false)
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
    delete this->skybox;
    delete this->gui;
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

void World::centerCamera()
{
    glm::vec3 eye;

    eye.x = floor(this->TERRAIN_WIDTH  / 2.0f);
    eye.z = floor(this->TERRAIN_HEIGHT / 2.0f);

    // above the ground
    eye.y = this->terrain->getElevation(eye.x, eye.z);

    // FIXME
    //this->terrain->rotate(glm::vec3(12.5, 0, 0) / 20.0f);
    //printf("eye.y: %f\n", eye.y);

    this->camera->setEye(eye);
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
    this->view = this->camera->getView();

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
    this->forest = new Forest(this->TERRAIN_WIDTH, this->TERRAIN_HEIGHT, this->terrain);

    /* Creates new cloud and set all it's starting drops
     * TODO define arguments clearly
     */
	this->wind  = new Wind(0.001f, 0.0f, 0.0f);
    // (x-position, y-position, z-position, width, length, max_particles, wind, mode for rain or snow[may never happen])
	this->cloud = new Cloud(0, 10, 0, 10, 10, 1000, this->wind, 1);

    // FIXME adjusted for current terrain elevation
    this->rotate(glm::vec3(12.5, 0, 0));

    this->centerCamera();

    this->skybox = new Skybox();

    this->gui = new GUI();

    this->setWindowContext();
    this->setWindowCallbacks();
}

void World::draw()
{
    while (!glfwWindowShouldClose(this->window->get()))
    {   
        glPolygonMode( GL_FRONT_AND_BACK, GL_LINE );

        glfwPollEvents();
        applyKeyboardBindings(this->window->get());

        glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        this->shader->use();

        this->skybox->renderSkybox(this->window, this->camera,
                                   this->view, this->projection);

        this->terrain->render(this->window, this->camera,
                              this->view, this->projection);

        this->forest->render(this->window, this->camera,
                             this->view, this->projection);

        this->cloud->render(this->window, this->camera,
                            this->view, this->projection);

	   this->outputUI();

       this->controlCamPos();

        // continuous rotation
        //this->rotate(glm::vec3(0, 0, 0));

        this->updateMVP();

        // swap the screen buffers
        glfwSwapBuffers(this->window->get());

        //this->camera->printView();
    }
}

void World::outputUI()
{
        //! Code relevant to UI text.
        glm::vec3 color = glm::vec3(0.0f, 0.0f, 0.0f);
        GLfloat textXPos = this->window->width() - 200;
        GLfloat textYPos = this->window->height() - 50;

    if(displayText)
    {
        
        this->gui->renderText("Controls:", textXPos, textYPos, 0.5f, 
            color, this->window);

        textYPos -= 30;

        this->gui->renderText("ESC :    exit", textXPos, textYPos, 0.3f, 
            color, this->window);

        textYPos -= 30;

        this->gui->renderText("i :  zoom in", textXPos, textYPos, 0.3f, 
            color, this->window);

        textYPos -= 30;

        this->gui->renderText("o :  zoom out", textXPos, textYPos, 0.3f, 
            color, this->window);

        textYPos -= 30;

        this->gui->renderText("scroll:  zoom in/out", textXPos, textYPos, 0.3f, 
            color, this->window);

        textYPos -= 60;

         this->gui->renderText("w:  move upward", textXPos, textYPos, 0.3f, 
            color, this->window);

         textYPos -= 30;

         this->gui->renderText("s:  move downward", textXPos, textYPos, 0.3f, 
            color, this->window);


         textYPos -= 30;

         this->gui->renderText("a:  move left", textXPos, textYPos, 0.3f, 
            color, this->window);


         textYPos -= 30;

         this->gui->renderText("d:  move right", textXPos, textYPos, 0.3f, 
            color, this->window);

        
        textYPos -= 60;

         this->gui->renderText("q:  quit help screen", textXPos, textYPos, 0.3f, 
            color, this->window);
    }

    else
    {
        this->gui->renderText("h:  help screen", textXPos, textYPos, 0.3f, 
            color, this->window);
    }
}

void World::controlCamPos()
{

    GLfloat xpos = this->camera->getEye().x;
    GLfloat zpos = this->camera->getEye().z;

    GLfloat terraintHeight

    if(this->camera->getEye().y <= 0)
        this->camera->setEye(glm::vec3(xpos, 0.0f, zpos));
}

void World::rotate(const glm::vec3 axes)
{
    glm::vec3 terrainSpin = axes / 20.0f;
    this->terrain->rotate(terrainSpin);
    //this->forest->rotate(terrainSpin);
}
