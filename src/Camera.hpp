/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#pragma once

#include <stdio.h>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

class Camera
{
    public:
        Camera();
        ~Camera();

        glm::mat4 view() const;

        void setAt(glm::vec3& new_at);

        void moveUp();
        void moveRight();
        void moveDown();
        void moveLeft();

        void moveForward();
        void moveBackward();

        void printView() const;
        void printEyePos() const;

    private:
        //! Camera position.
        glm::vec3 eye;
        //! Point being observed. 
        glm::vec3 at;
        //! Up direction of camera.
        glm::vec3 up;

        GLfloat speed;
};
