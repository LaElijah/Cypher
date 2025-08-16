#include "../Graphics/Camera.h"
#include "../Graphics/ResourceManager.h"
#include "../Graphics/GLFWCanvas.h"
#include "../Graphics/Renderer.h"
#include "../Graphics/RenderAPI.h"
#include "../Graphics/SystemManager.h" 
#include "Application.h"




void Core::GLFWApplication::startImpl()
{

    std::pair<float, float> resolution = std::make_pair(1920, 1080);

    // Initializing required resources	
    Graphics::OpenGLRenderAPI api = Graphics::OpenGLRenderAPI(); 
    std::shared_ptr<Graphics::Camera> Camera = std::make_shared<Graphics::Camera>(resolution);
    std::shared_ptr<Graphics::GUI> GUI = std::make_shared<Graphics::GUI>();
    std::shared_ptr<Graphics::SystemManager> SystemManager = std::make_shared<Graphics::SystemManager>();
    std::shared_ptr<Graphics::ResourceManager> ResourceManager = std::make_shared<Graphics::ResourceManager>();
    
    std::shared_ptr<Graphics::GLFWCanvas> Canvas = std::shared_ptr<Graphics::GLFWCanvas>
    (
        new Graphics::GLFWCanvas
            (
	            resolution,
    	        Camera, 
    	        GUI
            )
    );

    // Starts renderer on construction
    Graphics::Renderer renderer = Graphics::Renderer
    (
        Canvas,
        Camera,
        GUI,
        SystemManager,
        ResourceManager,
        api
    );
}
