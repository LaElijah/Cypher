#include <glad/glad.h>
#include <memory>
#include <iostream>
#include "RenderAPI.h"


std::shared_ptr<Graphics::OpenGLShader> Graphics::OpenGLRenderAPI::getShaderImpl(std::string name)
{
    return (m_Shaders[name]);
}


void Graphics::OpenGLRenderAPI::loadShaderImpl(Graphics::ShaderInfo& info)
{
    m_Shaders.emplace
    (
        info.name, 
	std::shared_ptr<Graphics::OpenGLShader>
	    (new Graphics::OpenGLShader(info))
    );	  
}


void Graphics::OpenGLRenderAPI::loadShadersImpl(std::map<std::string, Graphics::ShaderInfo>& infoData)
{
    for 
    (
        std::pair<std::string, Graphics::ShaderInfo> pair 
	: infoData 
    )
    { 
        loadShaderImpl(pair.second); 
    }
}



void Graphics::OpenGLRenderAPI::loadTexturesImpl(std::vector<Graphics::Texture>& textures)
{
    unsigned int diffuseN = 1;
    unsigned int specularN = 1;
    unsigned int ambientN = 1;
    unsigned int lightmapN = 1;
    unsigned int reflectionN = 1;
   
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

          
        std::string number;
        std::string name = textures[i].type;



        if(name == "texture_diffuse")
          number = std::to_string(diffuseN++);
        
	else if (name == "texture_specular")
          number = std::to_string(specularN++);

	else if (name == "texture_ambient")
          number = std::to_string(ambientN++);

	else if (name == "texture_lightmap")
          number = std::to_string(lightmapN++);

	else if (name == "texture_reflection")
          number = std::to_string(reflectionN++);


        //shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);

    }
    glActiveTexture(GL_TEXTURE0);
}

void Graphics::OpenGLRenderAPI::drawElementsImpl(int count, bool unbind)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, 0);

    if (unbind)
	resetFormat();
}

void Graphics::OpenGLRenderAPI::resetFormat()
{
    glBindVertexArray(0);
    CURRENT_FORMAT = -1;
}

void Graphics::OpenGLRenderAPI::clearImpl()
{
    // Move implememntation to api 
    glClearColor(0, 0.01f, 0.01f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

Graphics::RenderConfig& Graphics::OpenGLRenderAPI::generateRenderConfig(size_t format, std::string shaderName)
{
    std::cout << "GENERATING NEW RENDER CONFIG FOR: " << shaderName << std::endl;
    Graphics::RenderConfig config;
    
    config.format = format;
    
    glGenVertexArrays(1, &config.VAO); 
    glBindVertexArray(config.VAO);

    glGenBuffers(1, &config.VBO);
    glGenBuffers(1, &config.EBO); 

    glBindBuffer(GL_ARRAY_BUFFER, config.VBO);

    std::vector<Graphics::OpenGLVertexAttribute> attributes = m_Shaders[shaderName]
	                                                          ->getFormat().second;

    for (int i = 0; i < attributes.size(); i++)
    {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer
        (
            i, 
            attributes[i].size, 
            attributes[i].type, 
            attributes[i].normalize, 
            sizeof(Graphics::Vertex), 
            // ONLY REINTERPRETING TO PASS TO API
            reinterpret_cast<void*>(attributes[i].offset) 
        );
    }

    m_RenderConfigs[shaderName] = std::move(config);
    return getRenderConfig(format, shaderName);
}


Graphics::RenderConfig& Graphics::OpenGLRenderAPI::getRenderConfig
(
    size_t format, 
    std::string shaderName
)
{
    if (m_RenderConfigs.count(shaderName)) 
	return m_RenderConfigs[shaderName];
    else
        return generateRenderConfig(format, shaderName);
}



void Graphics::OpenGLRenderAPI::loadDataImpl
(
    std::vector<Graphics::Vertex>& vertices, 
    std::vector<unsigned int>& indices, 
    std::string shaderName
)
{
    size_t format = m_Shaders[shaderName]->getFormat().first;
    Graphics::RenderConfig& config = getRenderConfig(format, shaderName);

    if (CURRENT_FORMAT != config.format && CURRENT_FORMAT != -1)
    {
	std::cout << "BINDING NEW VAO" << std::endl;
        glBindVertexArray(config.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, config.VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, config.EBO);
	CURRENT_FORMAT = config.format;
    }
     
    glBufferData(GL_ARRAY_BUFFER, sizeof(Graphics::Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
}
