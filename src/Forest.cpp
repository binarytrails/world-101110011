/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Forest.cpp
 * Author: sami
 * 
 * Created on April 14, 2017, 12:12 PM
 */

#include "Forest.h"

Forest::Forest(const uint16_t xcells, const uint16_t zcells):
    X_CELLS(xcells), Z_CELLS(zcells), renderMode(GL_TRIANGLES) {
    this->shader = new Shader("src/shaders/objecttest.vs",
                              "src/shaders/objecttest.fs");
    this->build();
}

Forest::~Forest() {
}

GLenum Forest::getRenderMode() const {
    return this->renderMode;
}

void Forest::setRenderMode(const GLenum renderMode) {
    this->renderMode = renderMode;
}

void Forest::render(const Window* window,
                    const Camera* camera,
                    const glm::mat4 view,
                    const glm::mat4 projection) {
    this->shader->use();
    this->updateMVP(view, projection);
    this->draw();
}

void Forest::rotate(const glm::vec3 spin) {
    this->model = glm::rotate(this->model, spin.x, glm::vec3(1, 0, 0));
    this->model = glm::rotate(this->model, spin.y, glm::vec3(0, 1, 0));
    this->model = glm::rotate(this->model, spin.z, glm::vec3(0, 0, 1));
}

void Forest::build() {
    Point offset(0,0,0,Vector(0,1,0));
    PGTree1 tree1 = PGTree1(offset,6);
    objects.push_back(new GLObject(tree1.objects));
}

void Forest::updateMVP(const glm::mat4 view, const glm::mat4 projection)
{
    // locate in shaders
    GLint modelLoc = glGetUniformLocation(this->shader->programId, "model");
    GLint viewLoc = glGetUniformLocation(this->shader->programId, "view");
    GLint projLoc = glGetUniformLocation(this->shader->programId, "projection");

    // send to shaders
    glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(this->model));
    glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
}

void Forest::draw()
{
    // TODO remove
    glm::vec3 translations[1];
    glm::vec3 translation;
    translation.x = 10;
    translation.y = 0;
    translation.z = 10;
    translations[0] = translation;
    
    for(int c = 0; c < objects.size(); c++) {
        objects.at(c)->draw(translations, 1);
    }
}
