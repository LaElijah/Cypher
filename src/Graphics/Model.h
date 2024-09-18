#ifndef MODEL_H
#define MODEL_H

#include "Mesh.h"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <vector> 


namespace Graphics {

    class Model 
    {
        public:
            Model(std::string path);
            
            std::vector<Graphics::Mesh>& getMeshes();

	    // In transform system 
            glm::mat4 getModelMatrix();

            std::string getShaderName();
            void setShaderName(std::string name);

        private:
	    // Transform component
            glm::vec3 Position = glm::vec3(25.0f, 0.0f, -60.0f);
            glm::vec3 Scale = glm::vec3(10.0f, 10.0f, 10.0f);
            
            std::vector<Graphics::Mesh> meshes;
            std::vector<Graphics::Texture> texturesLoaded;

	    // Make default to mesh component 
            std::string shaderName = "DEBUG_VAO_DEBUG_SHADER";
            std::string directory;

            void loadModel(std::string path);
            void processNode(
			    aiNode *node, 
			    const aiScene *scene, 
			    const glm::mat4& parentTransform);

            Graphics::Mesh processMesh(
			    aiMesh *mesh, 
			    const aiScene *scene, 
			    const glm::mat4& transform);

            std::vector<Graphics::Texture> loadMaterialTextures(
			    aiMaterial *mat, 
			    const aiScene* scene, 
			    aiTextureType type, 
			    std::string typeName);

            unsigned int TextureFromFile(std::string path, std::string directory); 
            unsigned int TextureFromEmbedded(const aiTexture* embeddedTexture);
    };
}

#endif
