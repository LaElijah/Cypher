#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "ResourceManager.h"
#include "GLCanvas.h"
#include "Shader.h"
#include "GUI.h"

#include "System.h"
// POTENTIAL NAME CYPHER;


namespace Graphics {




    class Renderer 
    {
	public:
            Renderer();
            Renderer(Graphics::ResourceManager* resourceManager, Graphics::GLCanvas* canvas, Graphics::Camera* camera);
        
	    void run();
	    void shutdown();
	    void draw(std::vector<Graphics::RenderBatch>& batches);
            
            void clear();	    
	    std::string lastShader = "NULL";	 
	    float getDeltaTime();

	    Graphics::ResourceManager* getResourceManager();
	    Graphics::GLCanvas* getCanvas();
	    Graphics::Camera* getCamera();
	    Graphics::GUI* getGUI();
            void updateWindow(float width, float height);

   	
	private:	    

	    std::vector<Graphics::RenderBatch> batchi;
	    float currentFrame = 0;
	    float deltaTime = 0;
	    float lastFrame = 0;
	    
	    float SCREEN_WIDTH = 1920;
	    float SCREEN_HEIGHT = 1080;
           
	    Graphics::GUI* GUI; 
            Graphics::Camera* Camera;
	    Graphics::GLCanvas* Canvas;
	    Graphics::ResourceManager* ResourceManager;

            Graphics::RenderSystem* renderSystem;
            std::vector<std::function<void()>> PostRenderFunctions;
	    
	    void updateDeltaTime();
            void processInput(GLFWwindow *window);	
    };
}

