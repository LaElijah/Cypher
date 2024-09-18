


#include "System.h"


template <typename TSystem>
void Graphics::System<TSystem>::update(float deltaTime)
{
    //static_cast<TSystem*>(this)->update(deltaTime);
}


void Graphics::RenderSystem::update(float deltaTime)
{
 /*   
    for (EntityIterator it = m_EntityManager->iterator(); it != it.end(); ++it)
    { 
	Entity entity = *it;
	if (hasComponents(entity))
            sortEntity(*it);	
    } 
    */
}

/*
Graphics::RenderSystem::sortEntity(Entity entity)
{
    
}
*/

#include <iostream>

bool batchMade = false;
std::vector<Graphics::RenderBatch> Graphics::RenderSystem::getBatches()
{
if (!batchMade)
{
   std::cout << "MODEL LOAD" << std::endl;
   std::vector<glm::mat4> modelTransforms;

   std::cout << "MODEL MATRIX" << std::endl;
   modelTransforms.push_back(modela.getModelMatrix());
  
   std::cout << "MODEL LOAD2" << std::endl;
	  
   //for (Graphics::Mesh& mesh : modela.getMeshes())
   //{
	   std::cout << "LOADING MESH" << std::endl;
/*
       indices.push_back(mesh.getIndices()); 
       batches.push_back(new Graphics::RenderBatch(
	       std::string("DEBUG_VAO_DEBUG_SHADER"), 
               mesh.getVertices(), 
	       indices, 
	       modelTransforms, 
	       mesh.getTextures()));
*/
 
       std::vector<unsigned int> indices;
       std::vector<int> counts;
       std::vector<Graphics::Vertex> vertices;
       unsigned int indexOffset = 0;
       unsigned int vertexOffset = 0;
       std::vector<Graphics::GLDrawElementsIndirectCommand> drawCalls;
       int numCalls = 0;
       for (Graphics::Mesh& mesh : modela.getMeshes())
       {
           vertices.insert(vertices.end(), mesh.getVertices().begin(), mesh.getVertices().end()); 

           counts.insert(counts.end(), mesh.getIndices().begin(), mesh.getIndices().end()); 
           indices.insert(indices.end(), mesh.getIndices().begin(), mesh.getIndices().end()); 


	   Graphics::GLDrawElementsIndirectCommand drawCall
	   {
	       mesh.getIndices().size(),
	       1, // Instance count
	       indexOffset,
	       vertexOffset,
	       0 // base instance 
	   };

	   drawCalls.push_back(drawCall);
	   indexOffset += mesh.getIndices().size();
	   vertexOffset += mesh.getVertices().size();
       }


       numCalls = modela.getMeshes().size(); 
       
       Graphics::RenderBatch batch
       {
	       modelTransforms,
	       std::string("DEBUG_VAO_DEBUG_SHADER"), 
               vertices, 
	       counts,
	       indices,
	       numCalls,
	       drawCalls
       };
       batches.push_back(batch);

       
   batchMade = true;
   }

   return batches;
}

