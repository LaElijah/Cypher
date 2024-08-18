#include "Mesh.h"
#include <string>
#include <vector>
#include "Globals.cpp"
#include "ResourceManager.h"
#include <iostream>



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

bool flag = true;
unsigned int count;
unsigned int ccount;
// VAO
void Graphics::Mesh::Draw(Shader& shader)
{
 
       
//

     Graphics::RenderEntity renderEntity = resourceManager->getRenderEntity(STANDARD);
      // Replace the initialization of a new VAO with a resource manager 
      // instead to set/create resources 


    glBindBuffer(GL_ARRAY_BUFFER, renderEntity.VBO);
  
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);


          
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, renderEntity.EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

//

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


      //Graphics::Resource& vao = resourceManager.test(STANDARD);
 
    glBindVertexArray(renderEntity.VAO);
    glDrawElements(GL_TRIANGLES, Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}



// Have a destory mechanism on this object that activates when the reference pointer in the resource manager is destroyed 

// VAO
void Graphics::Mesh::setupMesh()
{

     //Graphics::Resource& vao = resourceManager.test(STANDARD);
	 if (ccount < 1)
  {
	  /*

      glGenVertexArrays(1, &vao.ID); 
      glBindVertexArray(vao.ID);


      glGenBuffers(1, &vao.vbo);
      glGenBuffers(1, &vao.ebo); 
      ccount++; 
  


    glBindBuffer(GL_ARRAY_BUFFER, vao.vbo);


      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);


      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));


      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));

*/
  }


	
	/*
      Graphics::Resource& vao = resourceManager.test(STANDARD);
      // Replace the initialization of a new VAO with a resource manager 
      // instead to set/create resources 



  if (flag)
  {

    std::cout << "SETUP REFERENCE ID LOCATION: ";
    std::cout << &vao.ID << std::endl;

    std::cout << "SETUP VALUE ID: ";
    std::cout << vao.ID << std::endl;

     }

      // Generating and binding vao 

  //ccount++;

  if (ccount < 1)
  {

      glGenVertexArrays(1, &vao.ID); 
      glBindVertexArray(vao.ID);


      glGenBuffers(1, &vao.vbo);
      glGenBuffers(1, &vao.ebo); 
 
  }




  if (flag)
  {

    std::cout << "SETUP POSTGETN REFERENCE ID LOCATION: ";
    std::cout << &vao.ID << std::endl;

    std::cout << "SETUP POSTGETN VAO VALUE ID: ";
    std::cout << vao.ID << std::endl;
    
     }


      // Setting vertex attribute array 


      // Generating, binding, and then loading VBO with data
     glBindBuffer(GL_ARRAY_BUFFER, vao.vbo);


      glEnableVertexAttribArray(0);
      glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);


      glEnableVertexAttribArray(1);
      glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));


      glEnableVertexAttribArray(2);
      glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));




      glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * Vertices.size(), &Vertices[0], GL_STATIC_DRAW);


          
      glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, vao.ebo);
      glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * Indices.size(), &Indices[0], GL_STATIC_DRAW);

 


     glBindVertexArray(0);
*/
};



