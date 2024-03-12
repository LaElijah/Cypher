
#include "../../include/Graphics/Shader.h"
#include <glad/glad.h>
#include "GLFW/glfw3.h"
#include <math.h>

namespace Graphics {
  class Renderer
  {
    public:
    void draw(Shader shaderProgram, float &visibility, unsigned int VAO, GLFWwindow *window);
  };
}


