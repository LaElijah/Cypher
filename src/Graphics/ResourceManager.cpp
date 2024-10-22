#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <iostream>
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
            auto [name, extension] = FileReader::splitFileExtension(filename);
	    modelFile.name = name;
	    modelFile.extension = extension;
	    modelFile.path = std::string(workingModel + "/" + filename);

	    ModelFiles.push_back(modelFile); 
	}
    }
}



// TODO: Create a function of this 
// but for a single given path 
void Graphics::ResourceManager::loadShaderInfo(bool single, std::string directory)
{

    Graphics::FileReader fileReader(directory);
    std::string vs, fs;

    if (single)
    {
        std::string shader = Graphics::FileReader::getNameFromDirectory(directory); 
	std::cout << "SHADER FOUND: " << shader << std::endl;

    	vs = directory + "/" + fileReader.getFile("vs");
        fs = directory + "/" + fileReader.getFile("fs");

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

void Graphics::ResourceManager::initialize()
{
    // TODO: Rename to ModelInfo    
    loadModelPaths(); 
    //loadShaderInfo();  
    loadShaderInfo(true, "./data/Shaders/debug");  
}



