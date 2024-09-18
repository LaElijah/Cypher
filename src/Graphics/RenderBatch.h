#ifndef RENDER_BATCH_H
#define RENDER_BATCH_H


#include "MeshTypes.h"

#include "Mesh.h"
namespace Graphics {
/*
    //template<T>
    struct RenderBatch 
    {
        std::string shaderName;
	std::vector<Graphics::Vertex> vertices;
	std::vector<std::vector<unsigned int>> indices;
	std::vector<glm::mat4> modelTransforms;
	std::vector<Texture> textures;	    
        // Maybe a batch draw type (single/batch) 
	// Remove constructor 
	RenderBatch(
        const std::string& shaderName,
        const std::vector<Graphics::Vertex>& vertices,
        const std::vector<std::vector<unsigned int>>& indices,
        const std::vector<glm::mat4>& modelTransforms,
        const std::vector<Texture>& textures
    )
        : shaderName(shaderName),
          vertices(vertices),
          indices(indices),
          modelTransforms(modelTransforms),
          textures(textures)
    {
        // Constructor body can be empty if there's no additional logic
    }
    };
*/
    //template<T>
    //
    //

struct GLDrawElementsIndirectCommand {
    unsigned int count;         // Number of indices
    unsigned int instanceCount; // Number of instances
    unsigned int firstIndex;    // Starting index in the index buffer
    unsigned int baseVertex;     // Base vertex added to each index
    unsigned int baseInstance;  // Base instance added to each instance
};
    struct RenderBatch 
    {

	std::vector<glm::mat4> modelTransforms;
        std::string shaderName;
	std::vector<Graphics::Vertex> vertices;
	std::vector<int> counts;
	std::vector<unsigned int> indices;
	int numCalls;	
        std::vector<Graphics::GLDrawElementsIndirectCommand> drawCalls; 
/*
	RenderBatch(
	std::vector<glm::mat4> modelTransforms,
        const std::string& shaderName,
	std::vector<Graphics::Vertex> vertices,
	std::vector<int> counts,
	std::vector<unsigned int> indices,
	int numCalls, 
        std::vector<Graphics::GLDrawElementsIndirectCommand> drawCalls	
    ) : 
	modelTransforms(modelTransforms),
	shaderName(shaderName), 
	vertices(vertices),
        counts(counts),
	indices(indices),
	numCalls(numCalls),
        drawCalls(drawCalls)	
    {
        // Constructor body can be empty if there's no additional logic
    }

    */
    };

}

#endif
