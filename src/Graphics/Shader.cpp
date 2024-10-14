#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include "Shader.h"
#include <string>
#include <glm/ext/matrix_transform.hpp>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

const std::set<std::string_view> Graphics::ShaderInfo::supportedExtensions = 
{
    "vs",
    "fs"
};



const std::unordered_map<std::string, Graphics::SHADER_FILE_TYPE> Graphics::ShaderInfo::extensionEnums = 
{
    {"vs", Graphics::SHADER_FILE_TYPE::VERTEX},
    {"fs", Graphics::SHADER_FILE_TYPE::FRAGMENT}
};


void Graphics::ShaderInfo::validateShaderSupport(const std::vector<std::string>& files)
	    {
		std::set<std::string> uniqueExtensions;
	        for (const std::string& file : files)
		{
	            std::string extension = Graphics::FileReader::splitFileExtension(file).second;

	            if (supportedExtensions.find(extension) == supportedExtensions.end())
                    {
		        throw std::invalid_argument("Unsupported file extension: " + extension); 
		    }

		    if (!uniqueExtensions.insert(extension).second)
		    {
		        throw std::invalid_argument("Duplicate file extension: " + extension); 
		    }
		}	
	    }



Graphics::OpenGLShader::OpenGLShader(Graphics::ShaderInfo& infoData)
	: Shader<OpenGLShader>(infoData)
{
    ID = glCreateProgram();

    for (Graphics::ShaderFileData shader : info.shaders)
    {
        linkShader(compileShader(shader));
    }
}
 
  
void Graphics::OpenGLShader::linkShader(unsigned int shader)
{
    int success;
    char infoLog[512];

    glAttachShader(ID, shader);
    glLinkProgram(ID);
    
    glGetProgramiv(ID, GL_LINK_STATUS, &success);

    if (!success)
    {
      glGetProgramInfoLog(ID, 512, NULL, infoLog);
      std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
      
    }
    
    glDeleteShader(shader); 
}

 
unsigned int Graphics::OpenGLShader::compileShader(Graphics::ShaderFileData& file)
  {
      int success;
      char infoLog[512]; 
      const char* typeLabel;
      unsigned int shader;

      switch (file.type)
      {
          case Graphics::SHADER_FILE_TYPE::VERTEX:
            typeLabel = "VERTEX";
            shader = glCreateShader(GL_VERTEX_SHADER);
            break;
          
          case Graphics::SHADER_FILE_TYPE::FRAGMENT:
            typeLabel = "FRAGMENT";
            shader = glCreateShader(GL_FRAGMENT_SHADER);
            break;
      }
     
      const char* fileData = file.data.c_str();
      glShaderSource(shader, 1, &fileData, NULL);
      glCompileShader(shader);
      glGetShaderiv(shader, GL_COMPILE_STATUS, &success);

      if (!success)
      {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cout << "ERROR::SHADER::" << typeLabel << "::COMPILATION::FAILED\n" << infoLog << std::endl;
        
      }

      return shader;
  }




void Graphics::OpenGLShader::useImpl()
  {
    glUseProgram(ID);
  }


	
void Graphics::OpenGLShader::setUniformImpl(std::string name, bool value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int) value);
} 

void Graphics::OpenGLShader::setUniformImpl(std::string name, int value)
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
} 

void Graphics::OpenGLShader::setUniformImpl(std::string name, float value)
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Graphics::OpenGLShader::setUniformImpl(
    	    std::string name, 
    	    float value1, 
    	    float value2, 
    	    float value3)
{
    glUniform3f(
        glGetUniformLocation(ID, name.c_str()),
       	value1, value2, value3);
}

void Graphics::OpenGLShader::setUniformImpl(std::string name, glm::mat4 value)
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));
}







