#ifndef MODEL_H
#define MODEL_H


#include "Mesh.h"
#include "Globals.cpp"
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <vector> 


namespace Graphics {

  class Model 
  {
    public:
      Model(std::string path, Graphics::ResourceManager& resourceManager);

      void Draw(Graphics::Shader& shader, Graphics::ResourceManager& resourceManager);

      private:
      
      std::vector<Graphics::Mesh> meshes;
      std::vector<Graphics::Texture> texturesLoaded;
      std::string directory;

      void loadModel(std::string path, Graphics::ResourceManager& resourceManager);

      void processNode(aiNode *node, const aiScene *scene, Graphics::ResourceManager& resourceManager);
      Graphics::Mesh processMesh(aiMesh *mesh, const aiScene *scene, Graphics::ResourceManager& resourceManager);

      std::vector<Graphics::Texture> loadMaterialTextures(aiMaterial *mat, const aiScene* scene, aiTextureType type, std::string typeName);

      unsigned int TextureFromFile(std::string path, std::string directory);
  
      unsigned int TextureFromEmbedded(const aiScene* scene, int embeddedIndex);

  };
}

#endif
