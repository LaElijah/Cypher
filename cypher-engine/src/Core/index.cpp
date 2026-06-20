#include "Application.h"
#include <string> 

// Application IN
int main(int argc, char *argv[]) 
{
    Core::Application app = Core::GLFWApplication();
    
    app.start(std::string(argv[1]));
    return 0; 
}
