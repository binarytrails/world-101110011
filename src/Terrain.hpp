/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#pragma once

#include <stdio.h>
#include <assert.h>

#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"

#include "TerrainHeight.hpp"
#include <SOIL/SOIL.h>

class Terrain : public Mesh
{
    public:
        Terrain(const uint16_t xcells, const uint16_t zcells);
        ~Terrain();

        GLenum getRenderMode() const;

        void setRenderMode(const GLenum renderMode);

        void render(const Window* window,
                    const Camera* camera,
                    const glm::mat4 view,
                    const glm::mat4 projection);

        void rotate(const glm::vec3 spin);

        void advance(const bool forward);

    private:
        void initBuffers();

        float getElevation(const float x, const float z);

        void build();

        void loadTexture();

        void genPlaneVertices();
        void genPlaneVerticesRecursive(uint16_t x, uint16_t z);

        void genTerrainVertices(uint16_t x,        uint16_t z,
                                uint16_t max_x,    uint16_t max_z);
        void genTerrainVerticesRecursive(uint16_t x,        uint16_t z,
                                         uint16_t max_x,    uint16_t max_z);

        void genVerticesI();

        void upload();
        void updateMVP(const glm::mat4 view, const glm::mat4 projection);
        void draw();

        Shader *shader;

        GLuint vboId,
               vaoId,
               eboId,
               texture;

        const std::string texFilepath;

        GLenum renderMode;
        glm::mat4 model;
        glm::mat4 view;

        std::vector<glm::vec3> vertices;
        std::vector<GLushort> verticesI;

        const uint16_t X_CELLS;
        const uint16_t Z_CELLS;
        short x_offset;
        short z_offset;

        TerrainHeight *elevation;
};
