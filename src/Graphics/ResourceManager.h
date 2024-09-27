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

	    Graphics::Shader* getShader(std::string); 
	    void loadShader(Graphics::Shader* shader, std::string name); 
	    void loadShaders(std::string shaderDirectory = "./data/Shaders"); 

	private:
	    std::map<std::string, Graphics::Texture> loadedTextures; 
	    std::map<std::string, Graphics::Shader*> loadedShaders;
	    std::vector<Graphics::Model*> loadedModels;
    	    std::vector<Graphics::ModelFile> ModelFiles;
    };


}

#endif // !RESOURCEMANAGER_H
