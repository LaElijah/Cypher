#ifndef GUI_H
#define GUI_H

#include "../../external/GLAD/glad.h"
#include <GLFW/glfw3.h>
#include "../../external/imgui/imgui.h"
#include "../../external/imgui/backends/imgui_impl_opengl3.h"
#include "../../external/imgui/backends/imgui_impl_glfw.h"
#include "ResourceManager.h"
#include "FrameBuffer.h"
#include "Camera.h"
#include "GUIComponent.h"

namespace Graphics 
{
    /**
     * This class provides GUI debgugging and 
     * editor functionality to application.
     *
     * Planned Features: 
     * - Editior to add objects to the scene and move 
     *   them around
     *
     *
     * Known Bugs: 
     * - Issues with selecting and losing focus with when 
     *   going in and out of camera mode.
     */
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



            static bool isWithinWindow(ImVec2 mousePos, ImVec2 windowPos, ImVec2 windowSize);
            static bool isWithinContentRegion(ImVec2 mousePos, ImVec2 contentRegionMin, ImVec2 contentRegionMax);



	private:	   
	    bool WINDOWED = true; 
	    bool GUI_ENABLED = false;
	    std::map<std::string, Graphics::GUIComponent*> Components;
	    std::map<std::string, Graphics::GUIComponent*> EditorComponents;
	    std::set<std::string> loggedComponents;


    };
}


#endif
