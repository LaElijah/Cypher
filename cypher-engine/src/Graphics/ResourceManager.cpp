#include <stdexcept>
#include <iostream>
#include "FileReader.h"
#include "ResourceManager.h"
#include "Shader.h"



std::map<std::string, Graphics::ShaderInfo>& Graphics::ResourceManager::getShaderInfo()
{
    return m_LoadedShaderInfo;
}

void Graphics::ResourceManager::loadModelInfo(std::string modelDirectory)
{
    std::cout << "Resource Manager modelDirectory: " << modelDirectory << std::endl;
    // Get full paths of files in this directory 
    std::vector<std::string> models = Graphics::FileReader::getFiles(modelDirectory);
    
    for (std::string model : models)
    {
        Graphics::ModelInfo modelInfo;

	// Search for main model file, support OBJ, GLTF, FBX
	std::vector<std::string> modelFiles = Graphics::FileReader::getFiles(model, {"gltf"});

	for (std::string filename : modelFiles)
	{
            std::pair<std::string, std::string> fileInfo = Graphics::FileReader::getFileExtension(Graphics::FileReader::getFileName(filename));


	    modelInfo.name = fileInfo.first;
	    modelInfo.extension = fileInfo.second;
	    modelInfo.path = filename;

	    m_ModelInfo.push_back(modelInfo); 
	}
    }
}



// TODO: Create a function of this 
// but for a single given path 
void Graphics::ResourceManager::loadShaderInfo(bool single, std::string directory)
{
    std::string vs, fs;

	std::vector<std::string> shaderFiles;
    if (single)
    {
        std::string shader = Graphics::FileReader::getFileName(directory); 
	std::cout << "SHADER FOUND: " << shader << std::endl;



	for (const auto& [extension, value] : Graphics::ShaderInfo::extensionEnums)
	{

		auto search = Graphics::FileReader::getFile(directory, {extension});


		std::cout << "-EXTENSION CHECK: " << extension << std::endl;
		std::cout << "-SEARCH CHECK: " << search.second << std::endl;
		switch (value)
		{
			case Graphics::SHADER_FILE_TYPE::VERTEX:
			case Graphics::SHADER_FILE_TYPE::FRAGMENT:


			if (!search.first)
			    throw std::invalid_argument("MISSING CRITICAL SHADER FILE, MISSING: " + extension);
			else
			{
			   shaderFiles.push_back(search.second); 
			   break;
			}


			default:
			    if (search.first)
			    {
                                std::cout << "+Extension: " << extension << "\n+Path: " << search.second << std::endl;
			        shaderFiles.push_back(search.second); 
			    } 
			    break;
		}
	}
	std::cout << "CREATING SHADER INFO" << std::endl;


	for (std::string& s : shaderFiles)
	{
            std::cout << s << std::endl;
	}
       m_LoadedShaderInfo.emplace(shader, Graphics::ShaderInfo(shader, shaderFiles));
    }
    else
    {
        std::vector<std::string> shaders = Graphics::FileReader::getFiles(directory);
 
        for (std::string shader : shaders)
        {

            m_LoadedShaderInfo.emplace(shader, Graphics::ShaderInfo(shader, shaderFiles));
	    shaderFiles.clear();
        }
    }
}

/*
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

*/


void Graphics::ResourceManager::initialize()
{
    std::cout << "RESOURCE MANAGER: " << "ONLY LOADING DEBUG IN INIT" << std::endl;
    loadModelInfo(); 
    loadShaderInfo(true, "./data/Shaders/debug");  
}



