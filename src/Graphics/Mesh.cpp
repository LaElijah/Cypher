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
    std::vector<Graphics::Texture> textures)
{
    this->Vertices= vertices; 
    this->Indices = indices;
    this->Textures = textures;

}

std::vector<Graphics::Vertex>& Graphics::Mesh::getVertices()
{
    return Vertices;
}


std::vector<unsigned int>& Graphics::Mesh::getIndices()
{
    return Indices;
}


std::vector<Graphics::Texture>& Graphics::Mesh::getTextures()
{
    return Textures;
}

