#ifndef MESH_H
#define MESH_H

#include <glad/glad.h>
#include <vector>
#include "Globals.cpp"
#include <glm/glm.hpp>
#include <string>
#include "Shader.h"

namespace Graphics{



class Mesh {
  public: 
    std::vector<Vertex> Vertices;   
    std::vector<unsigned int> Indices;
    std::vector<Texture> Textures;

    Mesh(std::vector<Vertex> verticies, 
        std::vector<unsigned int> indices, 
        std::vector<Texture> textures);

    void Draw(Shader &shader);


  private:
    unsigned int VAO, VBO, EBO;

    // In the future, use polymorphism and dependencuy injection
    // passing in a resource manager, it must have the get VAO, VBO, AND EBO 
    void setupMesh(); 
};

}

#endif // !MESH_H
