#include "../../include/Graphics/Shader.h"
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>


void print(const char *string);
void println(const char *string);

namespace Graphics
{



  Shader::Shader(const char *vertexShaderPath, const char *fragmentShaderPath)
  {
      std::string vertexCode;
      std::string fragmentCode;

      std::fstream vertexShaderFile;
      std::fstream fragmentShaderFile;
      
      vertexShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);
      fragmentShaderFile.exceptions (std::ifstream::failbit | std::ifstream::badbit);

      try {
        vertexShaderFile.open(vertexShaderPath);
        fragmentShaderFile.open(fragmentShaderPath);
        
        std::stringstream vertexShaderStream, fragmentShaderStream;

        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf(); 

        vertexShaderFile.close();
        fragmentShaderFile.close();


        vertexCode = vertexShaderStream.str();
        fragmentCode = fragmentShaderStream.str();
      }


      catch (std::ifstream::failure e)
      {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << e.what() << "\n" << std::endl;
      }

      const char *vertexShaderCode = vertexCode.c_str();
      const char *fragmentShaderCode = fragmentCode.c_str();
      
      unsigned int vertex, fragment;

      vertex = Shader::compileShader(GL_VERTEX_SHADER, vertexShaderCode);
      fragment = Shader::compileShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
      linkShaders(vertex, fragment);

  }




  void Shader::setBool(const std::string &name, bool value) const
  {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
  }


  void Shader::setInt(const std::string &name, int value) const
  {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
  }


  void Shader::setFloat(const std::string &name, float value) const
  {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
  }


  void Shader::linkShaders(unsigned int vertex, unsigned int fragment)
  {
    int success;
    char infoLog[512];

    ID = glCreateProgram();

    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    
    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    if (!success)
    {
      glGetProgramInfoLog(ID, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
      
    }
    
    glDeleteShader(vertex);
    glDeleteShader(fragment);



  }
  
  unsigned int Shader::compileShader(unsigned int type, const char *shaderCode)
  {
    int success;
    char infoLog[512];

    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &shaderCode, NULL);
    glCompileShader(shader);
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

    if (!success)
    {
 
      const char *typeLabel;

      switch (type)
      {
        case GL_VERTEX_SHADER:
          typeLabel = "VERTEX";
          break;
        
        case GL_FRAGMENT_SHADER:
          typeLabel = "FRAGMENT";
          break;
      }
    

      glGetShaderInfoLog(shader, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::" << typeLabel << "::COMPILATION::FAILED\n" << infoLog << std::endl;
      
    }

    return shader;

  }

  void Shader::use()
  {
    glUseProgram(ID);
  }

}
  
void print(const char *string) 
{
  std::cout << string;
}

 
void println(const char *string) 
{
  std::cout << string << std::endl;
}
