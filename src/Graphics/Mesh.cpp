#include <string>
#include <vector>
#include "Mesh.h"




Graphics::Mesh::Mesh(
    std::vector<Graphics::Vertex> 
    vertices, std::vector<unsigned int> indices, 
    std::vector<Graphics::Texture> textures)
{
    Vertices = vertices; 
    Indices = indices;
    Textures = textures;
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



std::string& Graphics::Mesh::getShaderName() 
{
    return ShaderName;
}



void Graphics::Mesh::setShaderName(std::string name)
{
    ShaderName = name;
}
