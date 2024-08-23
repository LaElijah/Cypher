#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Shader.h"
#include "FileReader.h"
#include "Renderer.h"
#include "ResourceManager.h"
#include "Model.h"
#include "Mesh.h"
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



void Graphics::Renderer::loadTextures(std::vector<Graphics::Texture>& textures)
{
    unsigned int diffuseN = 1;
    unsigned int specularN = 1;
    unsigned int ambientN = 1;
    unsigned int lightmapN = 1;
    unsigned int reflectionN = 1;
   
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

          
        std::string number;
        std::string name = textures[i].type;



        if(name == "texture_diffuse")
          number = std::to_string(diffuseN++);
        
	else if (name == "texture_specular")
          number = std::to_string(specularN++);

	else if (name == "texture_ambient")
          number = std::to_string(ambientN++);

	else if (name == "texture_lightmap")
          number = std::to_string(lightmapN++);

	else if (name == "texture_reflection")
          number = std::to_string(reflectionN++);


        //shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);

    }

    glActiveTexture(GL_TEXTURE0);
}


void Graphics::Renderer::bindBuffers(std::vector<Graphics::Vertex>& vertices, std::vector<unsigned int>& indices, Graphics::VAO_TYPE vaoType)
{
	Graphics::RenderResource& resource = ResourceManager->getRenderResource(DEBUG);


     glBindVertexArray(resource.VAO);


   glBindBuffer(GL_ARRAY_BUFFER, resource.VBO);
  
    glBufferData(GL_ARRAY_BUFFER, sizeof(Graphics::Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);


    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resource.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);


}


void Graphics::Renderer::draw()
{

    for (Graphics::Model* model : ResourceManager->getLoadedModels())
    {
        // add variable to denote vao type and shader name 

        //Graphics::Shader& shader = ResourceManager->getShader(DEBUG);
	

	    Graphics::Shader* shader = ResourceManager->getShader(model->getShaderName());
        shader->use();

        shader->setMat4("model", model->getModelMatrix());      
        shader->setMat4("view", Camera->getViewMatrix());
        shader->setMat4("projection", Camera->getProjectionMatrix());
        
 
        for (Graphics::Mesh& mesh : model->getMeshes())
        {
            bindBuffers(mesh.getVertices(), mesh.getIndices(), DEBUG); 

            loadTextures(mesh.getTextures());
 
    glDrawElements(GL_TRIANGLES, (mesh.getIndices()).size(), GL_UNSIGNED_INT, 0);

    glBindVertexArray(0);
	
	}
    }
 }






void Graphics::Renderer::run()
{
    ResourceManager->loadModelPaths();
    glEnable(GL_DEPTH_TEST);

    GLFWwindow *window = Canvas->getWindow();
    initializeGUI(window); 
    ResourceManager->loadShaders();   
   /* 
    ResourceManager->loadShader(new Graphics::Shader(
        "/home/laelijah/Gengine/data/Shaders/DEBUG/debug.vs",
        "/home/laelijah/Gengine/data/Shaders/DEBUG/debug.fs"), "DEBUG_VAO_DEBUG_SHADER");
*/
  
    ResourceManager->loadModel(new Graphics::Model("/home/laelijah/Gengine/data/Models/adamHead/adamHead.gltf"));

    ImGuiIO& io = ImGui::GetIO();
       
    while (!glfwWindowShouldClose(window)) 
    {   
 
        updateDeltaTime();	    
	clear();
         
        if (!io.WantCaptureKeyboard)
        {
            processInput(window);
        }
 
        draw();
	//simpleModel.Draw(debugShader);



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


