#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
#include "MeshTypes.h"
#include "FileReader.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Model.h"

#include "../../external/imgui/imgui.h"


std::map<std::string, Graphics::ShaderInfo>& Graphics::ResourceManager::getShaderInfo()
{
    return m_LoadedShaderInfo;
}

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



// TODO: Create a function of this 
// but for a single given path 
void Graphics::ResourceManager::loadShaderInfo(std::string directory, bool singleFolder)
{

    Graphics::FileReader fileReader(directory);
    std::string vs, fs;

    if (singleFolder)
    {
        vs = directory + "/" + fileReader.getFile("vs");
        fs = directory + "/" + fileReader.getFile("fs");

	// TODO: CHANGE THIS TO DYNAMICALLY GET SHADER NAME 
	// FROM DIRECTORY
	std::string shader = "debug";

        m_LoadedShaderInfo.emplace(shader, Graphics::ShaderInfo(shader, vs, fs));

    }
    else
    {
        std::vector<std::string> shaders = fileReader.getFiles();
        std::string workingShader;
 
        for (std::string shader : shaders)
        {
            workingShader = directory + "/" + shader;
            fileReader.setDirname(workingShader);

            std::string vs = workingShader + "/" + fileReader.getFile("vs");
            std::string fs = workingShader + "/" + fileReader.getFile("fs");

            m_LoadedShaderInfo.emplace(shader, Graphics::ShaderInfo(shader, vs, fs));
        }
    }
}

void Graphics::ResourceManager::loadModel(Graphics::Model* model)
{
    loadedModels.push_back(model); 
}





std::vector<Graphics::Model*>& Graphics::ResourceManager::getLoadedModels()
{
    return loadedModels;
}

/*

	   // Move implememntation to api 
void Graphics::ResourceManager::bindBuffers(
		std::vector<Graphics::Vertex>& vertices, 
		std::vector<unsigned int>& indices, 
		Graphics::VAO_TYPE vaoType,
		std::string shaderName)
{
    Graphics::RenderResource& resource = getRenderResource(vaoType, shaderName);

    if (CurrentVao != vaoType)
    {

        glBindVertexArray(resource.VAO);
        glBindBuffer(GL_ARRAY_BUFFER, resource.VBO);
    }
     
    glBufferData(GL_ARRAY_BUFFER, sizeof(Graphics::Vertex) * vertices.size(), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, resource.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_STATIC_DRAW);
    
}
*/

void Graphics::ResourceManager::initialize()
{
    // TODO: Rename to ModelInfo    
    loadModelPaths(); 
    loadShaderInfo("./data/Shaders", false);  
}



