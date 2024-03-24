#ifndef CAMERA_H
#define CAMERA_H

#include <glm/ext/quaternion_geometric.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

#include <iostream>
#include "./Globals.cpp"



namespace Graphics {

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

  class Camera
  {
    public: 

    Camera(int width, int height)
    {
      ScreenWidth = width;
      ScreenHeight = height;
      lastX = width / 2;
      lastY = height / 2;



     
    }



    void processMousePosition(double xoffset, double yoffset, bool constrainPitch = true)
    {
      xoffset *= MouseSensitivity;
      yoffset *= MouseSensitivity;

      yaw += xoffset;
      pitch += yoffset;


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

      
updateDirection();
    }


    void setLastX(double X)
    {
      lastX = X;
    }

    void setLastY(double Y)
    {
      lastY = Y;
    }



    float getLastX()
    {
      return lastX;
    }

    float getLastY()
    {
      return lastY;
    }


    bool isFirstMouse()
    {
      return firstMouse;
    }

    void startMouse()
    {
      firstMouse = false;
    }


    void updateDirection()
    {
      
      Direction.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
      Direction.y = sin(glm::radians(pitch));
      Direction.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch)); 
      CameraFront = glm::normalize(Direction);

      CameraRight = glm::normalize(glm::cross(CameraFront, WorldUp));
      CameraUp = glm::normalize(glm::cross(CameraRight, CameraFront));

    }

    glm::vec3 getDirection()
  {
    return CameraFront;
  }

    glm::mat4 getViewMatrix()
    {
      return glm::lookAt(Position, Position + CameraFront, CameraUp);
    }




    void setCameraPos(glm::vec3 position)
    {
      Position = position;
    }

 
  void processKeyboard(Direction keyPressed, float deltaTime)
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


void processMouseScroll(float yoffset)
{
  Zoom -= (float) yoffset;
  if (Zoom < 1.0f)
    Zoom = 1.0f;
  if (Zoom > 45.0f)
    Zoom = 45.0f;
}






    private:

 

    float MouseSensitivity = 0.1f; 
    float MovementSpeed = 2.5f;
    float Velocity;
    float Zoom;
    int ScreenWidth; 
    int ScreenHeight;

   
    float pitch; 
    float yaw;

   
    int lastX; 
    int lastY;
 
    bool firstMouse = true;
 
    glm::vec3 Direction; 
    glm::vec3 CameraFront;

    GLFWwindow *window;

    glm::vec3 Position = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 CameraDirection = glm::normalize(Position - CameraTarget); 
    glm::mat4 Projection = glm::mat4(1.0f);
    glm::vec3 WorldUp = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 CameraRight = glm::normalize(glm::cross(WorldUp, CameraDirection));
    glm::vec3 CameraUp = glm::cross(CameraRight, CameraFront);


     void setCameraFront(glm::vec3 direction)
    {
      CameraFront = direction;
    }


};

}


#endif // !CAMERA_H


