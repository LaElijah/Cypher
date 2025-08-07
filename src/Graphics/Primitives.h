#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map> 
#include <bitset> 

const unsigned int MAX_COMPONENT_SIZE = 256;


namespace Graphics
{



    typedef std::bitset<MAX_COMPONENT_SIZE> OBJECT_ID;

    typedef unsigned int Entity;

    struct ModelInfo
    {
        std::string path;
        std::string name;
        std::string extension;
    };

    struct TextureInfo
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
        Mesh(
            std::vector<Graphics::Vertex> vertices,
            std::vector<unsigned int> indices,
            std::vector<TextureInfo> textureInfo,
            std::string shader) : vertices(vertices),
                                  indices(indices),
                                  textureInfo(textureInfo),
                                  shader(shader)
        {
        }

        std::vector<Vertex> vertices;
        std::vector<unsigned int> indices;
        std::vector<TextureInfo> textureInfo;
        std::string shader;
    };

    struct Model
    {
        Model() {};

        Model(
            std::vector<Graphics::Mesh> &meshes,
            Graphics::ModelInfo &info) : meshes(meshes), info(info)
        {
        }

        std::vector<Graphics::Mesh> meshes;
        Graphics::ModelInfo info;
    };

    struct RenderBatch
    {
        std::vector<Graphics::Vertex> vertexData;
        std::vector<unsigned int> indexData;
        std::vector<unsigned int> counts;
        std::vector<unsigned int> indexCounts;
        std::map<std::string, std::vector<Graphics::TextureInfo>> textureInfo;
        std::string shader;

        void reset()
        {
            vertexData.clear();
            indexData.clear();
            counts.clear();
            indexCounts.clear();
            textureInfo.clear();

        }
    };

        struct Component
    {
    };

    struct RenderComponent : public Component
    {
        RenderComponent() {};

        RenderComponent(Graphics::Model model) : model(model) {};
        Graphics::Model model;
    };


}

#endif
