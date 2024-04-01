#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "GLCanvas.h"
#include "Shader.h"
#include <glm/ext/matrix_transform.hpp>
#include <math.h>
#include "../../external/STB_IMAGE/stb_image.h"
//#include "../../include/Graphics/Renderer.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "./Camera.cpp"
#include "Globals.cpp"
#include "Model.h"


// settings
float deltaTime;
float lastFrame = 0.0f;

float SCREEN_WIDTH = 800;
float SCREEN_HEIGHT = 600;
float currentFrame;

void loadImageData(const char *path);
void mouse_callback(GLFWwindow *window, double xpos, double ypos);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);    
void processInput(GLFWwindow *window);

Graphics::Camera camera(SCREEN_WIDTH, SCREEN_HEIGHT);
Graphics::GLCanvas canvas;    
Graphics::Shader simpleShader(
        "/home/laelijah/Programming/Gengine/data/Shaders/simpleModel.vs",
        "/home/laelijah/Programming/Gengine/data/Shaders/simpleModel.fs");
Graphics::Model simpleModel("/home/laelijah/Programming/Gengine/data/Models/backpack/backpack.obj");

    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);

    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);


int main()
{
    GLFWwindow *window = canvas.getWindow();

    stbi_set_flip_vertically_on_load(true);

    glfwSetCursorPosCallback(window,  mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

     
    glEnable(GL_DEPTH_TEST);
   
    while (!glfwWindowShouldClose(window)) 
    {  
      currentFrame = glfwGetTime();
      deltaTime = currentFrame - lastFrame; 
      lastFrame = currentFrame;

      glm::vec3 cameraFront = camera.getDirection();
      processInput(window);

      glClearColor(0, 0, 0, 1.0f);
      glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

      projection = glm::perspective(glm::radians(camera.Zoom), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f); 
      view = camera.getViewMatrix();

     
      model = glm::mat4(1.0f);
      simpleShader.use();
      simpleShader.setMat4("view", view);
      simpleShader.setMat4("projection", projection);

      model = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
      model = glm::scale(model, glm::vec3(1.0f, 1.0f, 1.0f));

      simpleShader.setMat4("model", model);
      
      simpleModel.Draw(simpleShader);
  
           // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
      // -------------------------------------------------------------------------------
      glfwSwapBuffers(window);
      glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // glDeleteVertexArrays(1, &VAOs[0]);
    //glDeleteBuffers(1, &VBOs[0]);
    // glDeleteBuffers(1, &EBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}



void loadImageData(const char* path)
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);


    unsigned int channelType;
    switch (nrChannels)
    {
      case 1:
        channelType = GL_RED;
        break;
      case 3:
        channelType = GL_RGB;
        break;
      case 4: 
        channelType = GL_RGBA;
        break;
    }

    if (data)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, channelType, width, height, 0, channelType, GL_UNSIGNED_BYTE, data); 
      glGenerateMipmap(GL_TEXTURE_2D);
    }

    else 
    {
      std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
    
}



void processInput(GLFWwindow *window)
{


    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
      camera.processKeyboard(Graphics::Direction::FORWARDS, deltaTime);
   
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
      camera.processKeyboard(Graphics::Direction::LEFT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)   
      camera.processKeyboard(Graphics::Direction::BACKWARDS, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
      camera.processKeyboard(Graphics::Direction::RIGHT, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
      camera.processKeyboard(Graphics::Direction::UP, deltaTime);

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
      camera.processKeyboard(Graphics::Direction::DOWN, deltaTime);    
}




void mouse_callback(GLFWwindow *window, double xpos, double ypos)
    {
      if (camera.isFirstMouse())
      {
        camera.setLastX(xpos);
        camera.setLastY(ypos);
        camera.startMouse(); 
      }

      camera.processMousePosition(xpos - camera.getLastX(), camera.getLastY() - ypos); 

      camera.setLastX(xpos);
      camera.setLastY(ypos);
    }


void scroll_callback(GLFWwindow *window, double xoffset, double yoffset)
{
  camera.processMouseScroll(yoffset);
}

