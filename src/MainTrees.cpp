/*
 *	Assignment 2 by Sami Boubaker (26517404)
 *
 *	Controls:
 *		AWSD or arrows 	-> rotate object (in sweep screen)
 *		P L F		-> change mode (points, lines, fill)
 * 
*/

#include <iostream>
#include <fstream>
#include <string>
#include <vector>

// GLEW
#define GLEW_STATIC
#include <GL/glew.h>

// GLFW
#include <GLFW/glfw3.h>

// GLM Mathematics
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <complex.h>

// Other includes
#include "Shader.h"

#include "GLObject.h"
#include "PGTree1.h"
#include "PGTree2.h"
#include "Point.h"

// Window dimensions
GLuint WIDTH = 800, HEIGHT = 800;

GLfloat cameraX = 0.0f;
GLfloat cameraY = 0.0f;
GLuint VBO, instanceVBO, VAO, EBO;
int glMode = 2; // render points, line or fill
int isTrans;
int rotCount;
int stage = 0; //1 user can input points, 2 spline is rendered, 3 sweep is rendered
float myScale = 1;

GLObject* obj;

GLFWwindow* window;

// Function prototypes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void window_size_callback(GLFWwindow* window, int w, int h);

void init() {
    // Init GLFW
    glfwInit();
    // Set all the required options for GLFW
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_RESIZABLE, GL_TRUE);

    // Create a GLFWwindow object that we can use for GLFW's functions
    window = glfwCreateWindow(WIDTH, HEIGHT, "Assignment 2", nullptr, nullptr);
    glfwMakeContextCurrent(window);

    // Set the required callback functions
    glfwSetKeyCallback(window, key_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetWindowSizeCallback(window, window_size_callback);

    // Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
    glewExperimental = GL_TRUE;
    // Initialize GLEW to setup the OpenGL Function pointers
    glewInit();

    // Setup OpenGL options
    glEnable(GL_DEPTH_TEST);
    //glReshapeFunc(reshape);
};

/*void draw(GLObject* obj){
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind the Vertex Array Object first, then bind and set vertex buffer(s) and attribute pointer(s).
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, obj->vSize*sizeof(obj->vertices[0]), obj->vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, obj->iSize*sizeof(obj->indices[0]), obj->indices, GL_STATIC_DRAW);
    
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    
    // Also set instance data
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);	
    glVertexAttribDivisor(2, 1); // Tell OpenGL this is an instanced vertex attribute.
    
    glBindVertexArray(0);
};*/

// The MAIN function, from here we start the application and run the game loop
int main()
{
    init();

    // Build and compile our shader program
    Shader ourShader("src/shaders/object.vs", "src/shaders/object.fs");
    
    // Generate a list of 100 quad locations/translation-vectors
    glm::vec3 translations[100];
    glm::vec3 translations2[100];
    int index = 0;
    GLfloat offset2 = 0.1f;
    for(GLint y = -10; y < 10; y += 2)
    {
        for(GLint x = -10; x < 10; x += 2)
        {
            glm::vec3 translation;
            translation.x = 0;
            translation.y = 0;
            translation.z = 0;
            translations[index] = translation;
            translations2[99-index++] = translation;
        }
    }

    /*// Store instance data in an array buffer
    glGenBuffers(1, &instanceVBO);
    glBindBuffer(GL_ARRAY_BUFFER, instanceVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec3) * 100, &translations[0], GL_STATIC_DRAW);
    glBindBuffer(GL_ARRAY_BUFFER, 0);*/

    Point offset(0,0,0,Vector(0,1,0));
    PGTree1 tree1 = PGTree1(offset);
    GLObject* obj1 = new GLObject(tree1.objects);
    PGTree2 tree2 = PGTree2(offset);
    GLObject* obj2 = new GLObject(tree2.objects);
    //draw(obj);

    // Game loop
    while (!glfwWindowShouldClose(window))
    {
        // Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
        glfwPollEvents();

        // Render
        // Clear the colorbuffer
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Activate shader
        ourShader.Use();

        // Create transformations
        glm::mat4 transform;
        transform = glm::rotate(transform, cameraX, glm::vec3(1.0f, 0.0f, 0.0f));
        transform = glm::rotate(transform, cameraY, glm::vec3(0.0f, 1.0f, 0.0f));

        transform = glm::scale(transform, glm::vec3(myScale, myScale, myScale));

        // Get matrix's uniform location and set matrix
        GLint transformLoc = glGetUniformLocation(ourShader.Program, "transform");
        glUniformMatrix4fv(transformLoc, 1, GL_FALSE, glm::value_ptr(transform));

        // Draw container
        //glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, (glMode == 2) ? GL_LINE : GL_FILL);
        //glDrawElementsInstanced(GL_TRIANGLES, obj->iSize, GL_UNSIGNED_INT, 0, 3);
        //obj1->draw(translations, 100);
        obj2->draw(translations2, 1);

        glBindVertexArray(0);

        // Swap the screen buffers
        glfwSwapBuffers(window);
    }
    // Properly de-allocate all resources once they've outlived their purpose
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    // Terminate GLFW, clearing any resources allocated by GLFW.
    glfwTerminate();
    return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode) {
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);


    // Camera controls
    if((key == GLFW_KEY_W || key == GLFW_KEY_UP) && action != GLFW_RELEASE) {
        cameraX += 0.1f;
    }
    if((key == GLFW_KEY_S || key == GLFW_KEY_DOWN) && action != GLFW_RELEASE) {
        cameraX -= 0.1f;
    }
    if((key == GLFW_KEY_A || key == GLFW_KEY_LEFT) && action != GLFW_RELEASE) {
        cameraY -= 0.1f;
    }
    if((key == GLFW_KEY_D || key == GLFW_KEY_RIGHT) && action != GLFW_RELEASE) {
        cameraY += 0.1f;
    }

    // Mode control
    if(key == GLFW_KEY_P && action == GLFW_PRESS) {
        glMode = 1;
    }
    if(key == GLFW_KEY_L && action == GLFW_PRESS) {
        glMode = 2;
    }
    if(key == GLFW_KEY_F && action == GLFW_PRESS) {
        glMode = 3;
    }
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
    
}

void window_size_callback(GLFWwindow* window, int w, int h) {
    glViewport(0, 0, h, h);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,h,0,h,-1,1);
    glMatrixMode(GL_MODELVIEW);
    HEIGHT = h;
}
