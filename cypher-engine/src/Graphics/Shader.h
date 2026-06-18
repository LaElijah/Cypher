#ifndef SHADER_H
#define SHADER_H

#include <iostream>
#include <glm/glm.hpp>
#include <set>
#include <string>
#include <sstream>
#include <fstream>
#include "FileReader.h"
#include <unordered_map>
#include <map>

namespace Graphics 
{
    
    enum SHADER_FILE_TYPE
    {
        VERTEX,
        FRAGMENT,
	COMPUTE
    };
    
    struct ShaderFileData 
    {
        SHADER_FILE_TYPE type;
        std::string source; 
        std::string path;
    };
    
    struct ShaderInfo
    {
	public: 
            std::vector<ShaderFileData> shaders;
	    std::string name;
    
            ShaderInfo(
	        std::string Name,
    	        std::vector<std::string> files
		)
            {
                name = Name; 
                loadData(files);   
	    };
             

	    static const std::unordered_map<std::string, Graphics::SHADER_FILE_TYPE> extensionEnums;
        private:
	    void loadData(std::vector<std::string>& files)
	    {
	        validateShaderSupport(files); 

                for (std::string& file : files)
		{
		    ShaderFileData data;
	            std::fstream shaderFile;	
                     
                    shaderFile.exceptions(
    	    	    	std::ifstream::failbit 
    	    	    	| std::ifstream::badbit);
    
                    try 
                    {
                        shaderFile.open(file);
		 	std::stringstream shaderSource;
    	                shaderSource << shaderFile.rdbuf();
		   	shaderFile.close();


			data.type = extensionEnums.find
			(
			    Graphics::FileReader::getFileExtension
			    (
			        Graphics::FileReader::getFileName(file)
			    ).second
			)->second;

                        data.source = shaderSource.str();
    	                data.path = std::string(file);
                    
		    	shaders.push_back(data);			
    	            }
                    catch (std::ifstream::failure e)
                    {
                        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ\n" << e.what() << "\n" << std::endl;
                    }
		}
	    }		

            
	    // TODO: Get compile time loading for this from supported extensions
	    static const int maxShaderTypes;

            void validateShaderSupport(const std::vector<std::string>& files);
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


    struct OpenGLVertexAttribute
    {
        std::string name;
        int size;
        unsigned int type;	
	unsigned int component;
        int offset;
	unsigned int normalize;
    };

    struct OpenGLUniform
    {
        std::string name;
        int size;
        unsigned int type;	
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

	    std::pair<size_t, std::vector<Graphics::OpenGLVertexAttribute>> getFormat(); 

        private:
	    unsigned int ID;
	    size_t formatKey;

	    unsigned int compileShader(Graphics::ShaderFileData& shader);
            void linkShader(unsigned int shader);
	    void storeFormat(size_t& key);

	    std::vector<Graphics::OpenGLVertexAttribute> m_Attributes;
	    std::vector<Graphics::OpenGLUniform> m_Uniforms;

	    static std::map<std::string, unsigned int> GLSLPrimitives;
	    static std::map<size_t, std::vector<Graphics::OpenGLVertexAttribute>> m_Formats;
            static unsigned int GetPrimitiveBaseType(unsigned int type);
	    static size_t GetPrimitiveSize(unsigned int type);
    };


}






#endif
