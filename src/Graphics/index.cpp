#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include "../../include/Graphics/GLCanvas.h"
#include "../../include/Graphics/Shader.h"
#include <math.h>
#include "../../include/Graphics/stb_image.h"
// settings

void loadImageData(const char *path);



int main()
{    // build and compile our shader program
    // ------------------------------------
    // we skipped compile log checks this time for readability (if you do encounter issues, add the compile-checks! see previous code samples)

    Graphics::GLCanvas canvas;
    
    canvas.initializeCanvas();

    GLFWwindow *window = canvas.getWindow();



    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------
float Triangle[] = {
        // positions          // colors           // texture coords
         0.5f,  0.5f, 0.0f,   1.0f, 0.0f, 0.0f,   1.0f, 1.0f, // top right
         0.5f, -0.5f, 0.0f,   0.0f, 1.0f, 0.0f,   1.0f, 0.0f, // bottom right
        -0.5f, -0.5f, 0.0f,   0.0f, 0.0f, 1.0f,   0.0f, 0.0f, // bottom left
        -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 0.0f,   0.0f, 1.0f  // top left 
    };
    unsigned int indices[] = {  
        0, 1, 3, // first triangle
        1, 2, 3  // second triangle
    };

    Graphics::Shader shaderProgram(
        "/home/ubuntu/Programming/Gengine/extras/SimpleVertexShader.glsl",
        "/home/ubuntu/Programming/Gengine/extras/SimpleFragmentShader.glsl");




    unsigned int VBOs[1], VAOs[1], EBO, textures[2];
    


    glGenVertexArrays(1, &VAOs[0]); // we can also generate multiple VAOs or buffers at the same time
    glGenBuffers(1, &VBOs[0]);
    glGenBuffers(1, &EBO);
  

    // first triangle setup
    // --------------------
    glBindVertexArray(VAOs[0]);
  
    glBindBuffer(GL_ARRAY_BUFFER, VBOs[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Triangle), Triangle, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);	// Vertex attributes stay the same
    glEnableVertexAttribArray(0);
   
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));	// Vertex attributes stay the same
    glEnableVertexAttribArray(1);

    
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));	// Vertex attributes stay the same
    glEnableVertexAttribArray(2);

    

    glGenTextures(2, textures); 

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textures[0]);
          
    glActiveTexture(GL_TEXTURE1);
    glBindTexture(GL_TEXTURE_2D, textures[1]);


    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);   


    loadImageData("/home/ubuntu/Programming/Gengine/extras/wet.jpeg");
   

    // uncomment this call to draw in wireframe polygons.
    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    shaderProgram.seInt("texture_1", 0);
    shaderProgram.seInt("texture_2", 1);
    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // input
        // -----
        Graphics::processInput(window);
    
        // render
        // ------
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        float timeValue = glfwGetTime();
        float sinWave = (sin(timeValue) / 2.0f) + 0.5f;


        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, textures[0]);


        glActiveTexture(GL_TEXTURE1);
        glBindTexture(GL_TEXTURE_2D, textures[1]);


        shaderProgram.use();
        shaderProgram.setFloat("offset", 0.0f, 0.0f);
        // shaderProgram.setFloat("colorValue", 1.0f, 0.2f);
       // draw the first triangle using the data from our first VAO
   

        glBindVertexArray(VAOs[0]);
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	
        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAOs[0]);
    glDeleteBuffers(1, &VBOs[0]);
    glDeleteBuffers(1, &EBO);
    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow *window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void loadImageData(const char* path)
{
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
