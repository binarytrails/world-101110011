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

#include <SOIL/SOIL.h>

#include <noise/noise.h>
// FIXME #3 normalize and make vary in range; currenly linear grow with (x, z)
#include "TerrainHeight.hpp"

//! Class for the ground of the world
class Terrain : public Mesh
{
    public:
        Terrain(const uint16_t xcells, const uint16_t zcells);
        ~Terrain();

        //! Returns the elevation at a given cell for the y (height) value
        float getElevation(const float x, const float z);
        GLenum getRenderMode() const;

        void setRenderMode(const GLenum renderMode);

        void render(const Window* window,
                    const Camera* camera,
                    const glm::mat4 view,
                    const glm::mat4 projection);

        //! Rotates the terrain by n spins on a given x, y, z values
        void rotate(const glm::vec3 spin);

        //! TODO WIP method to generate more terrain on the same grid range
        bool advance(const bool forward);

    private:
        //! Initializes the buffers for the shaders
        void initBuffers();

        //! Centralizing the action of adding a vertex for the terrain cell
        void addVertex(const float _x, const float _z, const bool elevate);

        void build();

        void initTextures();
        //! Prepares the texture for the shaders
        void loadTexture(const std::string fpath);

        //! Recursive method to generate a plane
        void genPlaneVertices(uint16_t x, uint16_t z);

        //! Recursive method to generate a terrain with elevation
        void genTerrainVertices(uint16_t x,     uint16_t z,
                                uint16_t max_x, uint16_t max_z);

        //! Generates terrain vertices indices with three render modes
        void genVerticesI();

        //! Uploads vertices to the GPU
        void upload();
        //! Updates the Homogeneous coordinates matrices in the vertex shader
        void updateMVP(const glm::mat4 view, const glm::mat4 projection);

        //! Draws the terrain using EBO indices
        void draw();

        Shader *shader;

        GLuint vboId,
               vaoId,
               eboId,
               texGrass,
               texRocks;

        //! Texture filepath
        const std::string texGrassFPath;
        const std::string texRocksFPath;

        GLenum renderMode;
        glm::mat4 model;
        glm::mat4 view;

        std::vector<glm::vec3> vertices;
        std::vector<GLushort> verticesI;

        const uint16_t X_CELLS;
        const uint16_t Z_CELLS;
        //! Keep it a small float value
        const float MAX_HEIGHT;
        //! The x and z offset attributes are used by the advance method
        short x_offset;
        short z_offset;

        //! Libnoise::Perlin for terrain elevation
        noise::module::Perlin perlinNoise;
        // FIXME #3
        //! Custom noise functions to generate height map values
        TerrainHeight *elevation;
};
