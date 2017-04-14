#include "Skybox.h"
#include "../src/Shader.hpp"

Skybox::Skybox(std::vector<const GLchar*>& faces)
{ 

  // Set up and compile our shaders.
  shader = new Shader("src/shaders/advanced.vs", "src/shaders/advanced.frag");
  skyboxShader = new Shader("src/shaders/skybox.vs", "src/shaders/skybox.frag");

  // Call our texture loading method.
  this->loadCubeMap(faces);
}

Skybox::~Skybox()
{

}

void Skybox::loadCubeMap(std::vector<const GLchar*>& faces)
{
  glGenTextures(1, &cubeMapID);

  int width, height;
  unsigned char* image;

  glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);
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

}

void Skybox::renderSkybox(Window* window, Camera* camera,
              const glm::mat4& view, const glm::mat4& projection)
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
  glGenVertexArrays(1, &skyboxVAO);
      glGenBuffers(1, &skyboxVBO);
      // Bind buffers for skybox VAO
      glBindVertexArray(skyboxVAO);
      glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
      glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), &skyboxVertices, GL_STATIC_DRAW);
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (GLvoid*)0);
    glBindVertexArray(0);

      // Draw skybox
      // glDepthMask(GL_FALSE);
      // skyboxShader->use();
      // // Removing translation component of vector.
      // glm::mat4 view3 = glm::mat4(glm::mat3(view));
      // // glm::mat4 projection = glm::perspective(45.0f, (float)800/(float)600, 0.1f, 100.0f);
      // glUniformMatrix4fv(glGetUniformLocation(skyboxShader->programId, "view"), 1, GL_FALSE, glm::value_ptr(view3));
      // glUniformMatrix4fv(glGetUniformLocation(skyboxShader->programId, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
      // // skybox cube
      // glBindVertexArray(skyboxVAO);
      //   glActiveTexture(GL_TEXTURE0);
      //   glUniform1i(glGetUniformLocation(shader->programId, "skybox"), 0);
      //   glBindTexture(GL_TEXTURE_CUBE_MAP, cubeMapID);
      //   glDrawArrays(GL_TRIANGLES, 0, 36);
      // glBindVertexArray(0);
      // glDepthMask(GL_TRUE); 
}