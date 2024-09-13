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

	    //void toggleDecoration();
        private: 
            bool fullscreen = false; 
            int VERSION;
            int lastX;
            int lastY;
	    float Width;
	    float Height;
            bool firstMouse;

	    void initialize();
	    std::string  WindowName;
            GLFWwindow* Window;

            void startWindow(unsigned int& width, unsigned int& height); 
            void loadGlad(); 
    };
}

#endif
