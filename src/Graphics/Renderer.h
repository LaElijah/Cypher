#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "ResourceManager.h"
#include "GLCanvas.h"




namespace Graphics {




    class Renderer 
    {
	public:
            Renderer();
            Renderer(Graphics::ResourceManager* resourceManager, Graphics::GLCanvas* canvas, Graphics::Camera* camera);
        
	    void run();
	    void shutdown();
	  
            void disableGUI();
            void enableGUI();
	 
	    float getDeltaTime();

	    Graphics::ResourceManager* getResourceManager();
	    Graphics::GLCanvas* getCanvas();
	    Graphics::Camera* getCamera();
   	
	private:	    
	    float currentFrame;
	    float deltaTime;
	    float lastFrame;
	    float SCREEN_WIDTH = 1920;
	    float SCREEN_HEIGHT = 1080;
	    bool GUI_ENABLED = true;
            
            Graphics::Camera* Camera;
	    Graphics::GLCanvas* Canvas;
	    Graphics::ResourceManager* ResourceManager;
	   
	    void drawGUI();
	    void initializeGUI(GLFWwindow* window);
	    void updateDeltaTime();
            void processInput(GLFWwindow *window);	
    };
}

