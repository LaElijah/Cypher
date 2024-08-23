#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "Camera.h"
#include "ResourceManager.h"
#include "GLCanvas.h"
#include "Shader.h"




namespace Graphics {




    class Renderer 
    {
	public:
            Renderer();
            Renderer(Graphics::ResourceManager* resourceManager, Graphics::GLCanvas* canvas, Graphics::Camera* camera);
        
	    void run();
	    void shutdown();
	    void draw();
            
            void clear();	    
            void disableGUI();
            void enableGUI();
	 
	    float getDeltaTime();

	    Graphics::ResourceManager* getResourceManager();
	    Graphics::GLCanvas* getCanvas();
	    Graphics::Camera* getCamera();
   	
	private:	    

            void loadTextures(std::vector<Texture>& Textures);
            void bindBuffers(std::vector<Graphics::Vertex>& vertices, std::vector<unsigned int>& indices, Graphics::VAO_TYPE vaoType );
	    float currentFrame;
	    float deltaTime;
	    float lastFrame;
	    float SCREEN_WIDTH = 1920;
	    float SCREEN_HEIGHT = 1080;
	    bool GUI_ENABLED = true;
            
            Graphics::Camera* Camera;
	    Graphics::GLCanvas* Canvas;
	    Graphics::ResourceManager* ResourceManager;

	    void Draw(); 
	    void drawGUI();
	    void initializeGUI(GLFWwindow* window);
	    void updateDeltaTime();
            void processInput(GLFWwindow *window);	
    };
}

