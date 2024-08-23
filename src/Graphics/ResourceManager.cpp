
#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "ResourceManager.h"
#include <stdexcept>


#include <algorithm>
#include <iostream>
#include "FileReader.h"
#include "Model.h"
#include "MeshTypes.h"



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

    std::cout << "VS: " + vs << std::endl;
    std::cout << "FS: " + fs << std::endl;
	loadShader(new Graphics::Shader(vs, fs), shader);
    }
}

Graphics::RenderResource& Graphics::ResourceManager::generateRenderResource(VAO_TYPE vaoType)
{

	Graphics::RenderResource resource;
    resource.vaoType = vaoType;
    
   // TODO: instead of passing in the shader, set up a getShader function to get reference 
    //entity.shader = shader;


    glGenVertexArrays(1, &resource.VAO); 
    glBindVertexArray(resource.VAO);

    glGenBuffers(1, &resource.VBO);
    glGenBuffers(1, &resource.EBO); 

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

Graphics::RenderResource& Graphics::ResourceManager::getRenderResource(VAO_TYPE vaoType)
{
/*	
	auto entity = std::find_if(
		  RenderEntities.begin(), 
	          RenderEntities.end(), 
		  [type] 
		  ( const Graphics::RenderResources& entity )
		  { return entity.vaoType == type; });
 */   
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



void Graphics::ResourceManager::generateVAO(VAO_TYPE vaoType)
{

    
   // TODO: instead of passing in the shader, set up a getShader function to get reference 
    //entity.shader = shader;
    if (!VAOs.count(vaoType))
    {
        unsigned int VAO;
        glGenVertexArrays(1, &VAO); 
        glBindVertexArray(VAO);

	    std::cout << "UNCACHED VAO" << std::endl;
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
         
	VAOs[vaoType] = std::move(VAO);

	    std::cout << "VAO CACHED" << std::endl;
    }
}

unsigned int& Graphics::ResourceManager::getVAO(VAO_TYPE vaoType)
{
	if (CurrentVao != vaoType)
	{
            generateVAO(vaoType);	
	}
            CurrentVao = vaoType;

            return VAOs[vaoType]; 	
}


void Graphics::ResourceManager::generateVBO(VAO_TYPE vaoType)
{

    
   // TODO: instead of passing in the shader, set up a getShader function to get reference 
    //entity.shader = shader;
    if (!VBOs.count(vaoType))
    {
        unsigned int VBO;
        glGenBuffers(1, &VBO); 
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
 
         
	VBOs[vaoType] = std::move(VBO);
    }
}

unsigned int& Graphics::ResourceManager::getVBO(VAO_TYPE vaoType)
{
        generateVBO(vaoType);	
        return VBOs[vaoType]; 	
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

void Graphics::ResourceManager::generateEBO(VAO_TYPE vaoType)
{

    
   // TODO: instead of passing in the shader, set up a getShader function to get reference 
    //entity.shader = shader;
    if (!EBOs.count(vaoType))
    {
        unsigned int EBO;
        glGenBuffers(1, &EBO); 
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
 
         
	EBOs[vaoType] = EBO;
    }
}



unsigned int& Graphics::ResourceManager::getEBO(VAO_TYPE vaoType)
{
	if (CurrentVao != vaoType)
	{
            generateEBO(vaoType);	
	}

            return EBOs[vaoType]; 	
}

/*

Graphics::Shader& Graphics::ResourceManager::getShader(SHADER_NAME shaderName)
{
	if (CurrentShader != shaderName)
	{
            loadShader(shaderName);	
	}

            return shaders[shaderName]; 	
}


void Graphics::ResourceManager::loadShader(SHADER_NAME shaderName)
{
    shaders[shaderName] = std::move(Graphics::Shader debugShader(
        "/home/laelijah/Gengine/data/Shaders/DEBUG/debug.vs",
        "/home/laelijah/Gengine/data/Shaders/DEBUG/debug.fs"));



   }
*/
