//#include <glad/glad.h>

#include "../../external/GLAD/glad.h"
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include "MeshTypes.h"
#include "FileReader.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Model.h"
#include "RenderBatch.h"

#include "../../external/imgui/imgui.h"



void Graphics::ResourceManager::loadModelPaths(std::string modelDirectory)
{
    Graphics::FileReader fileReader(modelDirectory);
    std::vector<std::string> models = fileReader.getFiles();
    std::string workingModel;
    
    for (std::string model : models)
    {
        Graphics::ModelFile modelFile;

	workingModel = modelDirectory + "/" + model;
	fileReader.setDirname(workingModel);
	std::vector<std::string> modelFiles = fileReader.getFiles("gltf");

	for (std::string filename : modelFiles)
	{
            auto [name, extension] = FileReader::splitFilename(filename);
	    modelFile.name = name;
	    modelFile.extension = extension;
	    modelFile.path = std::string(workingModel + "/" + filename);

	    ModelFiles.push_back(modelFile); 
	}
    }
}




void Graphics::ResourceManager::loadShaders(std::string shaderDirectory)
{
    Graphics::FileReader fileReader(shaderDirectory);
    std::vector<std::string> shaders = fileReader.getFiles();
    std::string workingShader;
 
    for (std::string shader : shaders)
    {
        workingShader = shaderDirectory + "/" + shader;
	fileReader.setDirname(workingShader);

	std::string vs = workingShader + "/" + fileReader.getFile("vs");
	std::string fs = workingShader + "/" + fileReader.getFile("fs");

	loadShader(new Graphics::Shader(vs, fs, shader), shader);
    }
}




Graphics::RenderResource& Graphics::ResourceManager::generateRenderResource(VAO_TYPE vaoType)
{
    Graphics::RenderResource resource;
    
    resource.vaoType = vaoType;
    
    glGenVertexArrays(1, &resource.VAO); 
    glBindVertexArray(resource.VAO);

    glGenBuffers(1, &resource.VBO);
    glGenBuffers(1, &resource.EBO); 

    glGenBuffers(1, &resource.IBO); 

    glBindBuffer(GL_ARRAY_BUFFER, resource.VBO);

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

	 
    RenderResources[vaoType] = std::move(resource);

    return getRenderResource(vaoType);
}


void Graphics::ResourceManager::loadTextures(std::vector<Graphics::Texture>& textures)
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




Graphics::RenderResource& Graphics::ResourceManager::getRenderResource(VAO_TYPE vaoType)
{
    // Was the vao found in our vector?
    if (RenderResources.count(vaoType)) 
    {
        // Return the found render entitiy
	return RenderResources[vaoType];
    }
    else 
    {
	std::cout << "GENERATING NEW RENDER ENTITY" << std::endl;
        return generateRenderResource(vaoType);
    }
}




void Graphics::ResourceManager::loadModel(Graphics::Model* model)
{
    loadedModels.push_back(model); 
}




void Graphics::ResourceManager::loadShader(Graphics::Shader* shader, std::string name)
{
    loadedShaders[name] = std::move(shader);
}




Graphics::Shader* Graphics::ResourceManager::getShader(std::string name)
{
    return loadedShaders[name];
}




std::vector<Graphics::Model*>& Graphics::ResourceManager::getLoadedModels()
{
    return loadedModels;
}



void Graphics::ResourceManager::bindBuffers(std::vector<Graphics::Vertex>& vertices, std::vector<unsigned int>& indices, std::vector<Graphics::GLDrawElementsIndirectCommand> drawCalls, Graphics::VAO_TYPE vaoType)
{
    Graphics::RenderResource& resource = getRenderResource(DEBUG);

    if (CurrentVao != vaoType)
    {
        //std::cout << "VAO: " << resource.VAO << std::endl;
        glBindVertexArray(resource.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, resource.VBO);
    
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resource.EBO);
        glBindBuffer(GL_DRAW_INDIRECT_BUFFER, resource.IBO);
    }
   
    glBufferData(GL_ARRAY_BUFFER, sizeof(Graphics::Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
  
    glBufferData(GL_DRAW_INDIRECT_BUFFER, drawCalls.size() * sizeof(Graphics::GLDrawElementsIndirectCommand), drawCalls.data(), GL_STATIC_DRAW); 

  
 
}


void Graphics::ResourceManager::initialize()
{
    loadModelPaths(); 
    loadShaders();  
}



