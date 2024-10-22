#ifndef PRIMITIVES_H
#define PRIMITIVES_H


#include <glm/glm.hpp>

namespace Graphics 
{
    struct ModelFile
    {
        std::string path;
        std::string name;
        std::string extension;
    };

    struct Texture
    {
      std::string type;
      std::string path;
      unsigned int id;
  
    };
  
    struct Vertex
    {
      glm::vec3 Position;
      glm::vec3 Normal;
      glm::vec2 TexCoords;
    };
  
}





#endif
