#ifndef SHADER_H
#define SHADER_H

 
#include <string>
#include <glm/ext/matrix_transform.hpp>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

namespace Graphics {

  class Shader
  {
    public:
      
      unsigned int ID;
      
      Shader(const char *vertexShaderPath, const char *fragmentShaderPath); 
      Shader(std::string vertexShaderPath, std::string fragmentShaderPath);

      void use();

      void setBool(const std::string &name, bool value) const;
      void setInt(const std::string &name, int value) const;

      void setFloat(const std::string &name, float value) const;
      void setVec3(const std::string &name, float value1, float value2, float value3) const;
      void setMat4(const std::string &name, glm::mat4 value) const;

    private:
      unsigned int compileShader(unsigned int type, const char *shaderCode);
      void linkShaders(unsigned int vertex, unsigned int fragment);
  };      
}


#endif
