#ifndef RENDERER_H
#define RENDERER_H


#include <GLFW/glfw3.h>
#include "Camera.h"
#include "ResourceManager.h"
#include "GLFWCanvas.h"
#include "Shader.h"
#include "GUI.h"
#include "RenderAPI.h"
#include <memory>
#include "ResourceManager.h"
#include "Model.h"
#include "Mesh.h"
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_opengl3.h"
#include "../../external/imgui/imgui_impl_glfw.h"
#include "GUIComponent.h"
#include "FrameBuffer.h"
#include <functional>
#include "../../external/GLAD/glad.h"



// POTENTIAL NAME CYPHER;

#include <cstdlib>
#include <ctime>
#include <thread>
namespace Graphics 
{



    class Renderer 
    {
	public:
            Renderer();
            Renderer(std::shared_ptr<Graphics::GLFWCanvas> canvas);

	    template <typename T>
            Renderer
	    (
		std::shared_ptr<Graphics::GLFWCanvas> canvas,
		std::shared_ptr<Graphics::Camera> camera,
		std::shared_ptr<Graphics::GUI> gui,
		std::shared_ptr<Graphics::ResourceManager> resourceManager,
		Graphics::RenderAPI<T>& api
	    )
                : Canvas(canvas),
		  Camera(camera),
		  GUI(gui), 
		  ResourceManager(resourceManager)
            {

		    ResourceManager->initialize();
                    api.loadShaders(ResourceManager->getShaderInfo());

                    GUI->initialize(Canvas->getWindow());
		    run(api);
            }

	    std::shared_ptr<Graphics::ResourceManager> getResourceManager();
	    std::shared_ptr<Graphics::GLFWCanvas> getCanvas();
	    std::shared_ptr<Graphics::Camera> getCamera();
	    std::shared_ptr<Graphics::GUI> getGUI();

            void updateWindow(float width, float height);
	    void shutdown();


	    int count = 4;


       
            // API Dependent functions
                        template <typename T>           
            void draw(Graphics::RenderAPI<T>& renderAPI)
            {
                auto shader = renderAPI.getShader("debug");
                shader->use();
                           
                for 
                (
                    std::shared_ptr<Graphics::Model> model 
                    : ResourceManager->getLoadedModels()
                )
                { 

                    modelMatrix = model->getModelMatrix();
                    shader->setUniform("view", Camera->getViewMatrix());
                    shader->setUniform("projection", Camera->getProjectionMatrix());

                    for (Graphics::Mesh& mesh : model->getMeshes())
                    {
                 
                        Graphics::ElementDrawCall call;
                        shaderName = mesh.getShaderName();

                        if (!done)
                        {
                            vertexData.insert
                            (
                                vertexData.end(), 
                                mesh.getVertices().begin(), 
                                mesh.getVertices().end()
                            ); 

                            indexData.insert
                            (
                                indexData.end(), 
                                mesh.getIndices().begin(), 
                                mesh.getIndices().end()
                            );

                        call.count = mesh.getIndices().size();
                        call.instanceCount = 1;
                        call.firstIndex = currentBaseIndex;
                        call.baseVertex = currentBaseVertex;
                        call.baseInstance = instanceIndex;

                        currentBaseIndex += mesh.getIndices().size();
                        currentBaseVertex += mesh.getVertices().size();
                        instanceIndex++;

                        drawCalls.push_back(std::move(call));
                        }
                    }
                }


                shader->setUniform("model", modelMatrix); 
// Later, before updating the buffer
                renderAPI.loadData
                (
                    vertexData, 
                    indexData,
                    drawCalls,
                    "debug"
                );
   
                    

                //renderAPI.drawElements(drawCalls.size());

                done = true;
            }
			/*
	    template <typename T>	    
            void draw(Graphics::RenderAPI<T>& renderAPI)
            {
		vertexData.clear();
		indexData.clear();
		drawCalls.clear();

    	    	    auto shader = renderAPI.getShader("debug");
                    shader->use();
                    
                    
		for 
		(
	            std::shared_ptr<Graphics::Model> model 
		    : ResourceManager->getLoadedModels()
		)
                { 

		    modelMatrix = model->getModelMatrix();
                    shader->setUniform("view", Camera->getViewMatrix());
                    shader->setUniform("projection", Camera->getProjectionMatrix());

		    for (Graphics::Mesh& mesh : model->getMeshes())
		    {
		 
			Graphics::ElementDrawCall call;
		        shaderName = mesh.getShaderName();

			if (!done)
			{
			    vertexData.insert
			    (
			        vertexData.end(), 
			        mesh.getVertices().begin(), 
			        mesh.getVertices().end()
			    ); 

		            indexData.insert
			    (
			        indexData.end(), 
			        mesh.getIndices().begin(), 
			        mesh.getIndices().end()
			    );

		        call.count = mesh.getIndices().size();	
			call.instanceCount = 1;
			call.firstIndex = currentBaseIndex;
			call.baseVertex = currentBaseVertex;
			call.baseInstance = instanceIndex;
	
		        currentBaseIndex += mesh.getIndices().size();	
			currentBaseVertex += mesh.getVertices().size();
			instanceIndex++;
			
			drawCalls.push_back(std::move(call));
			}
		    }
                }

		        renderAPI.loadData
		        (
		            vertexData, 
			    indexData,
			    drawCalls,
			    "debug"
		        );

		    


		    
	  
		    shader->setUniform("model", modelMatrix); 
		    
		    renderAPI.drawElements(drawCalls.size());
            }a
    */
                   /* 
		   
		    for (Graphics::Mesh& mesh : model->getMeshes())

                    {
            	        renderAPI.loadData(mesh.getVertices(), mesh.getIndices(), model->getShaderName()); 
                        // Move to implementation in api 
	   		//ResourceManager->loadTextures(mesh.getTextures());
            
	   		renderAPI.loadTextures(mesh.getTextures());
            	        renderAPI.drawElements(mesh.getIndices().size());	
            	    }

		    */
	
	    template <typename T>
            void run(Graphics::RenderAPI<T>& renderAPI)
            {
		// Initialize
                GLFWwindow *window = Canvas->getWindow();
                ResourceManager->loadModel("/home/laelijah/Gengine/data/Models/room/scene.gltf");
            
                ImGuiIO& io = GUI->getIO();
                Graphics::FrameBuffer* sceneBuffer = new Graphics::FrameBuffer(1920, 1080);
            
                std::function<void(float, float)> resizeFunction = 
			[this, sceneBuffer] 
			(float width, float height) 
			{
            	            sceneBuffer->updateWindowSize(width, height); 
            	            updateWindow(width, height);
                        };
               
                GUI->addEditorComponent(
            		    new Graphics::SceneWindow(
            			    std::string("Scene"),
            			    sceneBuffer,
            			    resizeFunction, 
            			    PostRenderFunctions));
            
                GUI->addEditorComponent(new Graphics::TestWindow(std::string("Test")));
                GUI->addComponent(new Graphics::TestWindow(std::string("WOAAH")));
		// Initialize End
                               // Render loop
                while (!glfwWindowShouldClose(window)) 
                {   
                    if (true) 
                    {
                        processInput(window);
                    }

                    //renderAPI.clear(); 

                    // For drawing to scene window
                    // within gui
                    if (GUI->isWindowed())
                    {
                        sceneBuffer->Bind();  
                        renderAPI.clear(); 
                    } 

                    renderAPI.clear(); 
                    draw(renderAPI); 
                    
		    if (GUI->isWindowed())
                        sceneBuffer->Unbind();      

                    GUI->drawGUI();

                    glfwPollEvents();
                    
		    glfwSwapBuffers(window);
		    
                   

                    // Handle post render duties
                    while (PostRenderFunctions.size() > 0)
                    {
                        PostRenderFunctions.back()();
                        PostRenderFunctions.pop_back(); 
                    }
                    
                    Canvas->updateDeltaTime();      
                }
		/*
	        // Render loop	
                while (!glfwWindowShouldClose(window)) 
                {   
                    if (true)
                    {
                        processInput(window);
                    }

	            renderAPI.clear(); 

	            if (GUI->isWindowed())
		    {
                        sceneBuffer->Bind();  
		        renderAPI.clear(); 
		    } 
		    draw(renderAPI); 
		    if (GUI->isWindowed())
		        sceneBuffer->Unbind(); 	    

		    GUI->drawGUI();
		    glfwPollEvents();

            	    glfwSwapBuffers(window);
            	        
                   
		    done = true; 
		    // Clear default frame buffer 	
            	    // Handle post render duties
            	    while (PostRenderFunctions.size() > 0)
            	    {
            	        PostRenderFunctions.back()();
            	        PostRenderFunctions.pop_back(); 
            	    }
                    
		    Canvas->updateDeltaTime();	    
                }a
		*/
                
		GUI->shutdown(); 
                glfwTerminate(); 
            }


	private:	    
	    float SCREEN_WIDTH = 1920;
	    float SCREEN_HEIGHT = 1080;
           
	    std::shared_ptr<Graphics::GUI> GUI; 
	    std::shared_ptr<Graphics::Camera> Camera;
	    std::shared_ptr<Graphics::GLFWCanvas> Canvas;
	    std::shared_ptr<Graphics::ResourceManager> ResourceManager;

            std::vector<std::function<void()>> PostRenderFunctions;
	    
            void processInput(GLFWwindow *window);
    		std::vector<Graphics::Vertex> vertexData;
    		std::vector<unsigned int> indexData;
		std::vector<int> counts;
		
		std::vector<const void *> indices;
		std::vector<int> baseVertices; // Current vertex offset per mesh
               
	       	unsigned int currentBaseIndex = 0;
	       	int currentBaseVertex = 0;
	       	unsigned int instanceIndex = 0;

	    	    std::vector<Graphics::ElementDrawCall> drawCalls;
	
		std::string shaderName;
	        glm::mat4 modelMatrix;	
		bool done = false;


	

    };
}


#endif
