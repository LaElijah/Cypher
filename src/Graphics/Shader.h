#ifndef SHADER_H
#define SHADER_H

 
#include <string>
#include <glm/ext/matrix_transform.hpp>
#include <math.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>
#include <fstream>
#include <sstream>

#include <iostream>

namespace Graphics {
    constexpr int SHADER_TYPE_SIZE = 2;

    
    enum SHADER_FILE_TYPE
    {
        VERTEX,
        FRAGMENT
    };
    
    
    struct ShaderFileData 
    {
        SHADER_FILE_TYPE type;
        std::string data; 
        std::string path;
    };
    
    struct ShaderInfo
    {
        ShaderFileData shaders[SHADER_TYPE_SIZE];
	std::string name;
    
    
        ShaderInfo(
	    std::string shaderName,
            std::string vertexShaderPath, 
    	    std::string fragmentShaderPath)
        {
            name = shaderName; 
            ShaderFileData vertexData;
            ShaderFileData fragmentData;
    
            std::fstream vertexShaderFile;
            std::fstream fragmentShaderFile;
            
            vertexShaderFile.exceptions(
    	    std::ifstream::failbit 
    	    | std::ifstream::badbit);
    
            fragmentShaderFile.exceptions(
    	    std::ifstream::failbit 
    	    | std::ifstream::badbit);
    
            try 
            {
                vertexShaderFile.open(vertexShaderPath.c_str());
                fragmentShaderFile.open(fragmentShaderPath.c_str());
                
                std::stringstream 
    		    vertexShaderStream, 
    		    fragmentShaderStream;
                
    	    vertexShaderStream << vertexShaderFile.rdbuf();
                fragmentShaderStream << fragmentShaderFile.rdbuf(); 
    
                vertexShaderFile.close();
                fragmentShaderFile.close();
    
    	    // Type
    	    vertexData.type = VERTEX;
    
    	    // Data
                vertexData.data = vertexShaderStream.str();
    
    	    // Path
    	    vertexData.path = vertexShaderPath;
    	    
    	    
    	    fragmentData.type = FRAGMENT;
                fragmentData.data = fragmentShaderStream.str();
    	    fragmentData.path = fragmentShaderPath;

	    shaders[0] = vertexData;
	    shaders[1] = fragmentData;
	    
          }
          catch (std::ifstream::failure e)
          {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << e.what() << "\n" << std::endl;
          }
    
    
        };
    
          
        ShaderInfo(
            const char* shaderName,
            const char* vertexShaderPath, 
    	    const char* fragmentShaderPath)
        {

            name = std::string(shaderName); 
            ShaderFileData vertexData;
            ShaderFileData fragmentData;
    
            std::fstream vertexShaderFile;
            std::fstream fragmentShaderFile;
            
            vertexShaderFile.exceptions(
    			std::ifstream::failbit 
    			| std::ifstream::badbit);
    
            fragmentShaderFile.exceptions(
    			std::ifstream::failbit 
    			| std::ifstream::badbit);
            try 
            {
    
    		//maybe add a switch statement and a loop taht
                    // checks the file typeanme and that will be the 
    		// type to add to the vector thhe
                vertexShaderFile.open(vertexShaderPath);
                fragmentShaderFile.open(fragmentShaderPath);
                
                std::stringstream 
    		    vertexShaderStream, 
    		    fragmentShaderStream;
                
    	    vertexShaderStream << vertexShaderFile.rdbuf();
                fragmentShaderStream << fragmentShaderFile.rdbuf(); 
    
                vertexShaderFile.close();
                fragmentShaderFile.close();
    
    
    	    // Type
    	    vertexData.type = VERTEX;
    
    	    // Data
                vertexData.data = vertexShaderStream.str();
    
    	    // Path
    	    vertexData.path = std::string(vertexShaderPath);
    	    
    	    
    	    fragmentData.type = FRAGMENT;
                fragmentData.data = fragmentShaderStream.str();
    	    fragmentData.path = std::string(fragmentShaderPath);


	    shaders[0] = vertexData;
	    shaders[1] = fragmentData;
	    
    
    	}
          catch (std::ifstream::failure e)
          {
            std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << e.what() << "\n" << std::endl;
          }
    
      };
    
    };

 
    template <typename T>
    class Shader
{
        public:

            Shader(Graphics::ShaderInfo& info) : info(info)
	    {
	         
	    }
	    
	    void use()
	    {
	        static_cast<T*>(this)->useImpl(); 
	    }	   

	    void setUniform(std::string name, bool value)
	    {
	    
	        static_cast<T*>(this)->useImpl(name, value); 
	    } 

	    void setUniform(std::string name, int value)
	    {
	    
	        static_cast<T*>(this)->useImpl(name, value); 
	    } 

	    void setUniform(std::string name, float value)
	    {
	    
	        static_cast<T*>(this)->useImpl(name, value); 
	    }

            void setUniform(
			    std::string name, 
			    float value1, 
			    float value2, 
			    float value3)
	    {
	    
	        static_cast<T*>(this)->setUniformImpl(
				name, 
				value1, 
				value2, 
				value3); 
	    }

	    void setUniform(std::string name, glm::mat4 value)
	    { 
	        static_cast<T*>(this)->setUniformImpl(name, value); 
	    } 


        protected:
	    Graphics::ShaderInfo& info; 


    };


    class OpenGLShader : public Shader<OpenGLShader>
    {

	public: 

            OpenGLShader(Graphics::ShaderInfo& info);
	    void useImpl();

   	    void setUniformImpl(std::string name, bool value);

	    void setUniformImpl(std::string name, int value);
	    void setUniformImpl(std::string name, float value);
	    
	    void setUniformImpl(
			    std::string name, 
			    float value1, 
			    float value2, 
			    float value3);
	    
	    void setUniformImpl(std::string name, glm::mat4 value);


        private:
	    unsigned int ID;

	    unsigned int compileShader(Graphics::ShaderFileData& shader);
            void linkShader(unsigned int shader);

 
    };


}






#endif
