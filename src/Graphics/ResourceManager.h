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



    struct RenderResource {
    	VAO_TYPE vaoType;
    	std::string shaderName;
    	unsigned int VAO;
    	unsigned int VBO; 
    	unsigned int EBO;     
	// Graphics::Shader& shader;
    	// TODO: Add texture vector, up to 16/32 with texture units and what 
    	// sounds like 1024 with texture arrays
    	// Add a material reference    	
        // I could have a property that counts the amount of textures loaded in the texture array
        // and also the resource manager keeps track of a mesh texture index that is 
        // incremented every time a mesh is loaded and then reset 
        // when a render entity reaches its limit 
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
            void initialize();

            void loadModel(Graphics::Model* model);
	    void generateVAO(VAO_TYPE vaoType); 
	    unsigned int& getVAO(VAO_TYPE vaoType);

    	    void generateVBO(VAO_TYPE vaoType);
	    unsigned int& getVBO(VAO_TYPE vaoType);
	    
	    void generateEBO(VAO_TYPE vaoType);
	    unsigned int& getEBO(VAO_TYPE vaoType);
           
	     





            void bindBuffers(
			    std::vector<Graphics::Vertex>& vertices, 
			    std::vector<unsigned int>& indices, 
			    Graphics::VAO_TYPE vaoType);
	
            void loadTextures(std::vector<Texture>& Textures);


       
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
