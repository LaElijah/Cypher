


#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>




  






namespace Graphics {
  enum Movement 
  {
    LEFT,
    RIGHT,
    FORWARDS,
    BACKWARDS,
    UP,
    DOWN
  };

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



    void processMousePosition(double xpos, double ypos)
    {

      float xoffset = xpos - lastX;
      float yoffset = lastY - ypos;
      lastX = xpos;
      lastY = ypos;

      float sensitivity = 0.1f;
      xoffset *= sensitivity;
      yoffset *= sensitivity;

      yaw += xoffset;
      pitch += yoffset;

      if (pitch > 89.0f)
      {
        pitch = 89.0f;
      }
      else if (pitch < -89.0f)
      {
        pitch = -89.0f;
      }



    }


    void setLastX(double X)
    {
      lastX = X;
    }

    void setLastY(double Y)
    {
      lastY = Y;
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
    }

    glm::vec3 getDirection()
  {
    return CameraFront;
  }

    glm::mat4 getViewMatrix()
    {
      return glm::lookAt(CameraPos, CameraPos + CameraFront, CameraUp);
    }





 
  void processMovement(int keyPressed, float deltaTime)
  { 
    const float cameraSpeed = 2.5f * deltaTime;

  
    switch (keyPressed)
    {

      case -1:
      CameraPos += glm::vec3(cameraSpeed + CameraFront.x, 0, cameraSpeed + CameraFront.z); 
      break;

       
      case LEFT:   
      CameraPos -= glm::normalize(glm::cross(CameraFront, CameraUp) + cameraSpeed);
      break;

      case BACKWARDS: 
      CameraPos += glm::vec3(cameraSpeed - CameraFront.x, 0, cameraSpeed - CameraFront.z);  
      break;

      case RIGHT:
      CameraPos += glm::normalize(glm::cross(CameraFront, CameraUp) + cameraSpeed);
      break;

      case UP:      
      CameraPos += glm::vec3(0, CameraUp.y + cameraSpeed, 0);
      break;

      case DOWN:
      CameraPos += glm::vec3(0, cameraSpeed - CameraUp.y, 0);
      break;

  }
}








    private:
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
 
    glm::vec3 CameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    glm::vec3 CameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    glm::vec3 CameraDirection = glm::normalize(CameraPos - CameraTarget); 
    glm::mat4 Projection = glm::mat4(1.0f);
    glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
    glm::vec3 CameraRight = glm::normalize(glm::cross(Up, CameraDirection));
    glm::vec3 CameraUp = glm::cross(CameraDirection, CameraRight);


     void setCameraFront(glm::vec3 direction)
    {
      CameraFront = direction;
    }


};

}




