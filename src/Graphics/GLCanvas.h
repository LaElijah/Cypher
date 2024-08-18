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
            GLCanvas(unsigned int width, unsigned int height, unsigned int version = 3);
            
	    void initializeCanvas();
            void captureMouse();
            void releaseMouse(); 
            GLFWwindow* getWindow();

        private: 
            bool fullscreen = false; 
	    int ScreenWidth;
            int ScreenHeight;
            int VERSION;
            int lastX;
            int lastY;
            bool firstMouse;

            const char* WindowName;
            GLFWwindow* Window;
            glm::vec3 cameraFront;

            void startWindow(); 
            void loadGlad(); 
    };
}

#endif
