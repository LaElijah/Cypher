#ifndef GUI_H
#define GUI_H


#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_opengl3.h"
#include "../../external/imgui/imgui_impl_glfw.h"

#include "ResourceManager.h"
#include "GLCanvas.h"
#include "FrameBuffer.h"
#include "Camera.h"

namespace Graphics {




    class GUI 
    {
	public:
	    GUI();
            GUI(GLFWwindow* window);
        
	    void drawGUI(Graphics::ResourceManager* resourceManager, Graphics::GLCanvas* canvas, Graphics::Camera* camera, Graphics::FrameBuffer* sceneBuffer);
            
	    void disable();
            void enable();
     
	    ImGuiIO& getIO();
     
	    void shutdown();

	    void initialize(GLFWwindow* window);
	    bool isEnabled();
	private:	    
	    bool GUI_ENABLED = true;



    };
}


#endif
