#include "../../include/Graphics/Mesh.h"
namespace Graphics {
  


  Graphics::Mesh Mesh(float *verticies, unsigned int *indices)
    {


      loadVBO(verticies);
      loadEBO(indices);

      loadVAO();



      glGenTextures(2, Textures); 


//      loadTexture(GL_TEXTURE0, Textures[0], "/home/laelijah/Programming/Gengine/extras/gate.jpg");
    
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, Textures[0]);


        float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
        glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor); 
     
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST); 
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   


        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        loadImageData("/home/laelijah/Programming/Gengine/extras/gate.jpg");












 //     loadTexture(GL_TEXTURE1, Textures[1], "/home/laelijah/Programming/Gengine/extras/logo.jpeg"); 
 
        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, Textures[1]);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);    
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    }


    unsigned int getVAO()
    {
      return VAO;
    }


    private:

    void loadVBO(float *verticies)
    {


    float Triangle[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,  // top left 
    };


      bool exists = false;

      if (!exists)
      {
      
        // Generate a Vertex Buffer Object
        glGenBuffers(1, &VBO); 

        // Select working vertex buffer object 
        glBindBuffer(GL_ARRAY_BUFFER, VBO);

        // Set buffer data to verticies 
        glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle, GL_STATIC_DRAW);

      }

    }




    void loadEBO(unsigned int *indices)
    {

    unsigned int indicess[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

 
      bool exists = false;

      if (!exists)
      {
        // Generate a buffer and assign an id that references to GPU EBO memory 
        glGenBuffers(1, &EBO);
     
        // Select working EBO Object
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

        // Add index data 
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indicess), indicess, GL_STATIC_DRAW);
      }

    }



    void loadVAO()
    {

      bool exists = false;
      if (!exists)
      {

        glGenVertexArrays(1, &VAO);
        glBindVertexArray(VAO);
        // VERTEX ATTRUBUTE SETUP
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);	// Vertex attributes stay the same
        glEnableVertexAttribArray(0);
       
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));	// Vertex attributes stay the same
        glEnableVertexAttribArray(1);

        
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));	// Vertex attributes stay the same
        glEnableVertexAttribArray(2);

      
        glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));	// Vertex attributes stay the same
        glEnableVertexAttribArray(3);
     
      }
    }


    void loadTexture(unsigned int activeTexture, unsigned int texture, const char *path)
    {
      bool exists = false;

      if(!exists)
      {

        
        // TEXTURE APPLICATIONS
        //
        //
        // FIRST TEXTURE
        glActiveTexture(activeTexture);
        glBindTexture(GL_TEXTURE_2D, texture);
          

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

        loadImageData(path);
      }  
    }



  };
}






