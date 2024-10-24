#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include "Primitives.h"


namespace Graphics
{
    class Mesh 
    {
        public: 
            Mesh
            (
	        std::vector<Graphics::Vertex> vertices, 
                std::vector<unsigned int> indices, 
                std::vector<Texture> textures
	    );

            std::vector<Vertex>& getVertices();
            std::vector<unsigned int>& getIndices();
            std::vector<Texture>& getTextures();

	    std::string& getShaderName();
	    void setShaderName(std::string);
   

    	private:
            std::vector<Vertex> m_Vertices;   
            std::vector<unsigned int> m_Indices;
            std::vector<Texture> m_Textures;
	    std::string ShaderName;

    };

}

#endif // !MESH_H
