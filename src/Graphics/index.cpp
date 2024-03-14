#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../include/Graphics/GLCanvas.h"
#include "../../include/Graphics/Shader.h"
#include <math.h>
#include "../../include/Graphics/stb_image.h"
#include "../../include/Graphics/Mesh.h"
#include "../../include/Graphics/Renderer.h"


// settings

void loadImageData(const char *path);


int main()
{
    Graphics::GLCanvas canvas;    
    canvas.initializeCanvas();
    GLFWwindow *window = canvas.getWindow();

   float Triangle[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, 1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, 0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f, 0.0f, 1.0f,  // top left 
    };

    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    float visibility = 0.5f;


















    // START
    // Graphics::Mesh Rectangle(Triangle, indices);

     // render
        // ------

        



    // ASSIGNMENT ORDER 
    // GENERATE VERTEX ARRAY 
    // BIND IT 
    // SET VERTEX BUFFER OBJECT 
    // BIND BUFFER DATA 
    // IF APPLICABLE DO THE SAME FOR ELEMENT BUFFER OBJECT
    // APPLY TEXTURES
    //
    // APPLY VERTEX ATTRIBUTES AFTER ALL DATA IS FIRST MOUNTED 

    unsigned int VBOs[1], VAOs[1], EBO, textures[2];
   

    glGenVertexArrays(1, &VAOs[0]); // we can also generate multiple VAOs or buffers at the same time

    // first triangle setup
    // --------------------
 


    glBindVertexArray(VAOs[0]);


    glGenBuffers(1, &VBOs[0]);
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle, GL_STATIC_DRAW);


    glGenBuffers(1, &EBO); 
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);







    glGenTextures(2, textures); 

    // TEXTURE APPLICATIONS
    //
    //
    // FIRST TEXTURE
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
 
    float borderColor[] = { 0.0f, 0.0f, 0.0f, 0.0f };
    glTexParameterfv(GL_TEXTURE_2D, GL_TEXTURE_BORDER_COLOR, borderColor);
 
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);


    
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);   

    loadImageData("/home/laelijah/Programming/Gengine/extras/gate.jpg");
    


    // SECOND TEXTURE
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);    
    loadImageData("/home/laelijah/Programming/Gengine/extras/logo.jpeg");


    

  



    // VERTEX ATTRUBUTE SETUP
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);
   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(3 * sizeof(float)));	// Vertex attributes stay the same
    glEnableVertexAttribArray(1);

    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(6 * sizeof(float)));	// Vertex attributes stay the same
    glEnableVertexAttribArray(2);

  
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, 10 * sizeof(float), (void*)(8 * sizeof(float)));	// Vertex attributes stay the same
    glEnableVertexAttribArray(3);



 










// ENDDDDDD

    Graphics::Shader shaderProgram(
        "/home/laelijah/Programming/Gengine/extras/vertex.vs",
        "/home/laelijah/Programming/Gengine/extras/fragment.fs"
    );

    Graphics::Renderer renderer;


      // Rendering in window 
    while (!glfwWindowShouldClose(window)) 
    {
      Graphics::processInput(window, visibility);
      
      renderer.draw(shaderProgram, visibility, VAOs[0], window);
    }




    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    // glDeleteVertexArrays(1, &VAOs[0]);
    //glDeleteBuffers(1, &VBOs[0]);
    // glDeleteBuffers(1, &EBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

void loadImageData(const char* path)
{
    stbi_set_flip_vertically_on_load(true);
    int width, height, nrChannels;
    unsigned char *data = stbi_load(path, &width, &height, &nrChannels, 0);


    if (data)
    {
      glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data); 
      glGenerateMipmap(GL_TEXTURE_2D);
    }

    else 
    {
      std::cout << "Failed to load texture" << std::endl;
    }

    stbi_image_free(data);
    
}
