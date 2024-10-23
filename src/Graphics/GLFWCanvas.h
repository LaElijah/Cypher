#ifndef GLFWCANVAS_H
#define GLFWCANVAS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <memory>
#include "Camera.h"
#include "GUI.h"

namespace Graphics
{
    class GLFWCanvas 
    {
        public:
            GLFWCanvas(std::pair<unsigned int, unsigned int> resolution,
		     std::shared_ptr<Graphics::Camera> camera,
		     std::shared_ptr<Graphics::GUI> gui,
	       	     unsigned int version = 3); 
		     
            void captureMouse();
            void releaseMouse();
            float getWidth();
            float getHeight();

	    float getLastX();
	    float getLastY();
	    void setLastX(double X);
	    void setLastY(double Y);

	   // Canvas Settings

	    void resizeViewport(unsigned int width, unsigned int height); 
            std::pair<unsigned int, unsigned int> getResolution();
            void setResolution(float width, float height);
	    void updateDeltaTime();
	    float getDeltaTime();

            GLFWwindow* getWindow();
	    //void toggleDecoration();
	    
        private: 
	    float currentFrame = 0;
	    float deltaTime = 0;
	    float lastFrame = 0;

	    bool fullscreen = false; 
 
	    std::string  WindowName;
            GLFWwindow* m_Window;

       	    int VERSION;
            int lastX;
            int lastY;
	    float Width;
	    float Height;
            bool firstMouse;

            void startWindow(unsigned int& width, unsigned int& height); 
	    void initialize();
            void loadGlad(); 
	    std::shared_ptr<Graphics::Camera> Camera;
	    std::shared_ptr<Graphics::GUI> GUI;

	    void mouseButtonCallbackImpl(GLFWwindow* window, int button, int action, int mods); 
	    
	    void mouseCallbackImpl(GLFWwindow *window, double xpos, double ypos);
	    
	    static void mouseCallback(GLFWwindow *window, double xpos, double ypos);
	    static void mouseButtonCallback(GLFWwindow* window, int button, int action, int mods);
	    static void scrollCallback(GLFWwindow *window, double xoffset, double yoffset);
            
    };
}

#endif
