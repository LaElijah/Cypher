#ifndef GLOBALS_CPP
#define GLOBALS_CPP








#include <glm/glm.hpp>
#include <string>


namespace Graphics {
  

  enum Direction 
  {
    LEFT,
    RIGHT,
    FORWARDS,
    BACKWARDS,
    UP,
    DOWN
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


#endif // !GLOBALS_CPP
