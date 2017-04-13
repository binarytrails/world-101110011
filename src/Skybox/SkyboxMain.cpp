#include "Skybox.h"

// Constants
const uint WIDTH = 800, HEIGHT = 600;

// Function prototypes 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void Do_Movement();


Camera* camera = new Camera();

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

int main()
{	

	// 1. Create our GLFW window (provide Window hints et al.)
	Window* window = new Window(WIDTH, HEIGHT, "Skybox Demo");

	// 2. Set up our callback functions.
	 glfwSetKeyCallback(window->get(), key_callback);
    // glfwSetCursorPosCallback(window->get(), mouse_callback);
    // glfwSetScrollCallback(window->get(), scroll_callback);
	
    // 3. Set input mode of GLFW window.
	glfwSetInputMode(window->get(), GLFW_CURSOR, GLFW_CURSOR_DISABLED);

	// 4. Define our cube map using six textures.
	// Order should be:
	// +X (right)
	// -X (left)
	// +Y (top)
	// -Y (bottom)
	// +Z (front) 
	// -Z (back)
	std::vector<const GLchar*> faces;
    faces.push_back("cubemap/right.jpg");
    faces.push_back("cubemap/left.jpg");
    faces.push_back("cubemap/top.jpg");
    faces.push_back("cubemap/bottom.jpg");
    faces.push_back("cubemap/front.jpg");
    faces.push_back("cubemap/back.jpg");

	//5. Create Skybox object
	Skybox* skybox = new Skybox(faces);

	//6. Render out Skybox
	skybox->renderSkybox(window, camera);

    delete window;
   	window = nullptr;


	return 0;
}

// Is called whenever a key is pressed/released via GLFW
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
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

#pragma endregion