/*
#include "../../external/GLAD/glad.h"
#include <GLFW/glfw3.h>
#include <assimp/Importer.hpp>
#include <assimp/material.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <string>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include "../../external/STB_IMAGE/stb_image.h"
#include "Model.h"


glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from);


Graphics::Model::Model(std::string path)
{
    loadModel(path);
}




void Graphics::Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile
	                   (
			       path, 
			       aiProcess_Triangulate 
			       //|| aiProcess_PreTransformVertices
			   );
  
    if
    (
          !scene 
          || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE 
	  || !scene->mRootNode
    )
    {
        std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
        return;
    }

    directory = path.substr(0, path.find_last_of('/'));

    // Parent root node of model
    processNode(scene->mRootNode, scene, glm::mat4(1.0f));
}



void Graphics::Model::processNode(aiNode *node, const aiScene *scene, const glm::mat4& parentTransform)
{
    glm::mat4 nodeTransform = ConvertMatrixToGLMFormat(node->mTransformation);
    glm::mat4 globalTransform = parentTransform * nodeTransform;

    // Pack child nodes ( meshes ) in this parent node
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
	// Process mesh loads in the 
	// primitive data of the given node
        meshes.push_back(processMesh(mesh, scene, globalTransform)); 
    }

    // Move onto the children of this node
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene, globalTransform);
    }
}




Graphics::Mesh Graphics::Model::processMesh(aiMesh *mesh, const aiScene *scene, const glm::mat4& transform)
{
    std::vector<Graphics::Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Graphics::TextureInfo> textures;


    for(unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
       
       	glm::vec3 vector = glm::vec3(
			transform * glm::vec4(mesh->mVertices[i].x, 
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

        if(mesh->mTextureCoords[0])
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

    
    for (unsigned int i = 0; i< mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for(unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }


    aiMaterial *material = scene->mMaterials[mesh->mMaterialIndex];

    std::vector<Graphics::TextureInfo> diffuseMaps = loadMaterialTextures(
        material, 
        scene,
        aiTextureType_DIFFUSE, 
        "texture_diffuse");


    textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());

    std::vector<Texture> specularMaps = loadMaterialTextures(
        material,
        scene,
        aiTextureType_SPECULAR, 
        "texture_specular");

    textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());

    return Mesh(vertices, indices, textures);
}

std::vector<Graphics::TextureInfo> Graphics::Model::loadMaterialTextures(aiMaterial *mat, const aiScene* scene, aiTextureType type, std::string typeName)
{
    std::vector<Graphics::TextureInfo> textures;

    for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {    
        aiString str; // Texture path 
        mat->GetTexture(type, i, &str);
        bool skip = false;
        
        // Does this current mesh have any textures already loaded into memory for this model? 
        for (unsigned int j = 0; j < texturesLoaded.size(); j++)
        { 
            if(std::strcmp(texturesLoaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(texturesLoaded[j]);
                skip = true;
                break;
            } 
        }

    // If not already found in loaded textures, 
        if(!skip)
        {      
            Graphics::TextureInfo texture;

            // Check here for embedded or not and emply the right texture load function
            if (str.length > 0 && str.data[0] == '*') // If embedded
                texture.id = TextureFromEmbedded(scene, std::stoi(&str.data[1]));
            else
                texture.id = TextureFromFile(str.C_Str(), this->directory);
 
            texture.type = typeName;
            texture.path = str.C_Str();
	    texture.directory = this->directory;
	   
            textures.push_back(texture);           
            texturesLoaded.push_back(texture);  
        }  
    } 
    return textures;
};



// TODO: MAKE FUNCTIONAL
unsigned int Graphics::Model::TextureFromEmbedded(const aiScene* scene, int embeddedIndex)
{ 
    std::cout << "LOADING EMBEDDED TEXTURE" << std::endl;
    unsigned int textureID;
    glGenTextures(1, &textureID);

    const aiTexture* embeddedTexture = scene->mTextures[embeddedIndex];
   
    if (embeddedTexture->pcData) 
    {
        // Update for a wider range of channels

	int width = embeddedTexture->mWidth;
	int height = embeddedTexture->mHeight;
        int format = 3;
	
	        // Not yet working need to fnd a way to get format	
		//embeddedTexture->mFormat == aiTextureFormat::aiTextureFormat_RGBA ? 4 : 3;
        
        unsigned char* data = new unsigned char[width * height * format];
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


unsigned int Graphics::Model::TextureFromFile(std::string path, std::string directory)
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




glm::mat4 Graphics::Model::getModelMatrix()
{
    return glm::scale(glm::translate(glm::mat4(1.0f), Position), Scale);
}




std::vector<Graphics::Mesh>& Graphics::Model::getMeshes()
{
    return meshes;
}




std::string Graphics::Model::getShaderName()
{
    return shaderName;
}




void Graphics::Model::setShaderName(std::string name)
{
    shaderName = name;
}




glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
{
    glm::mat4 to;
    //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
    to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
    to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
    to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
    to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
    return to;
}

*/
