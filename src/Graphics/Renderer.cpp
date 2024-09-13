
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
#include "GUI.h"
#include "GUIComponent.h"
#include "FrameBuffer.h"
#include <functional>

Graphics::Renderer::Renderer()
{
    ResourceManager = new Graphics::ResourceManager();
    Canvas = new Graphics::GLCanvas(SCREEN_WIDTH, SCREEN_HEIGHT);    
    Camera = new Graphics::Camera(SCREEN_WIDTH, SCREEN_HEIGHT);
    GUI = new Graphics::GUI();
}




Graphics::Renderer::Renderer(Graphics::ResourceManager* resourceManager, Graphics::GLCanvas* canvas, Graphics::Camera* camera)
{
    ResourceManager = resourceManager;
    Canvas = canvas;
    Camera = camera;
    GUI = new Graphics::GUI();
}



void Graphics::Renderer::draw()
{

    for (Graphics::Model* model : ResourceManager->getLoadedModels())
    {
 
	Graphics::Shader* shader = ResourceManager->getShader(model->getShaderName());
        shader->use();
        shader->setMat4("model", model->getModelMatrix());      
        shader->setMat4("view", Camera->getViewMatrix());
        shader->setMat4("projection", Camera->getProjectionMatrix());
        
 
        for (Graphics::Mesh& mesh : model->getMeshes())
        {
            ResourceManager->bindBuffers(mesh.getVertices(), mesh.getIndices(), DEBUG); 
            ResourceManager->loadTextures(mesh.getTextures());
 
            glDrawElements(GL_TRIANGLES, (mesh.getIndices()).size(), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
	
	}
    }
 }





void Graphics::Renderer::updateWindow(float width, float height)
{
 	  Canvas->resizeViewport(
			  width, 
			  height);

  	  Camera->setAspectRatio(
			  width 
			  / height);
	  //glViewport(0, 0, width, height);
}


void Graphics::Renderer::run()
{

    GLFWwindow *window = Canvas->getWindow();
 
    ResourceManager->loadModel(new Graphics::Model("/home/laelijah/Gengine/data/Models/room/scene.gltf"));


    ImGuiIO& io = GUI->getIO();

    Graphics::FrameBuffer* sceneBuffer = new Graphics::FrameBuffer(1920, 1080);

    std::function<void(float, float)> resizeFunction = [this, sceneBuffer] (float width, float height) {
	    sceneBuffer->updateWindowSize(width, height); 
	    updateWindow(width, height);
    };
   
    GUI->addEditorComponent(
		    new Graphics::SceneWindow(
			    std::string("Scene"),
			    sceneBuffer,
			    resizeFunction, 
			    PostRenderFunctions)
		    );

    GUI->addEditorComponent(new Graphics::TestWindow(std::string("Test")));


    GUI->addComponent(new Graphics::TestWindow(std::string("WOAAH")));
      
     
    while (!glfwWindowShouldClose(window)) 
    {   
 
         
        if (true)
        {
            processInput(window);
        }


       
        // Draw to scene frame buffer


        if (GUI->isWindowed())
	{	
	    sceneBuffer->Bind();  
	    clear(); 
	    draw(); 
            sceneBuffer->Unbind(); 

	    
	    GUI->drawGUI();
        }
	
	else 
	{	
	    draw(); 

	    GUI->drawGUI();
	}

	glfwPollEvents();
	glfwSwapBuffers(window);

    
        // Clear default frame buffer 	
	clear();


	// Handle post render duties
	
	while (PostRenderFunctions.size() > 0)
	{
	    PostRenderFunctions.back()();
	    PostRenderFunctions.pop_back(); 
	}

        updateDeltaTime();	    
    }
    GUI->shutdown(); 
    glfwTerminate(); 
}

/*
void Graphics::Renderer::render()
{       
        // Draw to scene frame buffer	
    sceneBuffer->Bind();  
    clear(); 
    draw(); 
    
    
    sceneBuffer->Unbind(); 
    
    
    GUI->drawGUI(sceneBuffer);
}

void Graphics::Renderer::newFrame()
{}
*/


void Graphics::Renderer::clear()
{
    glClearColor(0, 0.01f, 0.01f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
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


Graphics::GUI* Graphics::Renderer::getGUI()
{
    return GUI;
}









void Graphics::Renderer::updateDeltaTime()
{ 
    currentFrame = glfwGetTime();
    deltaTime = currentFrame - lastFrame; 
    lastFrame = currentFrame;
}

// have different "modes" under some identifier key
// like a string naming it, or ma
// into a map and have that map be keys with values 
// of function objects that each have their own implementation of
// the processing of inputs since a person can only have 1 "mode"
// active at a time (theoretically) and pass in the associated key 
// and press type, then


bool toggleWindowPressed = false;
void Graphics::Renderer::processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
    {
        Canvas->releaseMouse(); 
	GUI->enable();
        Camera->disableCamera(); 
    } 

    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS && !toggleWindowPressed) 
    {
	   int width;
	   int height;

	   glfwGetWindowSize(window, &width, &height);

	   std::cout << height << std::endl;
	    GUI->toggleWindow();
            //Canvas->toggleDecoration(); 
	    if (!GUI->isWindowed())
	    {
	        updateWindow(
		    width, 
		    height);

                Canvas->captureMouse(); 
                GUI->disable();	
                Camera->enableCamera();      
	    }
	    else 
	    {
	          Canvas->releaseMouse(); 
		  GUI->enable();	
		  Camera->disableCamera();
		 
	    }

            toggleWindowPressed = true;	    
    }
    else if ( glfwGetKey(window, GLFW_KEY_F) == GLFW_RELEASE)
    {
        toggleWindowPressed = false; 
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



