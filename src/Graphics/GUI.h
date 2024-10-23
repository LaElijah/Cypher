#ifndef GUI_H
#define GUI_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_opengl3.h"
#include "../../external/imgui/imgui_impl_glfw.h"
#include "ResourceManager.h"
#include "FrameBuffer.h"
#include "Camera.h"
#include "GUIComponent.h"

namespace Graphics {

     bool isWithinWindow(ImVec2 mousePos, ImVec2 windowPos, ImVec2 windowSize);
     bool isWithinContentRegion(ImVec2 mousePos, ImVec2 contentRegionMin, ImVec2 contentRegionMax);





    class GUI 
    {
	public:
	    GUI();
            GUI(GLFWwindow* window);
        
	    void drawGUI();
            
	    void disable();
            void enable();
     
	    ImGuiIO& getIO();
     
	    void shutdown();
	    
	    bool isWindowed();
	    void toggleWindow();
	    bool isEnabled();

	    void initialize(GLFWwindow* window);
	    void addComponent(Graphics::GUIComponent*);
	    void addEditorComponent(Graphics::GUIComponent*);
            void handleInputs();        	
            void handleClick();        	
	private:	   
	    bool WINDOWED = true; 
	    bool GUI_ENABLED = false;
	    std::map<std::string, Graphics::GUIComponent*> Components;
	    std::map<std::string, Graphics::GUIComponent*> EditorComponents;
	    std::set<std::string> loggedComponents;


    };
}


#endif
