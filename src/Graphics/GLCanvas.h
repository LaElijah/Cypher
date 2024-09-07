#ifndef GLCANVAS_H
#define GLCANVAS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>




namespace Graphics
{




    class GLCanvas 
    {
        public:
            GLCanvas(unsigned int width, 
		     unsigned int height,
	       	     unsigned int version = 3);
            
            void captureMouse();
            void releaseMouse(); 
            GLFWwindow* getWindow();

           void resizeCanvas(unsigned int width, unsigned height);
           std::pair<unsigned int, unsigned int> getResolution();

        private: 
            bool fullscreen = false; 
            int VERSION;
            int lastX;
            int lastY;
            bool firstMouse;

	    void initializeCanvas();
	    std::string  WindowName;
            GLFWwindow* Window;
            glm::vec3 cameraFront;

            void startWindow(unsigned int& width, unsigned int& height); 
            void loadGlad(); 
    };
}

#endif
