#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <memory>
#include <iostream>
#include "Primitives.h"
#include "Shader.h"


namespace Graphics 
{
    /**
     * This class handles primitives and access to them 
     *
     * Planned Features: 
     * - Change the model pointer to shared_ptr
     *
     */	
    class ResourceManager 
    {
    	public:
            void initialize(); 
	    void loadModelInfo(std::string modelDirectory = "./data/Models");
	    void loadShaderInfo(bool single = false, std::string directory = "./data/Shaders"); 
            void loadModel(std::string& path);
            void loadModel(const char* path);
            std::vector<std::shared_ptr<Graphics::Model>>& getLoadedModels();
	    std::map<std::string, Graphics::ShaderInfo>& getShaderInfo();


	private:
	    std::map<std::string, Graphics::TextureInfo> loadedTextures; 
	    std::map<std::string, Graphics::ShaderInfo> m_LoadedShaderInfo;

    	    std::vector<Graphics::ModelInfo> m_ModelInfo;
	    
    };
}

#endif // !RESOURCEMANAGER_H
