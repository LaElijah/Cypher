#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "FileReader.h"
#include "Renderer.h"
#include "Model.h"
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_opengl3.h"
#include "../../external/imgui/imgui_impl_glfw.h"




Graphics::Renderer::Renderer()
{
    ResourceManager = new Graphics::ResourceManager();
    Canvas = new Graphics::GLCanvas(SCREEN_WIDTH, SCREEN_HEIGHT);    
    Camera = new Graphics::Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
}




Graphics::Renderer::Renderer(Graphics::ResourceManager* resourceManager, Graphics::GLCanvas* canvas, Graphics::Camera* camera)
{
    ResourceManager = resourceManager;
    Canvas = canvas;
    Camera = camera;
}




void Graphics::Renderer::run()
{
    ResourceManager->loadModelPaths();
    glEnable(GL_DEPTH_TEST);

    GLFWwindow *window = Canvas->getWindow();
    initializeGUI(window); 
    
    Graphics::Shader simpleShader(
        "/home/laelijah/Gengine/data/Shaders/simpleModel.vs",
        "/home/laelijah/Gengine/data/Shaders/simpleModel.fs");

    Graphics::Shader simpleShader2(
        "/home/laelijah/Gengine/data/Shaders/simpleModel.vs",
        "/home/laelijah/Gengine/data/Shaders/simpleModel.fs");




    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 model2 = glm::mat4(1.0f);
    glm::mat4 model3 = glm::mat4(1.0f);
  
  
    glm::mat4 view = glm::mat4(1.0f);
    glm::mat4 projection = glm::mat4(1.0f);
  
    glm::vec3 lightPos(1.2f, 1.0f, 2.0f);
  
    Graphics::Model simpleModel2("/home/laelijah/Gengine/data/Models/wolverine/scene.gltf", ResourceManager);
    Graphics::Model simpleModel("/home/laelijah/Gengine/data/Models/mic/scene.gltf", ResourceManager);

    ImGuiIO& io = ImGui::GetIO();
      
    while (!glfwWindowShouldClose(window)) 
    {   
        updateDeltaTime();	    
  
        glm::vec3 cameraFront = Camera->getDirection();
     
        if (!io.WantCaptureKeyboard)
        {
            processInput(window);
        }


        glClearColor(0, 0, 0, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        projection = glm::perspective(glm::radians(Camera->Zoom), SCREEN_WIDTH / SCREEN_HEIGHT, 0.1f, 100.0f); 
        view = Camera->getViewMatrix();

        
        model = glm::mat4(1.0f);
        model2 = glm::mat4(1.0f);



        simpleShader.use();
        simpleShader.setMat4("view", view);
        simpleShader.setMat4("projection", projection);


        model = glm::translate(model, glm::vec3(1.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model = glm::scale(model, glm::vec3(0.8f, 0.8f, 0.8f));

        simpleShader.setMat4("model", model);
        
        simpleModel.Draw(simpleShader);






        model2 = glm::translate(model2, glm::vec3(2.0f, 0.0f, 0.0f)); // translate it down so it's at the center of the scene
        model2 = glm::scale(model2, glm::vec3(10.0f, 10.0f, 10.0f));

        simpleShader.setMat4("model", model2);
        


        simpleModel2.Draw(simpleShader);
             // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwPollEvents();
        drawGUI();
        glfwSwapBuffers(window);
    }
    shutdown(); 
    glfwTerminate(); 
}




void Graphics::Renderer::shutdown()
{
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext(); 
}




void Graphics::Renderer::disableGUI()
{
    GUI_ENABLED = false;
}




void Graphics::Renderer::enableGUI()
{
    GUI_ENABLED = true;
}




float Graphics::Renderer::getDeltaTime()
{
    return deltaTime;
}




Graphics::ResourceManager* Graphics::Renderer::getResourceManager()
{
    return ResourceManager;
}




Graphics::GLCanvas* Graphics::Renderer::getCanvas()
{
    return Canvas;
}




Graphics::Camera* Graphics::Renderer::getCamera()
{
    return Camera;
}




void Graphics::Renderer::drawGUI()
{ 
      if (GUI_ENABLED)
      { 
          ImGui_ImplOpenGL3_NewFrame();
          ImGui_ImplGlfw_NewFrame();
          ImGui::NewFrame();
          ImGui::ShowDemoWindow();

          ImGui::Render();
          ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
      } 
}




void Graphics::Renderer::initializeGUI(GLFWwindow* window)
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext(); 
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();  
}




void Graphics::Renderer::updateDeltaTime()
{ 
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame; 
    lastFrame = currentFrame;
}




void Graphics::Renderer::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        Canvas->releaseMouse(); 
	enableGUI();
        Camera->disableCamera(); 
    } 

    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS) 
        Camera->resetPosition(); 

    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) 
        glfwSetWindowShouldClose(window, true);
    
    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        Camera->processKeyboard(Graphics::Direction::FORWARDS, getDeltaTime());

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        Camera->processKeyboard(Graphics::Direction::LEFT, getDeltaTime());

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)   
        Camera->processKeyboard(Graphics::Direction::BACKWARDS, getDeltaTime());

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        Camera->processKeyboard(Graphics::Direction::RIGHT, getDeltaTime());

    if (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS)
        Camera->processKeyboard(Graphics::Direction::UP, getDeltaTime());

    if (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS) 
        Camera->processKeyboard(Graphics::Direction::DOWN, getDeltaTime());    
}


