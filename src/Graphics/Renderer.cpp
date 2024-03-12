


#include "../../include/Graphics/Renderer.h"
#include <GLFW/glfw3.h>
#include "../../include/Graphics/Shader.h"


void Graphics::Renderer::draw(Graphics::Shader shaderProgram, float &visibility, unsigned int VAO, GLFWwindow *window)
    {

        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);


        float timeValue = glfwGetTime();
        float sinWave = (sin(timeValue) / 2.0f) + 0.5f;


    shaderProgram.use();
    shaderProgram.setInt("texture1", 0);
    shaderProgram.setInt("texture2", 1);
     
    shaderProgram.setFloat("visibility", visibility);
    shaderProgram.setFloat("offset", 0.0f);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);	


    // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
    // -------------------------------------------------------------------------------
    glfwSwapBuffers(window);
    glfwPollEvents();

    }
