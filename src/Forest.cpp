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

#include "shadow.hpp"
#include "Forest.h"
#include <vector>

Forest::Forest(const uint16_t xcells, const uint16_t zcells, Terrain* terrain):
    X_CELLS(xcells), Z_CELLS(zcells), renderMode(GL_TRIANGLES) {
    this->shader = new Shader("src/shaders/objecttest.vs",
                              "src/shaders/objecttest.fs");
    this->build(terrain);
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

void Forest::build(Terrain* terrain) {
    Point offset(0,0,0,Vector(0,1,0));
    
    PGTree1 tree1 = PGTree1(offset, 0.25f, 0.05f, 1.0f);
    GLObject* myObj1 = new GLObject(tree1.objects);
    
    PGTree2 tree2 = PGTree2(offset, 0.25f, 0.05f, 1.0f);
    GLObject* myObj2 = new GLObject(tree2.objects);

	
    
    // TODO remove
    //glm::vec3 translations1[X_CELLS*Z_CELLS];
    //glm::vec3 translations2[X_CELLS*Z_CELLS];
    
    glm::vec3 translations1[2500];
    glm::vec3 translations2[2500];
    int index1 = 0;
    int index2 = 0;

	int numtrees1 = 0;
	int numtrees2 = 0;


    for(int c = 0; c < 2; c++) {
        for(GLint x = 0; x < X_CELLS; x += 10)
        {
            /*float maxTree = X_CELLS/2 >= x ? x : X_CELLS - x;
            int num = randNum(0, maxTree);
            float space = num > 0 ? x/num : 0;
            std::cout << x << ' ' << maxTree << ' ' << num << ' ' << space << '\n';*/
            for(GLint z = 0 + c * 5; z < Z_CELLS; z += 10)
            {
                glm::vec3 translation;
                /*translation.x = (GLfloat)x / 10.0f + offset2;
                translation.y = (GLfloat)y / 10.0f + offset2;*/
                translation.x = (GLfloat)x;
                translation.z = (GLfloat)z;
                translation.y = terrain->getElevation(translation.x, translation.z);
				
				if (c == 0)
				{
					translations1[index1++] = translation;
					numtrees1++;
				}
				else
				{
					translations2[index2++] = translation;
					numtrees2++;
				}
            }
        }
    }
    
	//-------shadows-------------

	//Sample the terrain around the tree because terrain is weird

	//tree 1
	for (int i = 0; i < numtrees1; i++)
	{
		float sampleoffset = 0.05;
		glm::vec3 translation = translations1[i];
		glm::vec3 p1, p2, p3;
		p1.x = translation.x + sampleoffset;
		p1.z = translation.z;
		p1.y = terrain->getElevation(p1.x, p1.z);
		p2.x = translation.x - sampleoffset;
		p2.z = translation.z - sampleoffset;
		p2.y = terrain->getElevation(p2.x, p2.z);
		p3.x = translation.x - sampleoffset;
		p3.z = translation.z + sampleoffset;
		p3.y = terrain->getElevation(p3.x, p3.z);

		glm::vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));

		Point slope;
		slope.x = translation.x;
		slope.y = translation.y;
		slope.x = translation.z;
		slope.direction.x = normal.x;
		slope.direction.y = normal.y;
		slope.direction.z = normal.z;

		//make the shadow

		for (int j = 0; j < tree1.objects.size(); j++)
		{

			GLObject* myshadow = new Shadow(*tree1.objects[j], translation, slope);

			myshadow->initBuffers(new glm::vec3(0.0, 0.0, 0.0), 1);
			objects.push_back(myshadow);


		}
	}

	//tree 2
	for (int i = 0; i < numtrees2; i++)
	{
		float sampleoffset = 0.05;
		glm::vec3 translation = translations2[i];
		glm::vec3 p1, p2, p3;
		p1.x = translation.x + sampleoffset;
		p1.z = translation.z;
		p1.y = terrain->getElevation(p1.x, p1.z);
		p2.x = translation.x - sampleoffset;
		p2.z = translation.z - sampleoffset;
		p2.y = terrain->getElevation(p2.x, p2.z);
		p3.x = translation.x - sampleoffset;
		p3.z = translation.z + sampleoffset;
		p3.y = terrain->getElevation(p3.x, p3.z);

		glm::vec3 normal = glm::normalize(glm::cross(p2 - p1, p3 - p1));

		Point slope;
		slope.x = translation.x;
		slope.y = translation.y;
		slope.x = translation.z;
		slope.direction.x = normal.x;
		slope.direction.y = normal.y;
		slope.direction.z = normal.z;

		//make the shadow

		for (int j = 0; j < tree1.objects.size(); j++)
		{

			GLObject* myshadow = new Shadow(*tree2.objects[j], translation, slope);

			myshadow->initBuffers(new glm::vec3(0.0, 0.0, 0.0), 1);
			objects.push_back(myshadow);


		}
	}
	
	

	//-------end shadows------------

    myObj1->initBuffers(translations1, index1-1);
    myObj2->initBuffers(translations2, index2-1);
    objects.push_back(myObj1);
    objects.push_back(myObj2);
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
    for(int c = 0; c < objects.size(); c++) {
        objects.at(c)->draw();
    }
}

float Forest::randNum(float min, float max) {
    std::random_device rd;     // only used once to initialise (seed) engine
    std::mt19937 rng(rd());    // random-number engine used (Mersenne-Twister in this case)
    std::uniform_real_distribution<float> uni(min,max); // guaranteed unbiased

    return uni(rng);
}