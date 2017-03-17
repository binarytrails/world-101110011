/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#pragma once

#include <stdio.h>

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

class Terrain : public Mesh
{
    public:
        Terrain();
        ~Terrain();

        GLenum getRenderMode() const;

        void setRenderMode(const GLenum renderMode);

        void render(const Window* window,
                    const Camera* camera,
                    const glm::mat4 view,
                    const glm::mat4 projection);

        void rotate(const glm::vec3 spin);

    private:
        void initBuffers();

        void build();
        void buildPlaneGrid(const uint16_t xcells, const uint16_t zcells);

        void upload();
        void updateMVP(const glm::mat4 view, const glm::mat4 projection);
        void draw();

        Shader *shader;

        GLuint vboId,
               vaoId,
               eboId;

        GLenum renderMode;

        glm::mat4 model;

        std::vector<glm::vec3> vertices;
        std::vector<uint16_t> verticesI;
};
