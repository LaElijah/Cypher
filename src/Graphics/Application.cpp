







Core::GLFWApplication::initImpl()
{
    GLFWwindow* window = Canvas->getWindow();  

    glfwSetCursorPosCallback(window,  mouse_callback;
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback); 

    GUI->initialize(window);
    ResourceManager->initialize(); 
   
    Graphics::OpenGLRenderAPI api = Graphics::OpenGLRenderAPI(); 
    api.loadShaders(ResourceManager->getShaderInfo());
    renderer.run(api);
}
