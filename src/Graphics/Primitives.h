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
 
const unsigned int MAX_COMPONENT_SIZE = 256;

namespace Graphics
{

    struct Vertex
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 TexCoords;
    };


    typedef std::bitset<MAX_COMPONENT_SIZE> OBJECT_ID;

    typedef size_t Entity;

struct BufferRecord
{
    BufferRecord
    (
        int size = 0,
        std::function<void(BufferRecord)> callback = nullptr,
        int start = 0,
	std::string owner = ""
    )
    : size(size),
      cleanup(callback),
      start(start),
      owner(owner)
    {
        split = 0;
 
        if (size == 0)
            active = false;
    }
 
    int start;
    int size;
    int split;
    bool active = true;
    std::string owner;
 
    std::function<void(BufferRecord)> cleanup;
 
    ~BufferRecord()
    {
        if (cleanup && active)
        {
            active = false;
            cleanup(BufferRecord(size, nullptr, start));
        }
    }
 
    std::string toString()
    {
        return "START: " + std::to_string(start)
             + " | SIZE: "   + std::to_string(size)
             + " | SPLIT: "  + std::to_string(split)
             + " | ACTIVE: " + std::to_string(active);
    }
};
 
	    
 
class Buffer
{
    public:
        Buffer(int size, std::string label) : size(size), name(label)
        {
            tail = 0;
            this->cleanup = [this](auto deleted)
            {
                for (int i = records.size() - 1; i >= 0; i--)
                {
                    if (records[i].expired())
		    {
                        freeSpaces.push_back
			(
                            std::make_shared<BufferRecord>(deleted.size, nullptr, deleted.start, name)
			);

                        records.erase(records.begin() + i);
			break;
		    }
                }
            };
        }
 

	void clear()
	{

	    records.clear();
	    freeSpaces.clear();
	}

        std::shared_ptr<BufferRecord> allocate(int byteSize)
        {
            if (byteSize > size)
                return nullptr;
 
            // Fresh buffer
            if (freeSpaces.empty() && records.empty())
            {
                auto record = std::make_shared<BufferRecord>(byteSize, cleanup, tail, name);
                records.push_back(record);   // store as weak_ptr
                tail += byteSize;
                return record;
            }
 
            // Try to fit into an existing free space
            for (int i = 0; i < freeSpaces.size(); i++)
            {
                if (freeSpaces[i]->size >= byteSize)
                {
                    auto record = std::make_shared<BufferRecord>(
                        byteSize, cleanup, freeSpaces[i]->start, name);
 
                    freeSpaces[i]->size  -= byteSize;
                    freeSpaces[i]->start += byteSize;
                    ++freeSpaces[i]->split;
 
                    if (freeSpaces[i]->size == 0)
                        freeSpaces.erase(freeSpaces.begin() + i);
 
                    records.push_back(record);   // store as weak_ptr
                    return record;
                }
            }
 
            // Try the unallocated tail
            if ((size - tail) >= byteSize)
            {
                auto record = std::make_shared<BufferRecord>(byteSize, cleanup, tail, name);
                records.push_back(record);   // store as weak_ptr
                tail += byteSize;
                return record;
            }
 
            return nullptr;
        }
 
        bool free(std::shared_ptr<BufferRecord> record)
        {
            if (!record)
                return false;
 
            for (int i = 0; i < (int)records.size(); i++)
            {
                if (records[i].lock() == record)
                {
                    freeSpaces.push_back(
                        std::make_shared<BufferRecord>(record->size, nullptr, record->start, name));
                    records.erase(records.begin() + i);
                    return true;
                }
            }
            return false;
        }
 
        std::vector<std::weak_ptr<BufferRecord>> getRecords()
        {
            return records;
        }
 
        std::vector<std::shared_ptr<BufferRecord>> getFreeSpaces()
        {
            return freeSpaces;
        }
 
        std::function<void(BufferRecord)> cleanup;

    private:
        std::vector<std::weak_ptr<BufferRecord>>   records;     // non-owning
        std::vector<std::shared_ptr<BufferRecord>> freeSpaces;  // buffer-owned
 
        int size;
        int tail;
	std::string name;
};



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

// GOTO: 327 of renderAPI to change loading method in renderAPI
    struct RenderDrawData
    {
	// Draw Call data 
	unsigned int vertexCount;
        unsigned int instanceCount;
	unsigned int indexCount;
	unsigned int startingIndex;
	unsigned int startingVertex;
	unsigned int baseInstance;

	// Instance data
	std::vector<Graphics::Transform> transforms;
    };


}

#endif
