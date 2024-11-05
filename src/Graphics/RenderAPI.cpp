#include "../../external/GLAD/glad.h"
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

//void Graphics::OpenGLRenderAPI::drawElementsImpl(int count, bool unbind)
void Graphics::OpenGLRenderAPI::drawElementsImpl(int callNums, bool unbind)
{
    glMultiDrawElementsIndirect(
		    GL_TRIANGLES,
		    GL_UNSIGNED_INT,
		    nullptr,
		    callNums,
		    0);      
    
    
    // "Index" of the buffer or essentialy where to
			                       // start reading indexes for the specified count amount
			                       // so each mesh has an indices count and i can 
			                       // calculate the offset easily by keeping 
			                       // track of what byte im storing on
			                       // per mesh data
				               ///////////////
				               // primcount = mesh count

    //glDrawElements(GL_POINTS, count, GL_UNSIGNED_INT, 0);

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
    glGenBuffers(1, &config.IBO); 
    glGenBuffers(1, &config.SSBO); 
   

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


/*
void Graphics::OpenGLRenderAPI::loadDataImpl
(
    std::vector<Graphics::Vertex>& vertices, 
    std::vector<unsigned int>& indices, 
    std::vector<Graphics::ElementDrawCall>& drawCalls,
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
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, config.IBO);

            
        CURRENT_FORMAT = config.format;
    }

    // This is where the issue occurs, it will operate normally
    // if I only call this once, but once there are draw calls 
    // written I cant call it again or else my rendering issue occurs
    // even if I were to call glBindBuffer every frame
    glBufferData
    (
        GL_DRAW_INDIRECT_BUFFER, 
        sizeof(ElementDrawCall) * drawCalls.size(), 
        drawCalls.data(), 
        GL_DYNAMIC_DRAW
    );


    glBufferData(GL_ARRAY_BUFFER, sizeof(Graphics::Vertex) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_DYNAMIC_DRAW);
 
}

*/

int even = 1;
bool done = false; 
int count = 1;
void Graphics::OpenGLRenderAPI::loadDataImpl
(
    std::vector<Graphics::Vertex>& vertices, 
    std::vector<unsigned int>& indices, 
    std::vector<Graphics::ElementDrawCall>& drawCalls,
    std::string shaderName
)
{
	glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT | GL_COMMAND_BARRIER_BIT);
    size_t format = m_Shaders[shaderName]->getFormat().first;
    Graphics::RenderConfig& config = getRenderConfig(format, shaderName);

    if (CURRENT_FORMAT != config.format && CURRENT_FORMAT != -1)
    {
	std::cout << "BINDING NEW VAO" << std::endl;
        glBindVertexArray(config.VAO);
 
       	glBindBuffer(GL_ARRAY_BUFFER, config.VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, config.EBO);

        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, config.SSBO);
        glBufferData
	    (
	        GL_DRAW_INDIRECT_BUFFER, 
	        sizeof(ElementDrawCall) * drawCalls.size(), 
	        drawCalls.data(), 
	        GL_DYNAMIC_DRAW
	    );

glBindBuffer(GL_DRAW_INDIRECT_BUFFER, config.IBO);
	 

       	glBufferData
	    (
	        GL_DRAW_INDIRECT_BUFFER, 
	        sizeof(ElementDrawCall) * drawCalls.size(), 
	        drawCalls.data(), 
	        GL_DYNAMIC_DRAW
	    );

        CURRENT_FORMAT = config.format;
    }
    else 
    {
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, config.SSBO);
    }

        glBufferSubData
	(
	    GL_DRAW_INDIRECT_BUFFER, 
	    0, 
	    sizeof(ElementDrawCall) * drawCalls.size(), 
	    drawCalls.data() 
	);


    glBufferData(GL_ARRAY_BUFFER, sizeof(Graphics::Vertex) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);
	
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_DYNAMIC_DRAW);

    std::vector<GLsizei> counts;
    std::vector<const void*> indicesOffsets;
    std::vector<GLint> baseVertices;
    // Fill the arrays with data from drawCalls
    for (const auto& drawCall : drawCalls) {
        counts.push_back(drawCall.count);
        indicesOffsets.push_back(reinterpret_cast<const void*>(drawCall.firstIndex * sizeof(unsigned int)));
        baseVertices.push_back(drawCall.baseVertex);
    }

    // Call glMultiDrawElementsBaseVertex
    // Note: the mode should be the same for all draw calls
    glMultiDrawElementsBaseVertex(
        GL_TRIANGLES,                   // Primitive type (must be the same for all)
        counts.data(),             // Pointer to the counts array
        GL_UNSIGNED_INT,           // Type of indices (assuming GLuint here)
	indicesOffsets.data(),     // Pointer to the offsets array
        drawCalls.size(),          // Number of draw calls
        baseVertices.data()        // Pointer to the base vertex array
    );
}
     
    
