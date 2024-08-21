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


/*

void Graphics::Renderer::Draw()
{
    for (Graphics::Model model : ResourceManager->loadedModels)
    {
        
    }
}
*/

void Graphics::Renderer::run()
{
    ResourceManager->loadModelPaths();
    glEnable(GL_DEPTH_TEST);

    GLFWwindow *window = Canvas->getWindow();
    initializeGUI(window); 
    
    Graphics::Shader simpleShader(
        "/home/laelijah/Gengine/data/Shaders/simpleModel.vs",
        "/home/laelijah/Gengine/data/Shaders/simpleModel.fs");

  
    Graphics::Model simpleModel("/home/laelijah/Gengine/data/Models/adamHead/adamHead.gltf", ResourceManager);

    ImGuiIO& io = ImGui::GetIO();
       
    while (!glfwWindowShouldClose(window)) 
    {   
 
        updateDeltaTime();	    
	clear();
         
        if (!io.WantCaptureKeyboard)
        {
            processInput(window);
        }

        simpleShader.use();

        simpleShader.setMat4("model", simpleModel.getModelMatrix());        
        simpleShader.setMat4("view", Camera->getViewMatrix());
        simpleShader.setMat4("projection", Camera->getProjectionMatrix());
        
	simpleModel.Draw(simpleShader);

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




void Graphics::Renderer::clear()
{
    glClearColor(0, 0.01f, 0.01f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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


