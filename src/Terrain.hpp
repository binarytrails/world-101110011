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

#include <noise/noise.h>

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

    private:
        void initBuffers();

        void onCameraChange(const Camera* camera);

        float getElevation(const float x, const float z);

        void build();
        void buildPlaneGrid();
        void buildPlaneGridRecursive(glm::vec3 v, const bool onetime);
        void buildPlaneGridIndices();
        //TODO void buildPlaneGridIndicesRecursive();

        void upload();
        void updateMVP(const glm::mat4 view, const glm::mat4 projection);
        void draw();

        Shader *shader;

        GLuint vboId,
               vaoId,
               eboId;

        GLenum renderMode;
        glm::mat4 model;
        glm::mat4 view;

        std::vector<glm::vec3> vertices;
        std::vector<uint16_t> verticesI;

        const uint16_t X_CELLS;
        const uint16_t Z_CELLS;

        noise::module::Perlin perlinNoise;
};
