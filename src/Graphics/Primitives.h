#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <iostream>

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
        std::string DEFAULT_SHADER = "debug";
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
        std::vector<glm::mat4> transforms;
        std::vector<Graphics::Vertex> vertexData;
        std::vector<unsigned int> indexData;
        std::vector<unsigned int> counts;
        std::vector<unsigned int> indexCounts;
        std::map<std::string, std::vector<Graphics::TextureInfo>> textureInfo;
        std::string shader = "debug";

        bool isChanged()
        {
            return CHANGED;
        }
        void reset()
        {
            vertexData.clear();
            indexData.clear();
            counts.clear();
            indexCounts.clear();
            textureInfo.clear();
            transforms.clear();
        }

        void finish()
        {

            CHANGED = false;
        }

        void alertChange()
        {
            std::cout << "ALERT" << std::endl;
            CHANGED = true;
        }

        void insert
        (
            Graphics::Mesh &mesh,
            glm::mat4& transform
        )
        {
            std::cout << mesh.textureInfo.back().directory << std::endl;
            std::cout << "TEXTURE PATH: " << mesh.textureInfo.back().path << std::endl;
            vertexData.insert(
                vertexData.end(),
                mesh.vertices.begin(),
                mesh.vertices.end());

            indexData.insert(
                indexData.end(),
                mesh.indices.begin(),
                mesh.indices.end());

            counts.push_back(mesh.vertices.size());
            indexCounts.push_back(mesh.indices.size());

            textureInfo["diffuse"]
                .insert(
                    textureInfo["diffuse"].end(),
                    mesh.textureInfo.begin(),
                    mesh.textureInfo.end());

                    std::cout << "BATCH TEXTURE SIZE: " << textureInfo.at("diffuse").size() << std::endl;


            transforms.push_back(transform);

            std::cout << "BATCH MESH SHADER: " << mesh.shader << std::endl;
            shader = mesh.shader;
            std::cout << "BATCH SHADER AFTER UPDATE: " << shader << std::endl;
        }

    private:
        bool CHANGED = true;
    };

    struct Component
    {

    };

    struct Renderable : public Component
    {
        Renderable
        (
            std::string path
        )
        {
            this->path = path;
        }

        std::string path;
    };

    struct Transform : public Component
    {
        Transform(glm::vec3 pos)
        {
            position = pos;
        }

        glm::vec3 position;
        glm::mat4 localTransform = glm::mat4(1.0f);
        bool isDirty = true;
    };

    struct RenderComponent : public Component
    {
        RenderComponent() {};

        RenderComponent(Graphics::Model model) : model(model) {};
        Graphics::Model model;
    };

}

#endif
