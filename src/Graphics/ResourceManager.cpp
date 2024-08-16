#include "ResourceManager.h"
#include <stdexcept>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include <iostream>
#include "Globals.cpp"
// TODO: Change this into the globals being in the resource manager function 



/*

Graphics::ResourceManager Graphics::ResourceManager::ResourceManager()
{
    

}
*/



Graphics::RenderEntity& Graphics::ResourceManager::generateRenderEntity(VAO_TYPE type)
{

    RenderEntity entity;
    entity.vaoType = type;
    
   // TODO: instead of passing in the shader, set up a getShader function to get reference 
    //entity.shader = shader;


    glGenVertexArrays(1, &entity.VAO); 
    glBindVertexArray(entity.VAO);

    glGenBuffers(1, &entity.VBO);
    glGenBuffers(1, &entity.EBO); 

    glBindBuffer(GL_ARRAY_BUFFER, entity.VBO);


    switch (type)
        case STANDARD:
            {
                glEnableVertexAttribArray(0);
                glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);


                glEnableVertexAttribArray(1);
                glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));


                glEnableVertexAttribArray(2);
                glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, TexCoords));
            }

	 
    RenderEntities.push_back(std::move(entity));

    return getRenderEntity(type);
}

Graphics::RenderEntity& Graphics::ResourceManager::getRenderEntity(VAO_TYPE type)
{
    // Search through saved render entities
    auto entity = std::find_if(
		  RenderEntities.begin(), 
	          RenderEntities.end(), 
		  [type] 
		  ( const Graphics::RenderEntity& entity )
		  { return entity.vaoType == type; });
    
    // Was the vao found in our vector?
    if (entity != RenderEntities.end()) 
    {
        // Return the found render entitiy
	return *entity;
    }
    else 
    {
	std::cout << "GENERATING NEW RENDER ENTITY" << std::endl;
        return generateRenderEntity(type);
    }




}


