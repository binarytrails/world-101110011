#include <iostream>
#include <fstream> // used for file reading

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//use GLM for transformation math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//include shader class from learnopengl.com
#include "Shader.hpp"

#include "Cloud.h"
#include "Wind.h"

#include <time.h>

using namespace std;

//used in the do_movement() method
bool keys[1024];

//initialize distance from the object
GLfloat radius = 5.0f;

//vertices for the camera
glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, radius);
glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
glm::vec3 cameraFromTarget = glm::normalize(cameraPos - cameraTarget);
glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraFromTarget));
glm::vec3 cameraUp = glm::cross(cameraFromTarget, cameraRight);

//initialize the model matrix
glm::mat4 model;

// Window dimensions
const GLuint WIDTH = 800, HEIGHT = 800;

//method to take in key strokes
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);

//camera rotations (from learnopengl.com)
void do_movement();

//variables for VBO and VAO
//GLuint VBO, VAO;

//maximum number of particles
#define maxParticles 3000

Window *window = new Window(WIDTH, HEIGHT, "cloud");
Camera *camera = new Camera();



int main() {


				////////////// Setup \\\\\\\\\\\\\\


	// Init GLFW
	glfwInit();
	// Set all the required options for GLFW
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
	glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);

	// Create a GLFWwindow object that we can use for GLFW's functions
	//GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Cloud", nullptr, nullptr);
	glfwMakeContextCurrent(window->get());

	// Set the required callback functions
	glfwSetKeyCallback(window->get(), key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	
				////////////// End Setup \\\\\\\\\\\\\\ 
	

	//enable depth testing
	glEnable(GL_DEPTH_TEST);



	// create new cloud and set all it's starting drops
	Wind* breeze = new Wind(0.001f, 0.0f, 0.0f);
	Cloud* test = new Cloud(0, 10, 0, 50, 50, maxParticles, breeze, 2);




	while (!glfwWindowShouldClose(window->get()))
	{

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		glm::mat4 projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);
		test->render(window, camera, camera->getView(), projection);

		//camera movements
		do_movement();


		// Swap the screen buffers
		glfwSwapBuffers(window->get());
	}

	test->deallocate();

	// terminate GLFW
	glfwTerminate();
	return 0;


}


// Is called whenever a key is pressed/released via GLFW (from learnopengl.com)
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GL_TRUE);

	if (key >= 0 && key < 1024)
	{
		if (action == GLFW_PRESS)
			keys[key] = true;
		else if (action == GLFW_RELEASE)
			keys[key] = false;
	}

}



void do_movement()
{

	if (keys[GLFW_KEY_W])
	{
		camera->moveUp();
	}

	if (keys[GLFW_KEY_S])
	{
		camera->moveDown();
	}

	if (keys[GLFW_KEY_A])
	{
		camera->moveLeft();
	}

	if (keys[GLFW_KEY_D])
	{
		camera->moveRight();
	}



	if (keys[GLFW_KEY_UP])
	{
		camera->moveForward();
	}

	if (keys[GLFW_KEY_DOWN])
	{
		camera->moveBackward();
	}

}

