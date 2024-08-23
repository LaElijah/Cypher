#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <iostream>
#include "Model.h"
#include "MeshTypes.h"

namespace Graphics {



    struct RenderResource {
    	VAO_TYPE vaoType;
    	std::string shaderName;
    	unsigned int VAO;
    	unsigned int VBO; // TODO: make this a vector and automatically push a new vbo when/if ever full and do multiple draw calls (maybe i could just have it be a vector by default and set up multidrawarrays that way? ill find out later
    	unsigned int EBO; // Same thing as VBO, also maybe give an index per buffer
    	// Graphics::Shader& shader;
    	// TODO: Add texture vector, up to 16/32 with texture units and what 
    	// sounds like 1024 with texture arrays
    	// Add a material reference and replace shader
    	
           // I could have a property that counts the amount of textures loaded in the texture array
           // and also the resource manager keeps track of a mesh texture index that is 
           // incremented every time a mesh is loaded and then reset 
           // when a render entity reaches its limit 
    };
    
    
    struct Resource {
	    unsigned int v;
    };
    
    
    class ResourceManager {
    	public:
            void loadModelPaths(std::string modelDirectory = "./data/Models");
            Graphics::RenderResource& getRenderResource(VAO_TYPE type);
            Graphics::RenderResource& generateRenderResource(VAO_TYPE type);
       	  
            std::vector<Graphics::Model*>& getLoadedModels();

	    Graphics::Shader* getShader(std::string); 
	    void loadShader(Graphics::Shader* shader, std::string name); 
	    void loadShaders(std::string shaderDirectory = "./data/Shaders"); 


            void loadModel(Graphics::Model* model);
	    void generateVAO(VAO_TYPE vaoType); 
	    unsigned int& getVAO(VAO_TYPE vaoType);

    	    void generateVBO(VAO_TYPE vaoType);
	    unsigned int& getVBO(VAO_TYPE vaoType);
	    
	    void generateEBO(VAO_TYPE vaoType);
	    unsigned int& getEBO(VAO_TYPE vaoType);
	


       
	private:
            std::map<VAO_TYPE, unsigned int> VAOs;
    	    std::map<VAO_TYPE, unsigned int> VBOs;
    	    std::map<VAO_TYPE, unsigned int> EBOs;
	    std::map<std::string, Graphics::Texture> loadedTextures;
	    std::vector<Graphics::Model*> loadedModels;
	    std::map<std::string, Graphics::Shader*> loadedShaders;
    	    std::vector<Graphics::ModelFile> ModelFiles;
    	    std::map<VAO_TYPE, Graphics::RenderResource> RenderResources;
    	    VAO_TYPE CurrentVao = NONE;
	    std::string CurrentShader;
    };


}





#endif // !RESOURCEMANAGER_H
