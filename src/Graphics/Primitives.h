#ifndef PRIMITIVES_H
#define PRIMITIVES_H


#include <glm/glm.hpp>
#include <string>
#include <vector>

namespace Graphics 
{
    typedef unsigned int Entity;




    struct ModelInfo
    {
        std::string path;
        std::string name;
        std::string extension;
    };




    struct Texture
    {
      std::string type;
      std::string path;
      std::string directory;
    };
 



    struct Vertex
    {
      glm::vec3 Position;
      glm::vec3 Normal;
      glm::vec2 TexCoords;
    };




    struct Mesh 
    {
        Mesh
        (
	    std::vector<Graphics::Vertex> vertices, 
            std::vector<unsigned int> indices, 
            std::vector<Texture> textures,
	    std::string shader
	) : 
	    vertices(vertices), 
            indices(indices), 
	    textures(textures), 
	    shader(shader)
	    {}

        std::vector<Vertex> vertices;   
        std::vector<unsigned int> indices;
        std::vector<Texture> textures;
	std::string shader;
    };




    struct Model 
    {
	Model() {};

	Model
	(
            std::vector<Graphics::Mesh>& meshes,
	    Graphics::ModelInfo& info
	) : meshes(meshes), info(info)
	{}

        std::vector<Graphics::Mesh> meshes;
	Graphics::ModelInfo info;
    };


    struct RenderBatch
    {
        std::vector<Graphics::Vertex> vertexData;
	std::vector<unsigned int> indexData;
	std::vector<unsigned int> counts;
	std::vector<unsigned int> indexCounts;
	std::vector<std::pair<unsigned int, unsigned int>> textureCells;
    	std::string shader;
    };
  
}





#endif
