#ifndef MODEL_LOADER_H
#define MODEL_LOADER_H

#include "../../external/GLAD/glad.h"
#include <string>
#include <iostream>
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <set>
#include <memory>
#include "../../external/STB_IMAGE/stb_image.h"
#include "Primitives.h"

namespace Graphics
{

    template <typename T>
    class ModelLoader
    {
    public:
        std::shared_ptr<Graphics::Model> load(Graphics::ModelInfo &info)
        {
            return static_cast<T *>(this)->loadImpl(info);
        }

    protected:
    };

    class AssimpImporter : public ModelLoader<AssimpImporter>
    {
    public:
        std::shared_ptr<Graphics::Model> loadImpl(Graphics::ModelInfo &info)
        {
            if (loadedModels.count(info.path) != 0)
                return loadedModels.at(info.path);

            std::cout << "LOADING MODEL AT: " <<info.path << std::endl;


            Assimp::Importer importer;
            const aiScene *scene = importer.ReadFile(
                info.path,
                aiProcess_Triangulate
                //|| aiProcess_PreTransformVertices
            );

            if (
                !scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
            {
                std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
                // return NULL;  // TODO: THROW ERROR
            }

            std::string directory = info.path.substr(0, info.path.find_last_of('/'));

            // Parent root node of model
            std::vector<Graphics::Mesh> meshes;

            processNode(scene->mRootNode, scene, glm::mat4(1.0f), meshes, directory, info.DEFAULT_SHADER);

            auto model = std::make_shared<Graphics::Model>(meshes, info);
            loadedModels.emplace(info.path, model);
            return model;
        }


    private:
        static inline std::unordered_map<std::string, std::shared_ptr<Graphics::Model>> loadedModels;
        // Diversify this function for each different
        // vertex type loading format

        void processNode(
            aiNode *node,
            const aiScene *scene,
            const glm::mat4 &parentTransform,
            std::vector<Graphics::Mesh> &meshes,
            std::string &directory,
            std::string& defaultShader)
        {

            // TODO:
            glm::mat4 nodeTransform = ConvertMatrixToGLMFormatt(node->mTransformation);
            glm::mat4 globalTransform = parentTransform * nodeTransform;


            // Pack child nodes ( meshes ) in this parent node
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {

                // Process mesh loads in the
                // primitive data of the given node
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                meshes.push_back(processMesh(mesh, scene, globalTransform, directory, defaultShader));
            }

            // Move onto the children of this node
            for (unsigned int i = 0; i < node->mNumChildren; i++)
            {
                processNode(node->mChildren[i], scene, globalTransform, meshes, directory, defaultShader);
            }


        }

        Graphics::Mesh processMesh(
            aiMesh *mesh,
            const aiScene *scene,
            const glm::mat4 &transform,
            std::string &directory,
            std::string &defaultShader)
        {

            std::vector<Graphics::Vertex> vertices;
            std::vector<unsigned int> indices;
            std::vector<Graphics::TextureInfo> textureInfo;

            for (unsigned int i = 0; i < mesh->mNumVertices; i++)
            {
                Graphics::Vertex vertex;
                glm::vec3 vector = glm::vec3(
                    transform * glm::vec4(
                                    mesh->mVertices[i].x,
                                    mesh->mVertices[i].y,
                                    mesh->mVertices[i].z,
                                    1.0f));

                vertex.Position = vector;

                if (mesh->HasNormals())
                {
                    vector.x = mesh->mNormals[i].x;
                    vector.y = mesh->mNormals[i].y;
                    vector.z = mesh->mNormals[i].z;
                    vertex.Normal = vector;
                }

                if (mesh->mTextureCoords[0])
                {
                    glm::vec2 vec;
                    vec.x = mesh->mTextureCoords[0][i].x;
                    vec.y = mesh->mTextureCoords[0][i].y;
                    vertex.TexCoords = vec;
                }
                else
                {
                    vertex.TexCoords = glm::vec2(0.0f, 0.0f);
                }

                vertices.push_back(vertex);
            }

            for (unsigned int i = 0; i < mesh->mNumFaces; i++)
            {
                aiFace face = mesh->mFaces[i];
                for (unsigned int j = 0; j < face.mNumIndices; j++)
                    indices.push_back(face.mIndices[j]);
            }

            aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];
            // TODO: FIX
            std::vector<Graphics::TextureInfo> diffuseMaps = getTextureInfo(
                material,
                scene,
                aiTextureType_DIFFUSE,
                "texture_diffuse",
                directory);

            textureInfo.insert(textureInfo.end(), diffuseMaps.begin(), diffuseMaps.end());
            /*


                  std::vector<Texture> specularMaps = loadMaterialTextures
                  (
                      material,
                      scene,
                      aiTextureType_SPECULAR,
                      "texture_specular"
                  );

                  textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
             */


            return Mesh(vertices, indices, textureInfo, defaultShader);
        }

        std::vector<Graphics::TextureInfo> getTextureInfo(
            aiMaterial *mat,
            const aiScene *scene,
            aiTextureType type,
            std::string typeName,
            std::string directory)
        {
            std::vector<Graphics::TextureInfo> textures;
            std::set<std::string> loadedTextures;
            
            for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
            {
                aiString str; // Texture path
                mat->GetTexture(type, i, &str);

                // Does this current mesh have any textures already loaded into memory for this model?
                if (loadedTextures.find(str.C_Str()) == loadedTextures.end())
                {
                    Graphics::TextureInfo texture;
                    texture.type = typeName;
                    texture.path = str.C_Str();
                    texture.directory = directory;
                    textures.push_back(texture);
                    loadedTextures.insert(texture.path);
                }
            }
            return textures;
        };

        /*
        // TODO: MAKE FUNCTIONAL
        unsigned int TextureFromEmbedded(const aiScene *scene, int embeddedIndex)
        {
            std::cout << "LOADING EMBEDDED TEXTURE" << std::endl;
            unsigned int textureID;
            glGenTextures(1, &textureID);

            const aiTexture *embeddedTexture = scene->mTextures[embeddedIndex];

            if (embeddedTexture->pcData)
            {
                // Update for a wider range of channels

                int width = embeddedTexture->mWidth;
                int height = embeddedTexture->mHeight;
                int format = 3;

                // Not yet working need to fnd a way to get format
                // embeddedTexture->mFormat == aiTextureFormat::aiTextureFormat_RGBA ? 4 : 3;

                unsigned char *data = new unsigned char[width * height * format];
                std::memcpy(data, embeddedTexture->pcData, width * height * format);

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                delete[] data;
            }

            else
            {
                std::cout << "Texture failed to load at embedded index: " << embeddedIndex << std::endl;
            }

            return textureID;
        }

        unsigned int TextureFromFile(std::string path, std::string directory)
        {
            std::string filename = std::string(path);
            filename = directory + '/' + filename;

            unsigned int textureID;
            glGenTextures(1, &textureID);

            stbi_set_flip_vertically_on_load(true);
            int width, height, nrComponents;
            unsigned char *data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

            if (data)
            {
                GLenum format;
                if (nrComponents == 1)
                    format = GL_RED;
                else if (nrComponents == 3)
                    format = GL_RGB;
                else if (nrComponents == 4)
                    format = GL_RGBA;

                glBindTexture(GL_TEXTURE_2D, textureID);
                glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
                glGenerateMipmap(GL_TEXTURE_2D);

                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

                stbi_image_free(data);
            }

            else
            {
                std::cout << "Texture failed to load at path: " << path << std::endl;
                stbi_image_free(data);
            }

            return textureID;
        }
*/
        glm::mat4 ConvertMatrixToGLMFormatt(const aiMatrix4x4 &from)
        {
            glm::mat4 to;
            // the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
            to[0][0] = from.a1;
            to[1][0] = from.a2;
            to[2][0] = from.a3;
            to[3][0] = from.a4;
            to[0][1] = from.b1;
            to[1][1] = from.b2;
            to[2][1] = from.b3;
            to[3][1] = from.b4;
            to[0][2] = from.c1;
            to[1][2] = from.c2;
            to[2][2] = from.c3;
            to[3][2] = from.c4;
            to[0][3] = from.d1;
            to[1][3] = from.d2;
            to[2][3] = from.d3;
            to[3][3] = from.d4;
            return to;
        }
    };

}

#endif
