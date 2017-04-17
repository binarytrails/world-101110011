#include <iostream>
#include <fstream> // used for file reading

#include <GL/glew.h>
#include <GLFW/glfw3.h>

//use GLM for transformation math
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

//include shader class from learnopengl.com
#include "Shader.h"

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
GLuint VBO, VAO;

//maximum number of particles
#define maxParticles 1000




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
	GLFWwindow* window = glfwCreateWindow(WIDTH, HEIGHT, "Cloud", nullptr, nullptr);
	glfwMakeContextCurrent(window);

	// Set the required callback functions
	glfwSetKeyCallback(window, key_callback);

	// Set this to true so GLEW knows to use a modern approach to retrieving function pointers and extensions
	glewExperimental = GL_TRUE;
	// Initialize GLEW to setup the OpenGL Function pointers
	glewInit();

	// Define the viewport dimensions
	glViewport(0, 0, WIDTH, HEIGHT);

	
				////////////// End Setup \\\\\\\\\\\\\\ 
	

	// Build and compile our shader program
	Shader ourShader("src/shaders/shaderRain.vs", "src/shaders/shaderRain.frag");

	//enable depth testing
	glEnable(GL_DEPTH_TEST);



	// create new cloud and set all it's starting drops
	Wind* breeze = new Wind(0.0005f, 0.0f, 0.0f);
	Cloud* test = new Cloud(0, 1, 0, 0.5, 0.5, maxParticles, breeze, 1);




	//generate the vertex buffer object(s), vertex array object
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);

	// Bind VAO
	glBindVertexArray(VAO);

	// Bind VBO
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(test->getAllDrops()), test->getAllDrops(), GL_DYNAMIC_DRAW);

	// Position attribute of the lines (location, # of float that make up a vertex (x,y,z), form (float, int, etc), normalised, distance between the starting point of each group of vertices, beginning offset) 
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (GLvoid*)0);
	glEnableVertexAttribArray(0);

	// Unbind VAO
	glBindVertexArray(0);





	while (!glfwWindowShouldClose(window))
	{

		// Check if any events have been activiated (key pressed, mouse moved etc.) and call corresponding response functions
		glfwPollEvents();

		// Clear the colorbuffer
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


		// use our Shader Program
		ourShader.Use();

		// Get their uniform location
		GLint modelLoc = glGetUniformLocation(ourShader.Program, "model");
		GLint viewLoc = glGetUniformLocation(ourShader.Program, "view");
		GLint projLoc = glGetUniformLocation(ourShader.Program, "projection");

		// Create projection matrix
		glm::mat4 projection;
		projection = glm::perspective(glm::radians(45.0f), (GLfloat)WIDTH / (GLfloat)HEIGHT, 0.1f, 100.0f);

		// Create view matrix
		glm::mat4 view;
		view = glm::lookAt(cameraPos, cameraTarget, cameraUp);


		// Set the view and projection matrixes each frame
		glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
		glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));


		//update all the drops positions
		test->updateAll();
		glBindBuffer(GL_ARRAY_BUFFER, VBO);
		glBufferData(GL_ARRAY_BUFFER, maxParticles * 6, test->getAllDrops(), GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ARRAY_BUFFER, 0);

		//bind the VAO
		glBindVertexArray(VAO);


		//camera rotations
		do_movement();

		//set the model matrix each frame
		glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(model));

		//draw all the drops
		glDrawArrays(GL_LINES, 0, maxParticles * 6);

		//unbind VAO
		glBindVertexArray(0);

		// Swap the screen buffers
		glfwSwapBuffers(window);

	}

	// de-allocate all resources
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);

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


// Camera controls
// WASD rotate the shape in XYZ, UP/DOWN zooms in and out
void do_movement()
{
	GLfloat distanceChange = 0.01f;

	if (keys[GLFW_KEY_W])
	{
		GLfloat angle = glm::radians(0.1f);
		model = glm::rotate(model, -angle, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	if (keys[GLFW_KEY_S])
	{
		GLfloat angle = glm::radians(0.1f);
		model = glm::rotate(model, angle, glm::vec3(1.0f, 0.0f, 0.0f));
	}

	if (keys[GLFW_KEY_A])
	{
		GLfloat angle = glm::radians(0.1f);
		model = glm::rotate(model, angle, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	if (keys[GLFW_KEY_D])
	{
		GLfloat angle = glm::radians(0.1f);
		model = glm::rotate(model, -angle, glm::vec3(0.0f, 1.0f, 0.0f));
	}



	if (keys[GLFW_KEY_UP])
	{
		radius -= distanceChange;
		cameraPos = glm::vec3(0.0f, 0.0f, radius);
	}

	if (keys[GLFW_KEY_DOWN])
	{
		radius += distanceChange;
		cameraPos = glm::vec3(0.0f, 0.0f, radius);
	}

}
