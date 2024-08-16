#ifndef RESOURCEMANAGER_H
#define RESOURCEMANAGER_H


#include <string>
#include <vector>

namespace Graphics {


enum VAO_TYPE {
	NONE,
	STANDARD
};




struct RenderEntity {
	VAO_TYPE vaoType;
	std::string shaderName;
	unsigned int VAO;
	unsigned int VBO; // TODO: make this a vector and automatically push a new vbo when/if ever full and do multiple draw calls (maybe i could just have it be a vector by default and set up multidrawarrays that way? ill find out later
	unsigned int EBO; // Same thing as VBO, also maybe give an index per buffer
	// Graphics::Shader& shader;
	// TODO: Add texture vector, up to 16/32 with texture units and what 
	// sounds like 64 with texture arrays
	// Add a material reference and replace shader
	
       // I could have a property that counts the amount of textures loaded in the texture array
       // and also the resource manager keeps track of a mesh texture index that is 
       // incremented every time a mesh is loaded and then reset 
       // when a render entity reaches its limit 
};


struct Resource {
};


class ResourceManager {
	public:
	// unsigned int getVAO(unsigned int (*func)());

		Graphics::RenderEntity& getRenderEntity(VAO_TYPE type);
		Graphics::RenderEntity& generateRenderEntity(VAO_TYPE type);
                //void addModelFile(std::string filePath);
		//void removeModelFile(std::string filePath);
		

	private:
	        std::vector<Resource> VAOs;
		std::vector<Resource> VBOs;
		//std::vector<Graphics::Model> Models;
		//std::vector<std::string> ModelFiles;
	        std::vector<Graphics::RenderEntity> RenderEntities;
	        VAO_TYPE CurrentVao;
};


}





#endif // !RESOURCEMANAGER_H
