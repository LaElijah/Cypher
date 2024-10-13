#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <iostream>
#include "MeshTypes.h"
#include "Model.h"
#include "Shader.h"

#include "../../external/imgui/imgui.h"

namespace Graphics { 
    class ResourceManager {
    	public:
            void initialize(); 
	    void loadModelPaths(std::string modelDirectory = "./data/Models");
            void loadModel(Graphics::Model* model);
            std::vector<Graphics::Model*>& getLoadedModels();
	    std::map<std::string, Graphics::ShaderInfo>& getShaderInfo();

	    void loadShaderInfo(std::string directory = "./data/Shaders", bool singleFolder = true); 

	private:
	    std::map<std::string, Graphics::Texture> loadedTextures; 
	    std::map<std::string, Graphics::ShaderInfo> m_LoadedShaderInfo;
	    std::vector<Graphics::Model*> loadedModels;
    	    std::vector<Graphics::ModelFile> ModelFiles;
    };


}

#endif // !RESOURCEMANAGER_H
