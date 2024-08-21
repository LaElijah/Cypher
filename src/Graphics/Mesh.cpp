#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <vector>
#include <iostream>
#include "ResourceManager.h"
#include "Mesh.h"




Graphics::Mesh::Mesh(
    std::vector<Graphics::Vertex> 
    vertices, std::vector<unsigned int> indices, 
    std::vector<Graphics::Texture> textures,
    Graphics::ResourceManager* rManager)
{
    this->Vertices= vertices; 
    this->Indices = indices;
    this->Textures = textures;
    this->resourceManager = rManager;

    setupMesh();
}

void Graphics::Mesh::Draw(Shader& shader)
{
     Graphics::RenderEntity renderEntity = resourceManager->getRenderEntity(STANDARD);


    glBindBuffer(GL_ARRAY_BUFFER, renderEntity.VBO);
  
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);


          
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderEntity.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

//

    unsigned int diffuseN = 1;
    unsigned int specularN = 1;
    unsigned int ambientN = 1;
    unsigned int lightmapN = 1;
    unsigned int reflectionN = 1;
   
    for (unsigned int i = 0; i < Textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);

          
        std::string number;
        std::string name = Textures[i].type;



        if(name == "texture_diffuse")
          number = std::to_string(diffuseN++);
        
	else if (name == "texture_specular")
          number = std::to_string(specularN++);

	else if (name == "texture_ambient")
          number = std::to_string(ambientN++);

	else if (name == "texture_lightmap")
          number = std::to_string(lightmapN++);

	else if (name == "texture_reflection")
          number = std::to_string(reflectionN++);


        shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, Textures[i].id);

    }

    glActiveTexture(GL_TEXTURE0);


      //Graphics::Resource& vao = resourceManager.test(STANDARD);
 
    glBindVertexArray(renderEntity.VAO);
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}



// Have a destory mechanism on this object that activates when the reference pointer in the resource manager is destroyed 

// VAO
void Graphics::Mesh::setupMesh()
{

};



