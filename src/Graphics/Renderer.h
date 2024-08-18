
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "ResourceManager.h"
#include "GLCanvas.h"



namespace Graphics {

    class Renderer 
    {
	public:

            Renderer(Graphics::ResourceManager* resourceManager, Graphics::GLCanvas* canvas, Graphics::Camera* camera);
            void start();
	    void end();
	    void drawGUI();
	    bool getGUIStatus();
	    

	    

	    void initializeGUI(GLFWwindow* window);
            void disableGUI();
            void enableGUI();
	    void updateDeltaTime();
	    float getDeltaTime();

	    float currentFrame;
	    float deltaTime;
	    float lastFrame;

	    float SCREEN_WIDTH = 1920;
	    float SCREEN_HEIGHT = 1080;

	    bool GUI_ENABLED = true;
             // Change to private  
            void processInput(GLFWwindow *window);
   	private:
	    Graphics::Camera* Camera;
	    Graphics::GLCanvas* Canvas;
	    Graphics::ResourceManager* ResourceManager;
              };



}

