#include "Camera.hpp"

namespace gps {

    //Camera constructor
    Camera::Camera(glm::vec3 cameraPosition, glm::vec3 cameraTarget, glm::vec3 cameraUp) {
        this->cameraPosition = cameraPosition;
        this->cameraTarget = cameraTarget;
        this->cameraUpDirection = cameraUp;
       // this->cameraDirection = glm::normalize(cameraTarget - cameraPosition);
        
        this->cameraRightDirection = glm::normalize(glm::cross(this->cameraUpDirection, glm::vec3(0.0f, 1.0f, 0.0f)));

    }

    //return the view matrix, using the glm::lookAt() function
    glm::mat4 Camera::getViewMatrix() {
        return glm::lookAt(cameraPosition, cameraTarget, cameraUpDirection);
    }

    //update the camera internal parameters following a camera move event
    void Camera::move(MOVE_DIRECTION direction, float speed) {
        //TODO
        switch (direction) {
        case MOVE_FORWARD:
           // cameraPosition += cameraUpDirection * speed;
            cameraPosition += (cameraTarget - cameraPosition) * speed;
            break;

        case MOVE_BACKWARD:
            //cameraPosition -= cameraUpDirection * speed;
            cameraPosition -= (cameraTarget - cameraPosition) * speed;
            break;

        case MOVE_RIGHT:
            cameraPosition += cameraRightDirection * speed;
            break;

        case MOVE_LEFT:
            cameraPosition -= cameraRightDirection * speed;
            break;
        }
    }

    //update the camera internal parameters following a camera rotate event
    //yaw - camera rotation around the y axis
    //pitch - camera rotation around the x axis
    void Camera::rotate(float pitch, float yaw) {
        //TODO
        cameraUpDirection = glm::vec3(
            cos(pitch) * sin(yaw),
            sin(pitch),
            cos(pitch) * cos(yaw)
        );

        cameraRightDirection = glm::normalize(glm::cross(cameraUpDirection, glm::vec3(0.0f, 1.0f, 0.0f)));
    }
}