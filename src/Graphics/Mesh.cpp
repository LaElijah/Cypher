#include "Mesh.h"
#include <string>
#include <vector>
#include "Globals.cpp"

  
Graphics::Mesh::Mesh(
    std::vector<Graphics::Vertex> 
    vertices, std::vector<unsigned int> indices, 
    std::vector<Graphics::Texture> textures)
{
  this->Vertices= vertices; 
  this->Indices = indices;
  this->Textures = textures;

  setupMesh();
}


void Graphics::Mesh::Draw(Shader &shader)
{
  unsigned int diffuseN = 1;
  unsigned int specularN = 1;
  for (unsigned int i = 0; i < Textures.size(); i++)
  {
    glActiveTexture(GL_TEXTURE0 + i);

      
    std::string number;
    std::string name = Textures[i].type;


    if(name == "texture_diffuse")
      number = std::to_string(diffuseN++);
    else if (name == "texture_specular")
      number = std::to_string(specularN++);

    shader.setInt(("material." + name + number).c_str(), i);
    glBindTexture(GL_TEXTURE_2D, Textures[i].id);

  }
  glActiveTexture(GL_TEXTURE0);

  glBindVertexArray(VAO);
  glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
  glBindVertexArray(0);
}

void Graphics::Mesh::setupMesh()
{
      // Replace the initialization of a new VAO with a resource manager 
      // instead to set/create resources 
     
      // Generating and binding vao 
      glGenVertexArrays(1, &VAO);
      glGenBuffers(1, &VBO);
      glGenBuffers(1, &EBO); 
      
      glBindVertexArray(VAO);
      // Generating, binding, and then loading VBO with data
      glBindBuffer(GL_ARRAY_BUFFER, VBO);

      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);


          
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);


      // Setting vertex attribute array 
      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);


      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));


      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

      glBindVertexArray(0);
    };



