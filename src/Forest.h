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


class Forest : public Mesh {
public:
    Forest();
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
    
    void build();
    void updateMVP(const glm::mat4 view, const glm::mat4 projection);
    void draw();
};

#endif /* FOREST_H */

