
#include <glad/glad.h>
#include "RenderAPI.h"
#include <memory>
#include <iostream>




//OPENGL API
std::shared_ptr<Graphics::OpenGLShader> Graphics::OpenGLRenderAPI::getShaderImpl(std::string name)
{
    return (m_Shaders[name]);
}


void Graphics::OpenGLRenderAPI::loadShaderImpl(Graphics::ShaderInfo& info)
{
    m_Shaders.emplace(info.name, std::shared_ptr<Graphics::OpenGLShader>(new Graphics::OpenGLShader(info)));	  
}


void Graphics::OpenGLRenderAPI::loadShadersImpl(std::map<std::string, Graphics::ShaderInfo>& infoData)
{

    for (std::pair<std::string, Graphics::ShaderInfo> pair    
        : infoData)
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
        glBindVertexArray(0);
}

void Graphics::OpenGLRenderAPI::clearImpl()
{
    // Move implememntation to api 
    glClearColor(0, 0.01f, 0.01f, 1.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

// Move implememntation to api 
// Should be indexed by shader name 
Graphics::RenderConfig& Graphics::OpenGLRenderAPI::generateRenderConfig(VAO_TYPE vaoType, std::string shaderName)
{
    Graphics::RenderConfig config;
    
    config.vaoType = vaoType;
    
    glGenVertexArrays(1, &config.VAO); 
    glBindVertexArray(config.VAO);

    glGenBuffers(1, &config.VBO);
    glGenBuffers(1, &config.EBO); 

    glBindBuffer(GL_ARRAY_BUFFER, config.VBO);

    switch (vaoType)
        case DEBUG:
            {
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Graphics::Vertex), (void*)0);


                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Graphics::Vertex), (void*)offsetof(Graphics::Vertex, Graphics::Vertex::Normal));


                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Graphics::Vertex, Graphics::Vertex::TexCoords));
            }

	 
    m_RenderConfigs[shaderName] = std::move(config);

    return getRenderConfig(vaoType, shaderName);
}


Graphics::RenderConfig& Graphics::OpenGLRenderAPI::getRenderConfig(
		Graphics::VAO_TYPE vaoType, 
		std::string shaderName)
{
    // Was the vao found in our vector?
    if (m_RenderConfigs.count(shaderName)) 
    {
        // Return the found render entitiy
	return m_RenderConfigs[shaderName];
    }
    else 
    {
	std::cout << "GENERATING NEW RENDER CONFIG FOR: " << shaderName << std::endl;
        return generateRenderConfig(vaoType, shaderName);
    }
}



void Graphics::OpenGLRenderAPI::loadDataImpl(
	                    std::vector<Graphics::Vertex>& vertices, 
			    std::vector<unsigned int>& indices, 
			    std::string shaderName)
{
   
    // Change this to input vaotype from shader	
    Graphics::RenderConfig& config = getRenderConfig(DEBUG, shaderName);

    if (CurrentVao != DEBUG)
    {
        glBindVertexArray(config.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, config.VBO);
    }
     
    glBufferData(GL_ARRAY_BUFFER, sizeof(Graphics::Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, config.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
 
}
