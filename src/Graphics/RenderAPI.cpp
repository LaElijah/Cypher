#include "../../external/GLAD/glad.h"
#include <memory>
#include <iostream>
#include "RenderAPI.h"

#include "../../external/STB_IMAGE/stb_image.h"

std::shared_ptr<Graphics::OpenGLShader> Graphics::OpenGLRenderAPI::getShaderImpl(std::string name)
{
    return (m_Shaders[name]);
}

void Graphics::OpenGLRenderAPI::loadShaderImpl(Graphics::ShaderInfo &info)
{
    m_Shaders.emplace(
        info.name,
        std::shared_ptr<Graphics::OpenGLShader>(new Graphics::OpenGLShader(info)));
}

void Graphics::OpenGLRenderAPI::loadShadersImpl(std::map<std::string, Graphics::ShaderInfo> &infoData)
{
    for (
        std::pair<std::string, Graphics::ShaderInfo> pair : infoData)
    {
        loadShaderImpl(pair.second);
    }
}

int Graphics::OpenGLRenderAPI::flushImpl(Graphics::RenderBatch &batch)
{
    offloadTextureHandles(batch);
    return 0;
}

void Graphics::OpenGLRenderAPI::loadTexturesImpl(std::vector<Graphics::TextureInfo> &textures)
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

        if (name == "texture_diffuse")
            number = std::to_string(diffuseN++);

        else if (name == "texture_specular")
            number = std::to_string(specularN++);

        else if (name == "texture_ambient")
            number = std::to_string(ambientN++);

        else if (name == "texture_lightmap")
            number = std::to_string(lightmapN++);

        else if (name == "texture_reflection")
            number = std::to_string(reflectionN++);

        // shader.setInt(("material." + name + number).c_str(), i);
        // glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);
}

void Graphics::OpenGLRenderAPI::drawElementsImpl(int callNums, bool unbind)
{
    glMultiDrawElementsIndirect(
        GL_TRIANGLES,
        GL_UNSIGNED_INT,
        nullptr,
        callNums,
        0);

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

bool print = true;
std::vector<uint64_t> Graphics::OpenGLRenderAPI::loadTextureHandles(std::vector<Graphics::TextureInfo> &textureInfo)
{
    //std::cout << "Working" << std::endl;
    // TODO: Add support for any texture type
    std::vector<uint64_t> handles;
    for (Graphics::TextureInfo info : textureInfo)
    {

        //std::cout << "Working loop" << std::endl;
        unsigned int id = loadTextureData(info);


        // Retrieve the texture handle after we finish creating the texture
      
        uint64_t handle;
        auto it = loadedTextureHandles.find(id);

        if (it == loadedTextureHandles.end())
            handle = glGetTextureHandleARB(id);
        else
            handle = it->second;


        if (handle == 0)
        {
            std::cerr << "Error! Handle returned null" << std::endl;
            exit(-1);
        }

        /*
        unsigned int handle = glGetTextureHandleARB(id);
        std::cout << "handle" << std::endl;
        */

        if (residentHandles.find(handle) == residentHandles.end())
        {
            glMakeTextureHandleResidentARB(handle);
           
        } 
        handles.push_back(handle);
        

        //std::cout << "Done working loop" << std::endl;
    }


    int id = 0;
    if (print)
        for (unsigned int handle : handles)
        {
            id++; 
        } 


    print = false;
    //std::cout << "Returning: " << handles.size() << " handles" << std::endl;
    return handles;
}

void Graphics::OpenGLRenderAPI::offloadTextureHandles(Graphics::RenderBatch &batch)
{
    for (Graphics::TextureInfo info : batch.textureInfo["diffuse"])
    {
        unsigned int id = loadTextureData(info);
        glMakeTextureHandleNonResidentARB(id);
        residentHandles.erase(id);
    }
}

unsigned int Graphics::OpenGLRenderAPI::loadTextureData(Graphics::TextureInfo &info)
{

    // TODO: Add support for embedded textures
    if (loadedTextureData.find(info.path) != loadedTextureData.end())
    {
        //std::cout << loadedTextureData.size() << std::endl;
        return loadedTextureData[info.path];
    }
    std::string filename = info.directory + '/' + info.path;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    //std::cout << filename << std::endl;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }

    else
    {
        std::cout << "Texture failed to load at path: " << filename << std::endl;
        stbi_image_free(data);
    }

    loadedTextureData.emplace(info.path, textureID);
    //std::cout << "STORED" << std::endl;
    return textureID;
}

Graphics::RenderConfig &Graphics::OpenGLRenderAPI::generateRenderConfig(size_t format, std::string shaderName)
{
    std::cout << "GENERATING NEW RENDER CONFIG FOR: " << shaderName << std::endl;
    Graphics::RenderConfig config;

    config.format = format;

    glGenVertexArrays(1, &config.VAO);
    glBindVertexArray(config.VAO);

    glGenBuffers(1, &config.VBO);
    glGenBuffers(1, &config.EBO);
    glGenBuffers(1, &config.IBO[0]);
    glGenBuffers(1, &config.IBO[1]);
    glGenBuffers(1, &config.SSBO);

    glBindBuffer(GL_ARRAY_BUFFER, config.VBO);

    std::vector<Graphics::OpenGLVertexAttribute> attributes = m_Shaders[shaderName]
                                                                  ->getFormat()
                                                                  .second;

    for (int i = 0; i < attributes.size(); i++)
    {
        glEnableVertexAttribArray(i);
        glVertexAttribPointer(
            i,
            attributes[i].size,
            attributes[i].type,
            attributes[i].normalize,
            sizeof(Graphics::Vertex),
            // ONLY REINTERPRETING TO PASS TO API
            reinterpret_cast<void *>(attributes[i].offset));
    }

    m_RenderConfigs[shaderName] = std::move(config);
    return getRenderConfig(format, shaderName);
}

Graphics::RenderConfig &Graphics::OpenGLRenderAPI::getRenderConfig(
    size_t format,
    std::string shaderName)
{
    if (m_RenderConfigs.count(shaderName))
        return m_RenderConfigs[shaderName];
    else
        return generateRenderConfig(format, shaderName);
}

bool original = true;
void Graphics::OpenGLRenderAPI::loadDataImpl(Graphics::RenderBatch &batch)
{
    size_t format = m_Shaders[batch.shader]->getFormat().first;
    Graphics::RenderConfig &config = getRenderConfig(format, batch.shader);

    std::vector<Graphics::ElementDrawCall> drawCalls;

    unsigned int currentBaseIndex = 0;
    unsigned int currentBaseVertex = 0;
    unsigned int instanceIndex = 0;

    for (int i = 0; i < batch.counts.size(); i++)
    {
        Graphics::ElementDrawCall drawCall;

        drawCall.count = batch.indexCounts[i];
        drawCall.instanceCount = 1;
        drawCall.firstIndex = currentBaseIndex;
        drawCall.baseVertex = currentBaseVertex;
        drawCall.baseInstance = instanceIndex;

        currentBaseIndex += batch.indexCounts[i];
        currentBaseVertex += batch.counts[i];

        instanceIndex++;

        drawCalls.push_back(std::move(drawCall));
    }

    // std::cout << "BATCH VERTEX SIZE: " << std::endl;

    if (CURRENT_FORMAT != config.format && CURRENT_FORMAT != -1)
    {
        std::cout << "BINDING NEW VAO" << std::endl;
        glBindVertexArray(config.VAO);

        glBindBuffer(GL_ARRAY_BUFFER, config.VBO);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, config.EBO);

        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, config.IBO[0]);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, config.SSBO);

        glBufferData(
            GL_DRAW_INDIRECT_BUFFER,
            sizeof(ElementDrawCall) * drawCalls.size(),
            drawCalls.data(),
            GL_DYNAMIC_DRAW);

        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, config.IBO[1]);

        glBufferData(
            GL_DRAW_INDIRECT_BUFFER,
            sizeof(ElementDrawCall) * drawCalls.size(),
            drawCalls.data(),
            GL_DYNAMIC_DRAW);

        glBufferData(
            GL_ARRAY_BUFFER,
            sizeof(Graphics::Vertex) * batch.vertexData.size(),
            &batch.vertexData[0],
            GL_DYNAMIC_DRAW);

        glBufferData(
            GL_ELEMENT_ARRAY_BUFFER,
            sizeof(unsigned int) * batch.indexData.size(),
            &batch.indexData[0],
            GL_DYNAMIC_DRAW);


        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, 0, config.SSBO );

        glBufferData(
            GL_SHADER_STORAGE_BUFFER,
            sizeof(GLuint64) * batch.textureInfo["diffuse"].size(),
            loadTextureHandles(batch.textureInfo.at("diffuse")).data(),
            GL_DYNAMIC_DRAW);

            std::cout << sizeof(GLuint64) << std::endl;



            CURRENT_FORMAT = config.format;
    
    }

    
/*
    glBufferSubData(
        GL_SHADER_STORAGE_BUFFER,
        0,
        sizeof(unsigned int) * batch.textureInfo["diffuse"].size(),
        loadTextureHandles(batch.textureInfo.at("diffuse")).data());
*/
    if (original)
    {
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, config.IBO[0]);

        glBufferSubData(
            GL_DRAW_INDIRECT_BUFFER,
            0,
            sizeof(ElementDrawCall) * drawCalls.size(),
            drawCalls.data());
    }
    else
    {
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, config.IBO[1]);

        glBufferSubData(
            GL_DRAW_INDIRECT_BUFFER,
            0,
            sizeof(ElementDrawCall) * drawCalls.size(),
            drawCalls.data());
    }

    original = !original;

    glBufferSubData(
        GL_ARRAY_BUFFER,
        0,
        sizeof(Graphics::Vertex) * batch.vertexData.size(), &batch.vertexData[0]);

    glBufferSubData(
        GL_ELEMENT_ARRAY_BUFFER,
        0,
        sizeof(unsigned int) * batch.indexData.size(),
        &batch.indexData[0]);
}

unsigned int Graphics::OpenGLRenderAPI::loadTextureImpl(Graphics::TextureInfo &texture)
{
    if (true)
    {
        // return loadFileTexture(texture); //TODO:
        return 0;
    }
}

unsigned int Graphics::OpenGLRenderAPI::loadFileTexture(Graphics::TextureInfo &texture)
{
    std::string filename = std::string(texture.path);
    filename = texture.directory + '/' + filename;

    unsigned int textureID;
    glGenTextures(1, &textureID);

    stbi_set_flip_vertically_on_load(true);
    int width, height, nrComponents;
    unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else if (nrComponents == 4)
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_image_free(data);
    }

    else
    {
        std::cout << "Texture failed to load at path: " << texture.path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}
