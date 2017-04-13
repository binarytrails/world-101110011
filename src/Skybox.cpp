/*
* @file
* @author Tarik Abou-Saddik
* 
* Test file for creating a skybox.
*
*/


/*
	SETTTING UP THE FILE
	====================

	1. GLFW WINDOW/RENDERING CONTEXT CREATED BY WINDOW.HPP

	2. CAMERA OBJECT IS CREATED BY CAMERA.HPP

	3. TODO: SHADERS. 

	4. TODO: TEXTURES
	
*/

// Basic includes
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
// Textures
#include <SOIL/SOIL.h>
#include <glm/gtc/type_ptr.hpp>

// User-defined includes
#include "Camera.hpp" 
#include "Window.hpp"
// #include "../World.hpp" // Already includes GLM
#include "Shader.hpp"

// Constants
const uint WIDTH = 800, HEIGHT = 600;

// Function prototypes 
void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode);
// void scrollCallback(GLFWwindow* window, double xoffset, double yoffset);
// void mouseCallback(GLFWwindow* window, double xpos, double ypos);
GLuint loadTexture(GLchar* path);
GLuint loadCubemap(std::vector<const GLchar*> faces);

Camera* camera = new Camera();
GLfloat lastFrame;
GLfloat deltaTime;


int main() 
{
	// 1. Create our GLFW window (provide Window hints et al.)
	Window* window = new Window(WIDTH, HEIGHT, "Skybox Demo");

	// 2. Set up our callback functions.
	glfwSetKeyCallback(window->get(), keyCallback);
	// glfwSetCursorPosCallback(window, mouseCallback);
	// glfwSetScrollCallback(window, scrollCallback);

	glfwSetInputMode(window->get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// 3. Initialize GLEW to set up the function pointers.
	glewExperimental = GL_TRUE;
	glewInit();

	// 4. Define viewport dimensions
	glViewport(0,0, WIDTH, HEIGHT);

	// 5. Set up some OpenGL options
	glEnable(GL_DEPTH_TEST);
	glDepthFunc(GL_LESS);

	// 6. Set up and compile our shaders.
	Shader shader("src/shaders/advanced.vs", "src/shaders/advanced.frag");
	Shader skyboxShader("src/shaders/skybox.vs", "src/shaders/skybox.frag");

	// 7. Setting up vertices for our cube.
	GLfloat cubeVertices[] = {
        // Positions          // Texture Coords
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 0.0f,

        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,

        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,

        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,
         0.5f, -0.5f, -0.5f,  1.0f, 1.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
         0.5f, -0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f, -0.5f, -0.5f,  0.0f, 1.0f,

        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f,
         0.5f,  0.5f, -0.5f,  1.0f, 1.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f,  1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f,  0.0f, 0.0f,
        -0.5f,  0.5f, -0.5f,  0.0f, 1.0f

      };

      // 8. Setting up vertices for our skybox. 
       GLfloat skyboxVertices[] = {
        // Positions          
        -1.0f,  1.0f, -1.0f,
        -1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
  
        -1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f, -1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
  
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
   
        -1.0f, -1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f, -1.0f,  1.0f,
        -1.0f, -1.0f,  1.0f,
  
        -1.0f,  1.0f, -1.0f,
         1.0f,  1.0f, -1.0f,
         1.0f,  1.0f,  1.0f,
         1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f,  1.0f,
        -1.0f,  1.0f, -1.0f,
  
        -1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f, -1.0f,
         1.0f, -1.0f, -1.0f,
        -1.0f, -1.0f,  1.0f,
         1.0f, -1.0f,  1.0f
    };

    // 9. Setting up the VAO and VBO for our cube.
    GLuint cubeVAO, cubeVBO;
    // Generate VAO and VBO for cube.
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
	// Bind buffers for cube VAO.
    glBindVertexArray(cubeVAO);
   		glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(cubeVertices), &cubeVertices, GL_STATIC_DRAW);
    	glEnableVertexAttribArray(0);
    	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)0);
    	glEnableVertexAttribArray(1);
    	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 5, (GLvoid*)(3 * sizeof(GLfloat)));
    glBindVertexArray(0);

    // 10. Setting up VAO and VBO for our skybox.
    GLuint skyboxVAO, skyboxVBO;
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    // Bind buffers for skybox VAO
    glBindVertexArray(skyboxVAO);
    	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
    	glEnableVertexAttribArray(0);
    	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*)0);
    glBindVertexArray(0);

    //11. Load textures
    GLuint cubeTexture  = loadTexture("assets/images/cubemap/textures/container.jpg");

    // 12. Cubemap (Skybox)
    std::vector<const GLchar*> faces;
    faces.push_back("/assets/images/cubemap/skybox-2/right.jpg");
    faces.push_back("/assets/images/cubemap/skybox-2/left.jpg");
    faces.push_back("/assets/images/cubemap/skybox-2/top.jpg");
    faces.push_back("/assets/images/cubemap/skybox-2/bottom.jpg");
    faces.push_back("/assets/images/cubemap/skybox-2/front.jpg");
    faces.push_back("/assets/images/cubemap/skybox-2/back.jpg");
    GLuint cubemapTexture = loadCubemap(faces);

    // 13. Game loop
   	while(!glfwWindowShouldClose(window->get()))
   	{
   		// Set frame time
   		GLfloat currentFrame = glfwGetTime();
   		deltaTime = currentFrame - lastFrame;
   		lastFrame = currentFrame;

   		// Check and call events
   		glfwPollEvents();

   		// Clear buffers
   		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
   		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

   		// Draw skybox
   		glDepthMask(GL_FALSE);
   		skyboxShader.use();
   		// Removing translation component of vector.
   		glm::mat4 view = glm::mat4(glm::mat3(camera->view()));
   		glm::mat4 projection = glm::perspective(45.0f, (float)WIDTH/(float)HEIGHT, 0.1f, 100.0f);
   		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));
   		glUniformMatrix4fv(glGetUniformLocation(skyboxShader.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
   		// skybox cube
   		glBindVertexArray(skyboxVAO);
   			glActiveTexture(GL_TEXTURE0);
   			glUniform1i(glGetUniformLocation(shader.programId, "skybox"), 0);
   			glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTexture);
   			glDrawArrays(GL_TRIANGLES, 0, 36);
   		glBindVertexArray(0);
   		glDepthMask(GL_TRUE);

   		// // Then draw scene as normal.
   		// shader.use();
   		// glm::mat4 model; 
   		// view = camera->view();

   		// glUniformMatrix4fv(glGetUniformLocation(shader.programId, "model"), 1, GL_FALSE, glm::value_ptr(model));
     //    glUniformMatrix4fv(glGetUniformLocation(shader.programId, "view"), 1, GL_FALSE, glm::value_ptr(view));
     //    glUniformMatrix4fv(glGetUniformLocation(shader.programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
     //    // Cubes
     //    glBindVertexArray(cubeVAO);
     //    	glActiveTexture(GL_TEXTURE0);
     //    	glUniform1i(glGetUniformLocation(shader.programId, "texture_diffuse1"), 0);
     //    	glBindTexture(GL_TEXTURE_2D, cubeTexture);  		
     //    	glDrawArrays(GL_TRIANGLES, 0, 36);
     //    glBindVertexArray(0);			

        // Swap the buffers
        glfwSwapBuffers(window->get()); 	

   	}

   	delete window;
   	window = nullptr;

   	delete camera;
   	camera = nullptr;


 return 0;

}

// Loads a cubemap texture from 6 individual texture faces
// Order should be:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
// This is due to the fact that each face is stored is part of an enum in OpenGL.
GLuint loadCubemap(std::vector<const GLchar*> faces)
{
	GLuint textureID;
	glGenTextures(1, &textureID);

	int width, height;
	unsigned char* image;

	glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);
	for(GLuint i = 0; i < faces.size(); i++)
	{
		image = SOIL_load_image(faces[i], &width, &height, 0, SOIL_LOAD_RGB);
		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
		SOIL_free_image_data(image);
	}

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

    return textureID;
}

// This function loads a texture from file. Note: texture loading functions like these are usually 
// managed by a 'Resource Manager' that manages all resources (like textures, models, audio). 
// For learning purposes we'll just define it as a utility function.
GLuint loadTexture(GLchar* path)
{
    //Generate texture ID and load texture data 
    GLuint textureID;
    glGenTextures(1, &textureID);
    int width,height;
    unsigned char* image = SOIL_load_image(path, &width, &height, 0, SOIL_LOAD_RGB);
    // Assign texture to ID
    glBindTexture(GL_TEXTURE_2D, textureID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    glGenerateMipmap(GL_TEXTURE_2D);	

    // Parameters
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR );
    glTexParameteri( GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glBindTexture(GL_TEXTURE_2D, 0);
    SOIL_free_image_data(image);
    return textureID;
}

void keyCallback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if(key == GLFW_KEY_UP && action == GLFW_PRESS)
		camera->moveForward();

	if(key == GLFW_KEY_DOWN && action == GLFW_PRESS)
		camera->moveBackward();

	if(key == GLFW_KEY_UP && action == GLFW_PRESS)
		camera->moveUp();

	if(key == GLFW_KEY_RIGHT && action == GLFW_PRESS)
		camera->moveRight();

	if(key == GLFW_KEY_LEFT && action == GLFW_PRESS)
		camera->moveLeft();
}








