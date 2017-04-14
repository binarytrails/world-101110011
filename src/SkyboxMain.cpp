#include "Skybox.h"

// Constants
const uint WIDTH = 800, HEIGHT = 600;

// Function prototypes 
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void mousePositionCallback(GLFWwindow* window, double xpos, double ypos);
// void Do_Movement();


Camera* camera = new Camera();

bool keys[1024];
GLfloat lastX = 400, lastY = 300;
bool firstMouse = true;

GLfloat yaw = -90.0f;
GLfloat pitch = 0.0f;

int main()
{	

	// 1. Create our GLFW window (provide Window hints et al.)
	Window* window = new Window(WIDTH, HEIGHT, "Skybox Demo");

	// 2. Set up our callback functions.
	 glfwSetKeyCallback(window->get(), key_callback);
    glfwSetCursorPosCallback(window->get(), mousePositionCallback);
    // glfwSetScrollCallback(window->get(), scroll_callback);


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
    faces.push_back("cubemap/back.jpg");
    faces.push_back("cubemap/front.jpg");

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

void mousePositionCallback(GLFWwindow* w, double xpos, double ypos)
{
    // Delta of position along x-axis.
    GLfloat xoffset = xpos - lastX;

    // Delta of positon along y-axis.
    GLfloat yoffset = ypos - lastY;

    // Reset our x and y positions for the frame. 
    lastX = xpos;
    lastY = ypos;

    // In order to lessen the jerk of mouse movement, we add this sensitivity to offsets. 
    GLfloat sensitivity = 0.05f;
    xoffset *= sensitivity;
    yoffset *= sensitivity;

    yaw += xoffset;
    pitch += yoffset;

    // Make sure that pitch is in the domain [-89.0, 89.0] (not exactly 90 due to cosine).
    if(pitch > 89.0f)
        pitch = 89.0f;
    if(pitch < -89.0f)
        pitch = -89.0f;

    glm::vec3 viewDirection;
    viewDirection.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    viewDirection.y = sin(glm::radians(pitch));
    viewDirection.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    glm::vec3 normalized = glm::normalize(viewDirection);
    camera->setAt(normalized);

}


#pragma endregion