#ifndef GLFWCANVAS_H
#define GLFWCANVAS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>


namespace Graphics
{
    class GLFWCanvas 
    {
        public:
            GLFWCanvas(unsigned int width, 
		     unsigned int height,
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
            GLFWwindow* Window;

       	    int VERSION;
            int lastX;
            int lastY;
	    float Width;
	    float Height;
            bool firstMouse;

            void startWindow(unsigned int& width, unsigned int& height); 
	    void initialize();
            void loadGlad(); 
    };
}

#endif
