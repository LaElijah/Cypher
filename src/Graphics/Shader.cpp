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
#include "RenderAPI.h"
#include <regex>




//OPENGL API
//

std::map<std::string, unsigned int>  Graphics::OpenGLShader::GLSLPrimitives = 
		{
	            {"float", GL_FLOAT },	
	            {"uint", GL_UNSIGNED_INT},	
	            {"vec2", GL_FLOAT_VEC2},	
	            {"vec3", GL_FLOAT_VEC3},	
	            {"vec4", GL_FLOAT_VEC4},	
	            {"ivec2", GL_INT_VEC2},	
	            {"ivec3", GL_INT_VEC3},	
	            {"ivec4", GL_INT_VEC4},	
	            {"uvec2", GL_UNSIGNED_INT_VEC2},	
	            {"uvec3", GL_UNSIGNED_INT_VEC3},	
	            {"uvec4", GL_UNSIGNED_INT_VEC4},	
	            {"mat2", GL_FLOAT_MAT2},	
		
		};



unsigned int Graphics::OpenGLShader::GetPrimitiveBaseType(unsigned int type)
    {
	switch (type)
	{
	    case GL_FLOAT:
	    case GL_FLOAT_VEC2:  
	    case GL_FLOAT_VEC3: 
	    case GL_FLOAT_VEC4: 
	    case GL_FLOAT_MAT2: 
	    case GL_FLOAT_MAT3:
	    case GL_FLOAT_MAT4:
	    case GL_FLOAT_MAT2x3:
	    case GL_FLOAT_MAT2x4: 
	    case GL_FLOAT_MAT3x2: 
	    case GL_FLOAT_MAT3x4:  
	    case GL_FLOAT_MAT4x2: 
	    case GL_FLOAT_MAT4x3:
                return GL_FLOAT;

	    case GL_INT:
	    case GL_INT_VEC2:
	    case GL_INT_VEC3: 
	    case GL_INT_VEC4:
                return GL_INT; 

	    case GL_UNSIGNED_INT: 
	    case GL_UNSIGNED_INT_VEC2: 
	    case GL_UNSIGNED_INT_VEC3: 
	    case GL_UNSIGNED_INT_VEC4: 
		return GL_UNSIGNED_INT;

/* NOT YET SUPPORTED UNTIL GL VERSION > 4.1
	    case GL_DOUBLE:
            case GL_DOUBLE_VEC2: 
	    case GL_DOUBLE_VEC3:  
	    case GL_DOUBLE_VEC4:  
	    case GL_DOUBLE_MAT2:  
	    case GL_DOUBLE_MAT3:
	    case GL_DOUBLE_MAT4:
	    case GL_DOUBLE_MAT2x3:
	    case GL_DOUBLE_MAT2x4:
	    case GL_DOUBLE_MAT3x2:
	    case GL_DOUBLE_MAT3x4:
	    case GL_DOUBLE_MAT4x2:
	    case GL_DOUBLE_MAT4x3:
		return GL_DOUBLE;
*/	
            default:
                return 0; 
	}
    }


 
    size_t Graphics::OpenGLShader::GetPrimitiveSize(unsigned int type)
    {

        switch (type) 
	{
            case GL_FLOAT:                return sizeof(float);
            case GL_FLOAT_VEC2:           return sizeof(float) * 2;
            case GL_FLOAT_VEC3:           return sizeof(float) * 3;
            case GL_FLOAT_VEC4:           return sizeof(float) * 4;
            case GL_FLOAT_MAT2:           return sizeof(float) * 2 * 2;
            case GL_FLOAT_MAT3:           return sizeof(float) * 3 * 3;
            case GL_FLOAT_MAT4:           return sizeof(float) * 4 * 4;
            case GL_FLOAT_MAT2x3:         return sizeof(float) * 2 * 3;
            case GL_FLOAT_MAT2x4:         return sizeof(float) * 2 * 4;
            case GL_FLOAT_MAT3x2:         return sizeof(float) * 3 * 2;
            case GL_FLOAT_MAT3x4:         return sizeof(float) * 3 * 4;
            case GL_FLOAT_MAT4x2:         return sizeof(float) * 4 * 2;
            case GL_FLOAT_MAT4x3:         return sizeof(float) * 4 * 3;
            case GL_INT:                  return sizeof(int);
            case GL_INT_VEC2:             return sizeof(int) * 2;
            case GL_INT_VEC3:             return sizeof(int) * 3;
            case GL_INT_VEC4:             return sizeof(int) * 4;
            case GL_UNSIGNED_INT:         return sizeof(unsigned int);
            case GL_UNSIGNED_INT_VEC2:    return sizeof(unsigned int) * 2;
            case GL_UNSIGNED_INT_VEC3:    return sizeof(unsigned int) * 3;
            case GL_UNSIGNED_INT_VEC4:    return sizeof(unsigned int) * 4;

/* NOT YET SUPPORTED UNTIL GL VERSION > 4.1
            case GL_DOUBLE:               return sizeof(double);
            case GL_DOUBLE_VEC2:          return sizeof(double) * 2;
            case GL_DOUBLE_VEC3:          return sizeof(double) * 3;
            case GL_DOUBLE_VEC4:          return sizeof(double) * 4;
            case GL_DOUBLE_MAT2:          return sizeof(double) * 2 * 2;
            case GL_DOUBLE_MAT3:          return sizeof(double) * 3 * 3;
            case GL_DOUBLE_MAT4:          return sizeof(double) * 4 * 4;
            case GL_DOUBLE_MAT2x3:        return sizeof(double) * 2 * 3;
            case GL_DOUBLE_MAT2x4:        return sizeof(double) * 2 * 4;
            case GL_DOUBLE_MAT3x2:        return sizeof(double) * 3 * 2;
            case GL_DOUBLE_MAT3x4:        return sizeof(double) * 3 * 4;
            case GL_DOUBLE_MAT4x2:        return sizeof(double) * 4 * 2;
            case GL_DOUBLE_MAT4x3:        return sizeof(double) * 4 * 3;
*/

            default:                      return 0; // Unknown type
        }	
    }



std::map<size_t, std::vector<Graphics::OpenGLVertexAttribute>> Graphics::OpenGLShader::m_Formats;

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

	            if (extensionEnums.count(extension) <= 0)
                    {
		        throw std::invalid_argument("Unsupported file extension: " + extension); 
		    }

		    if (!uniqueExtensions.insert(extension).second)
		    {
		        throw std::invalid_argument("Duplicate file extension: " + extension); 
		    }
		}	
	    }

std::pair<size_t, std::vector<Graphics::OpenGLVertexAttribute>> Graphics::OpenGLShader::getFormat()
{
    return std::make_pair(formatKey, m_Formats[formatKey]);
}

Graphics::OpenGLShader::OpenGLShader(Graphics::ShaderInfo& infoData)
	: Shader<OpenGLShader>(infoData)
{
    ID = glCreateProgram();

    std::regex attributeRegex(R"(layout\s*\(location\s*=\s*(\d+)\)\s*in\s+(\w+)\s+(\w+);)");
    std::regex sizeRegex(R"([a-z]+(\d+))");
    std::regex typeRegex(R"((\w+[2-4]))");
    std::smatch matches;
    std::smatch sizeMatch;
    std::smatch typeMatch;

    
    for (Graphics::ShaderFileData shader : info.shaders)
    {
        std::stringstream source = std::stringstream(shader.source);
        // Processing file for attributes etc
        std::string line;
	unsigned int offset = 0;
	unsigned int attrIndex = 1;
	int testSize = 3;

        while(std::getline(source, line))
        {
	    if (std::regex_search(line, matches, attributeRegex)) 
	    {
        
	    
		    //attr.location = std::stoi(matches[1]); // Convert location to int
		    
		std::string attrType = matches.str(2);
		std::string attrName = matches.str(3);
                std::regex_search(
		            attrType, 
		            sizeMatch, 
		            sizeRegex);


		std::cout << "COMPONENT SIZE: " << attrType << std::endl;
		std::cout << "TRUE COMPONENT SIZE: " << sizeMatch[1] << std::endl;

	        std::regex_search(
				attrType, 
				typeMatch,
				typeRegex);

	        std::string typeName = typeMatch[0];

		m_Attributes.push_back(Graphics::OpenGLVertexAttribute{
		        attrName,                                          // Name
		        (
		            std::regex_search(
		            attrType, 
		            sizeMatch, 
		            sizeRegex)
		        ) ? std::stoi(sizeMatch[1])
                          : 1,	                                           // Size (as in component size)
		        GetPrimitiveBaseType(GLSLPrimitives[typeName]),    // Base Type
		        GLSLPrimitives[typeName],                          // Component Type
		        offset,                                            // offset
		        GL_FALSE		                           // normalize
		    });

		offset += GetPrimitiveSize(GLSLPrimitives[typeName]);
		formatKey += GLSLPrimitives[typeName] * (16 + attrIndex);
		++attrIndex;

	    }
        } // END LOOP	
        storeFormat(formatKey);	
        linkShader(compileShader(shader));
    }


    // LOADING ATTRIBUTES AND UNIFORMS
    GLint numUniforms;
    
    GLchar name[256];
    GLint size;
    GLenum type;
    GLsizei length;
    unsigned int offset;
     

    glGetProgramiv(ID, GL_ACTIVE_UNIFORMS, &numUniforms);

    for (GLuint i = 0; i < numUniforms; i++) 
    {
        glGetActiveUniform(ID, i, sizeof(name), &length, &size, &type, name);
        printf("Uniform %d: Name=%s, Size=%d, Type=%u\n", i, name, size, type);

	m_Uniforms.push_back(Graphics::OpenGLUniform
			{
		            name,
		            size,
		            type	    
			});
    }



}
 
void Graphics::OpenGLShader::storeFormat(size_t& key)
{
    if (m_Formats.count(key))
    {
        std::vector<Graphics::OpenGLVertexAttribute> foundAttributes = m_Formats[key];
        unsigned int numAttributes = foundAttributes.size();

        if (numAttributes != m_Attributes.size())
        {
            ++key;
            storeFormat(key);
        }

        else
        {
            for (int i = 0; i < numAttributes; i++)
            {
                if(foundAttributes[i].name != m_Attributes[i].name
        	        || foundAttributes[i].size != m_Attributes[i].size
        	        || foundAttributes[i].type != m_Attributes[i].type)
        	{
                    ++key;
                    storeFormat(key);	 
        	        break;   
        	}	
            }	    
        }
    }
    m_Formats[key] = m_Attributes;
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
     
      const char* source = file.source.c_str();
      glShaderSource(shader, 1, &source, NULL);
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







