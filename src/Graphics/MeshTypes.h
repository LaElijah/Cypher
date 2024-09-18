#ifndef MESHTYPES_H
#define MESHTYPES_H


#include <glm/glm.hpp>
#include <string>

namespace Graphics {
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
      // Remove this, leave texture handling to render api
      unsigned int id;
  
    };
  
    struct Vertex
    {
      glm::vec3 Position;
      glm::vec3 Normal;
      glm::vec2 TexCoords;
    };
  
 

    enum SHADER_NAME {
    	DEBUG_SHADER,
    	STANDARD_SHADER
    };

    enum VAO_TYPE {
        NONE,
    	DEBUG,
    	STANDARD
    };
}





#endif
