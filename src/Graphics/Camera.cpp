#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <iostream>
#include "Camera.h"



/*
 * Camera Constructor
 * Initializes the camera object with a given 
 * screen width and height in ints representing pixels.
 * To get the first mouse position, calculate the midpoint of both axis
 * by dividing resolution in half.
 */
Graphics::Camera::Camera(int width, int height)
{
    // Setting screen resolution
    ScreenWidth = width;
    ScreenHeight = height;

    // Calculating screen midpoint for first mouse
    lastX = width / 2;
    lastY = height / 2; 
}



/*
 * processMousePosition
 * This function calculates the new angle of the mouse
 * from the cursors offset from the center
 * when the cursor is captured. 
 * Modululates offset impact with mouse sensitivity
 */
void Graphics::Camera::processMousePosition(double xoffset, double yoffset, bool constrainPitch)
{
    // Modulate offset impact
    xoffset *= MouseSensitivity;
    yoffset *= MouseSensitivity;

    yaw += xoffset;
    pitch += yoffset;
   
    // Prevent axis flipping
    if (constrainPitch)
    {
        if (pitch > 89.0f)
        {
            pitch = 89.0f;
        }
    
        else if (pitch < -89.0f)
        {
            pitch = -89.0f;
        }
    }    
    // Updates camera direction vector
    updateDirection();
}




/*
 * 
 * */
void Graphics::Camera::updateDirection()
{  
    Direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
    Direction.y = sin(glm::radians(pitch));
    Direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch)); 
    CameraFront = glm::normalize(Direction);

    CameraRight = glm::normalize(glm::cross(CameraFront, WorldUp));
    CameraUp = glm::normalize(glm::cross(CameraRight, CameraFront));
}




float Graphics::Camera::getLastX()
{
    return lastX;
}




float Graphics::Camera::getLastY()
{
    return lastY;
}



void Graphics::Camera::setLastX(double X)
{
    lastX = X;
}




void Graphics::Camera::setLastY(double Y)
{
    lastY = Y;
}




bool Graphics::Camera::isFirstMouse()
{
    return firstMouse;
}




void Graphics::Camera::startMouse()
{
    firstMouse = false;
}






glm::vec3 Graphics::Camera::getDirection()
{
    return CameraFront;
}




glm::mat4 Graphics::Camera::getViewMatrix()
{
    return glm::lookAt(Position, Position + CameraFront, CameraUp);
}

glm::mat4 Graphics::Camera::getProjectionMatrix()
{
    return glm::perspective(glm::radians(Zoom), ScreenWidth / ScreenHeight, 0.1f, 100.0f); 
}




void Graphics::Camera::setCameraPos(glm::vec3 position)
{
    Position = position;
}




void Graphics::Camera::processKeyboard(Graphics::Direction keyPressed, float deltaTime)
{ 
    Velocity = MovementSpeed * deltaTime; 
    float currentHeight; 
    
    switch (keyPressed)
    {
      //FORWARDS
        case FORWARDS:
            currentHeight = Position.y; 
            Position += Velocity * CameraFront;
            Position.y = currentHeight;
            break;
 
      // LEFT
        case LEFT:   
            Position -= Velocity * CameraRight; 
            break;

      // BACKWARDS
        case BACKWARDS:
            currentHeight = Position.y;
            Position -= Velocity * CameraFront;
            Position.y = currentHeight;
            break;

      // RIGHT
        case RIGHT: 
            Position += Velocity * CameraRight; 
            break;

      // UP
        case UP:       
            Position += glm::vec3(0, CameraUp.y * Velocity, 0); 
            break;

      // DOWN
        case DOWN: 
            Position -= glm::vec3(0, CameraUp.y * Velocity, 0); 
            break; 
    }
}




void Graphics::Camera::processMouseScroll(float yoffset)
{
    Zoom -= (float) yoffset;
    
    if (Zoom < 1.0f)
        Zoom = 1.0f;
    
    if (Zoom > 45.0f)
        Zoom = 45.0f;
}




void Graphics::Camera::setCameraFront(glm::vec3 direction)
{
    CameraFront = direction;
}




void Graphics::Camera::disableCamera()
{
    CAMERA_ENABLED = false;
}




void Graphics::Camera::enableCamera()
{
    CAMERA_ENABLED = true;
}




bool Graphics::Camera::getCameraStatus()
{
    return CAMERA_ENABLED;
}




void Graphics::Camera::resetPosition()
{
    Position = glm::vec3(0.0f, 0.0f, 3.0f);
}
