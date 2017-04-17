/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Forest.h
 * Author: sami
 *
 * Created on April 14, 2017, 12:12 PM
 */

#ifndef FOREST_H
#define FOREST_H

#include <vector>
#include <ctime>
#include <iostream>
#include <random>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Window.hpp"
#include "Camera.hpp"
#include "Shader.hpp"
#include "Mesh.hpp"
#include "GLObject.h"
#include "PGObject.h"
#include "PGTree.h"
#include "PGTree1.h"
#include "PGTree2.h"
#include "Terrain.hpp"


class Forest : public Mesh {
public:
    Forest(const uint16_t xcells, const uint16_t zcells, Terrain* terrain);
    virtual ~Forest();
    
    virtual GLenum getRenderMode() const;

    virtual void setRenderMode(const GLenum renderMode);

    virtual void render(const Window* window,
                        const Camera* camera,
                        const glm::mat4 view,
                        const glm::mat4 projection);

    virtual void rotate(const glm::vec3 spin);
private:
    Shader *shader;
    GLenum renderMode;
    std::vector<GLObject*> objects;
    
    glm::mat4 model;
    glm::mat4 view;
    
    const uint16_t X_CELLS;
    const uint16_t Z_CELLS;
    
    void build(Terrain* terrain);
    void updateMVP(const glm::mat4 view, const glm::mat4 projection);
    void draw();
    float randNum(float min = -0.05f, float max = 0.05f);
};

#endif /* FOREST_H */

