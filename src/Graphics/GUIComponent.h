#ifndef GUICOMPONENT_H
#define GUICOMPONENT_H


#include "../../external/imgui/imgui.h"
#include "../../external/imgui/imgui_impl_opengl3.h"
#include "../../external/imgui/imgui_impl_glfw.h"
#include <functional>
#include <string>
#include "FrameBuffer.h"
// abstract class

namespace Graphics {
    class GUIComponent {
    	public: 
    	    //virtual ~GUIComponent();	
    	    virtual void draw() = 0;
    	    virtual void handleInput() = 0;
    };
    
    class TestWindow : public GUIComponent {
        public: 
            TestWindow(std::string name);
	    void draw();
	    void handleInput();
	private: 
	    std::string Name; 
    };

    class SceneWindow : public GUIComponent {
   
	public: 
            SceneWindow( 
                std::string name,
    	        Graphics::FrameBuffer* sceneBuffer,
                std::function<void(float, float)>& resizeFunction,
		std::vector<std::function<void()>>& functionStorage);
            
            void draw();
            void handleInput();
    
        private:
            Graphics::FrameBuffer* SceneBuffer;
            std::string Name;
	    std::function<void(float, float)>& resizeWindow; 
	    std::vector<std::function<void()>>& postRenderFunctions;
    };
}

#endif
