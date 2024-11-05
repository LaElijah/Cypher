#include <stdexcept>
#include <iostream>
#include "FileReader.h"
#include "ResourceManager.h"
#include "Shader.h"
#include "Model.h"



std::map<std::string, Graphics::ShaderInfo>& Graphics::ResourceManager::getShaderInfo()
{
    return m_LoadedShaderInfo;
}

void Graphics::ResourceManager::loadModelInfo(std::string modelDirectory)
{
    Graphics::FileReader fileReader(modelDirectory);
    std::vector<std::string> models = fileReader.getFiles();
    std::string workingModel;
    
    for (std::string model : models)
    {
        Graphics::ModelInfo modelInfo;

	workingModel = modelDirectory + "/" + model;
	fileReader.setDirname(workingModel);
	std::vector<std::string> modelInfoFound = fileReader.getFiles("gltf");

	for (std::string filename : modelInfoFound)
	{
            auto [name, extension] = FileReader::splitFileExtension(filename);
	    modelInfo.name = name;
	    modelInfo.extension = extension;
	    modelInfo.path = std::string(workingModel + "/" + filename);

	    m_ModelInfo.push_back(modelInfo); 
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

void Graphics::ResourceManager::loadModel(const char* path)
{
    m_LoadedModels.push_back(std::shared_ptr<Graphics::Model>(new Graphics::Model(path))); 
}


void Graphics::ResourceManager::loadModel(std::string& path)
{
    m_LoadedModels.push_back(std::make_shared<Graphics::Model>(path)); 
    //m_LoadedModels.push_back(std::shared_ptr<Graphics::Model>(new Graphics::Model(path))); 
}


std::vector<std::shared_ptr<Graphics::Model>>& Graphics::ResourceManager::getLoadedModels()
{
    return m_LoadedModels;
}

void Graphics::ResourceManager::initialize()
{
    loadModelInfo(); 
    loadShaderInfo(true, "./data/Shaders/debug");  
}



