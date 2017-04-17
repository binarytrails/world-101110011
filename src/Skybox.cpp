#include "Skybox.h"
#include "Shader.hpp"

Skybox::Skybox()
{ 

  // Set up and compile our shaders.
  shader = new Shader("src/shaders/advanced.vs", "src/shaders/advanced.frag");
  skyboxShader = new Shader("src/shaders/skybox.vs", "src/shaders/skybox.frag");

  // Faces of our cubemap.
  this->cubeMapFaces.push_back("assets/cubemap/right.jpg");
  this->cubeMapFaces.push_back("assets/cubemap/left.jpg");
  this->cubeMapFaces.push_back("assets/cubemap/top.jpg");
  this->cubeMapFaces.push_back("assets/cubemap/bottom.jpg");
  this->cubeMapFaces.push_back("assets/cubemap/back.jpg");
  this->cubeMapFaces.push_back("assets/cubemap/front.jpg");

  // Load our cubemap faces. 
  this->loadCubeMap(this->cubeMapFaces);

  // cubeMapFaces.push_back("assets/cubemap/hills_rt.jpg");
  // cubeMapFaces.push_back("assets/cubemap/hills_lf.jpg");
  // cubeMapFaces.push_back("assets/cubemap/hills_up.jpg");
  // cubeMapFaces.push_back("assets/cubemap/hills_dn.jpg");
  // cubeMapFaces.push_back("assets/cubemap/hills_bk.jpg");
  // cubeMapFaces.push_back("assets/cubemap/hills_ft.jpg");

    this->initBuffers();
}

Skybox::~Skybox()
{
}

void Skybox::initBuffers()
{ 

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

  // Set up our VAO and VBO
  glGenVertexArrays(1, &this->skyboxVAO);
      glGenBuffers(1, &this->skyboxVBO);
      // Bind buffers for skybox VAO
      glBindVertexArray(skyboxVAO);
      glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*)0);
    glBindVertexArray(0);

}

void Skybox::loadCubeMap(std::vector<const GLchar*>& cubeMapFaces)
{
  glGenTextures(1, &cubeMapID);

  int width, height;
  unsigned char* image;

  glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);
  for(GLuint i = 0; i < cubeMapFaces.size(); i++)
  {
    image = SOIL_load_image(cubeMapFaces[i], &width, &height, 0, SOIL_LOAD_RGB);
    if(!image)
      printf("ERROR::UNABLE TO LOAD TEXTURE\n");
    glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, image);
    SOIL_free_image_data(image);
  }

  glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
    glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

}

void Skybox::renderSkybox(Window* window, Camera* camera,
              const glm::mat4& view, const glm::mat4& projection)
{   
      
      glDepthMask(GL_FALSE);
      
      this->skyboxShader->use();
      // Removing translation component of vector.
      glm::mat4 view3 = glm::mat4(glm::mat3(view));
    
      // Get location of uniform variables.
      glUniformMatrix4fv(glGetUniformLocation(this->skyboxShader->programId, "view"), 1, GL_FALSE, glm::value_ptr(view3));
      glUniformMatrix4fv(glGetUniformLocation(this->skyboxShader->programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
      
      // Render our skybox cube.
      glBindVertexArray(this->skyboxVAO);
        glActiveTexture(GL_TEXTURE0);
        glUniform1i(glGetUniformLocation(this->shader->programId, "skybox"), 0);
        glBindTexture(GL_TEXTURE_CUBE_MAP, this->cubeMapID);
        glDrawArrays(GL_TRIANGLES, 0, 36);
      glBindVertexArray(0);
      
      glDepthMask(GL_TRUE);
}