#ifndef PRIMITIVES_H
#define PRIMITIVES_H

#include <glm/glm.hpp>
#include <string>
#include <vector>
#include <map>
#include <bitset>
#include <iostream>
#include <memory> 

#include <functional>
#include "Buffer.h" 
#include "IconsFontAwesome5.h"

const unsigned int MAX_COMPONENT_SIZE = 256;

namespace Graphics
{

    typedef unsigned int MaterialID;
    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };
    typedef std::bitset<MAX_COMPONENT_SIZE> OBJECT_ID;

    typedef size_t Entity;


    struct ModelInfo
    {
        std::string path;
        std::string name;
        std::string extension;
        std::string DEFAULT_SHADER = "debug";
	int meshIndex = 0;
    };

    struct TextureInfo
    {
        std::string type;
        std::string path;
        std::string directory;
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


	std::vector<unsigned int> instanceCounts;
	std::vector<unsigned int> startingVertices;
	std::vector<unsigned int> startingIndices;


        std::vector<glm::mat4> transforms;
        std::vector<Graphics::Vertex> vertexData;
        std::vector<unsigned int> indexData;
        std::vector<unsigned int> counts;
        std::vector<unsigned int> indexCounts;
        std::map<std::string, std::vector<Graphics::TextureInfo>> textureInfo;
        std::unordered_map<std::string, std::pair<unsigned int, unsigned int>> transformMappings;
        unsigned int TRANSFORM_OFFSET = 0;
        unsigned int lastModelSize = 0;
        std::string recentModel;
        std::string shader = "debug";

        bool isChanged()
        {
            return CHANGED;
        }
        void reset()
        {
            counts.clear();
            indexCounts.clear();
            textureInfo.clear();
            transforms.clear();
            lastModelSize = 0;
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

        void insert(
            std::shared_ptr<Graphics::BufferRecord>vRecord,
            //std::shared_ptr<Graphics::BufferRecord<Graphics::Index>>iRecord,
            glm::mat4 &transform)
        {
            //std::cout << mesh.textureInfo.back().directory << std::endl;
            //std::cout << "TEXTURE PATH: " << mesh.textureInfo.back().path << std::endl;

	    counts.push_back(vRecord->size);
            //indexCounts.push_back(iRecord->size);

	    /*
            textureInfo["diffuse"]
                .insert(
                    textureInfo["diffuse"].end(),
                    mesh.textureInfo.begin(),
                    mesh.textureInfo.end());
*/

    
            transforms.push_back(transform);

            

            shader = "debug";
        }

    private:
        bool CHANGED = true;
    };

    struct Component
    {
    };

    struct Renderable : public Component
    {
        Renderable(
            std::string path,
            std::string name)
        {
            this->path = path;
            this->name = name;
        }

        std::string path;
        std::string name = "debug";
	int meshIndex = 0;
    };

    struct Index : public Component
    {};

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

    struct Material
    {
        Graphics::TextureInfo diffuseMap;
    };   

    typedef uint64_t TextureHandle;

    struct MaterialData
    {
        TextureHandle diffuseMap;
    };


// GOTO: 327 of renderAPI to change loading method in renderAPI
    struct RenderDrawData
    {
	// Draw Call data 
	unsigned int vertexCount = 0;
        unsigned int instanceCount = 0;
	unsigned int indexCount = 0;
	unsigned int startingIndex = 0;
	unsigned int startingVertex = 0;
	unsigned int baseInstance = 0;

	// Instance data
	std::vector<Graphics::Transform> transforms;
	std::vector<std::shared_ptr<Graphics::Material>> materials;
    };


        inline const std::unordered_map<std::string, const char*> IconMap = {
        // Documents
        {"pdf",  ICON_FA_FILE_PDF},
        {"doc",  ICON_FA_FILE_WORD},
        {"docx", ICON_FA_FILE_WORD},
        {"odt",  ICON_FA_FILE_WORD},
        {"xls",  ICON_FA_FILE_EXCEL},
        {"xlsx", ICON_FA_FILE_EXCEL},
        {"ods",  ICON_FA_FILE_EXCEL},
        {"csv",  ICON_FA_FILE_CSV},
        {"ppt",  ICON_FA_FILE_POWERPOINT},
        {"pptx", ICON_FA_FILE_POWERPOINT},
        {"odp",  ICON_FA_FILE_POWERPOINT},

        // Images
        {"jpg",  ICON_FA_FILE_IMAGE},
        {"jpeg", ICON_FA_FILE_IMAGE},
        {"png",  ICON_FA_FILE_IMAGE},
        {"gif",  ICON_FA_FILE_IMAGE},
        {"bmp",  ICON_FA_FILE_IMAGE},
        {"svg",  ICON_FA_FILE_IMAGE},
        {"webp", ICON_FA_FILE_IMAGE},
        {"ico",  ICON_FA_FILE_IMAGE},

        // Audio
        {"mp3",  ICON_FA_FILE_AUDIO},
        {"wav",  ICON_FA_FILE_AUDIO},
        {"ogg",  ICON_FA_FILE_AUDIO},
        {"flac", ICON_FA_FILE_AUDIO},
        {"m4a",  ICON_FA_FILE_AUDIO},

        // Video
        {"mp4",  ICON_FA_FILE_VIDEO},
        {"mkv",  ICON_FA_FILE_VIDEO},
        {"avi",  ICON_FA_FILE_VIDEO},
        {"mov",  ICON_FA_FILE_VIDEO},
        {"wmv",  ICON_FA_FILE_VIDEO},

        // Compressed / Archives
        {"zip",  ICON_FA_FILE_ARCHIVE},
        {"rar",  ICON_FA_FILE_ARCHIVE},
        {"7z",   ICON_FA_FILE_ARCHIVE},
        {"tar",  ICON_FA_FILE_ARCHIVE},
        {"gz",   ICON_FA_FILE_ARCHIVE},

        // Code & Data Syntaxes
        {"c",    ICON_FA_FILE_CODE},
        {"cpp",  ICON_FA_FILE_CODE},
        {"h",    ICON_FA_FILE_CODE},
        {"hpp",  ICON_FA_FILE_CODE},
        {"cs",   ICON_FA_FILE_CODE},
        {"java", ICON_FA_FILE_CODE},
        {"py",   ICON_FA_FILE_CODE},
        {"js",   ICON_FA_FILE_CODE},
        {"ts",   ICON_FA_FILE_CODE},
        {"go",   ICON_FA_FILE_CODE},
        {"rs",   ICON_FA_FILE_CODE},
        {"html", ICON_FA_FILE_CODE},
        {"css",  ICON_FA_FILE_CODE},
        {"json", ICON_FA_FILE_CODE},
        {"xml",  ICON_FA_FILE_CODE},
        {"sh",   ICON_FA_FILE_CODE},
        {"bat",  ICON_FA_FILE_CODE}
    };

}


#endif
