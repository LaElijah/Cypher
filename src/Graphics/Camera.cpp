



#include <glm/gtc/matrix_transform.hpp>

#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>




  






namespace Graphics {

void mouse_callback(GLFWwindow *window, double xpos, double ypos);

  class Camera
  {
    //glm::vec3 cameraPos = glm::vec3(0.0f, 0.0f, 3.0f);
    //glm::vec3 cameraTarget = glm::vec3(0.0f, 0.0f, 0.0f);
    //glm::vec3 cameraDirection = glm::normalize(cameraPos - cameraTarget);
    
    //glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);
    //glm::vec3 cameraRight = glm::normalize(glm::cross(up, cameraDirection));
    //glm::vec3 cameraUp = glm::cross(cameraDirection, cameraRight);
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
      if (firstMouse == false) 
      {
       firstMouse = true; 
      }

      return !firstMouse;
    }

    void startMouse()
    {
      firstMouse = true;
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


    private:
     int ScreenWidth;
     int ScreenHeight;
     const char *WindowName;
     int VERSION;
 
     float pitch;
     float yaw;

     int lastX;
     int lastY;

     bool firstMouse;

     glm::vec3 Direction;
     glm::vec3 CameraFront;
     void setCameraFront(glm::vec3 direction)
    {
      CameraFront = direction;
    }


};

}




