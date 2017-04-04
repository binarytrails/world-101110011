/*
 * @file
 * @author Vsevolod (Seva) Ivanov
*/

#include "Camera.hpp"

Camera::Camera() :
    eye(0.0f, 0.0f, 0.0f),
    at(0.0f, 0.0f, -1.0f),
    up(0.0f, 1.0f, 0.0f),
    speed(0.05f)
{
}

Camera::~Camera(){}

glm::mat4 Camera::view() const
{
    return glm::lookAt(eye, eye + at, up);
}

void Camera::moveForward()
{
    this->eye += this->speed * this->at;
}

void Camera::moveBackward()
{
    this->eye -= this->speed * this->at;
}

void Camera::moveUp()
{
    this->eye += this->speed * this->up;
}

void Camera::moveRight()
{
    this->eye -= glm::normalize(glm::cross(this->at, this->up)) * this->speed;
}

void Camera::moveDown()
{
    this->eye -= this->speed * this->up;
}

void Camera::moveLeft()
{
    this->eye += glm::normalize(glm::cross(this->at, this->up)) * this->speed;
}

void Camera::printView() const
{
    const float *pSource = (const float*) glm::value_ptr(this->view());
    printf("+-----------------------------------------+\n");
    for (int i = 0; i < 16; i++)
    {
        printf("%f ", pSource[i]);
        if (i % 4 == 0)
            printf("\n");
    }
    printf("\n+-----------------------------------------+\n");
}
