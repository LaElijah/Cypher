#ifndef RENDER_API_H
#define RENDER_API_H

#include <map>
#include <vector>
#include <string>
#include "MeshTypes.h"
#include "ResourceManager.h"

namespace Graphics
{


    struct RenderResource {
    	VAO_TYPE vaoType;
    	std::string shaderName;
    	unsigned int VAO;
    	unsigned int VBO; 
    	unsigned int EBO;     
	// Graphics::Shader& shader;
    	// TODO: Add texture vector, up to 16/32 with texture units and what 
    	// sounds like 1024 with texture arrays
    	// Add a material reference    	
        // I could have a property that counts the amount of textures loaded in the texture array
        // and also the resource manager keeps track of a mesh texture index that is 
        // incremented every time a mesh is loaded and then reset 
        // when a render entity reaches its limit 
    };
    
     
    template <typename BaseAPI>
    class RenderAPI
    {
        public:

	    void clear()
	    {
	        static_cast<BaseAPI*>(this)->clearImpl(); 
            }


            void loadData(
			    std::vector<Graphics::Vertex>& vertices, 
			    std::vector<unsigned int>& indices, 
			    std::string shaderName)
	    { 
                static_cast<BaseAPI*>(this)->loadDataImpl(vertices, indices, shaderName); 
	    }

	    void loadTextures(std::vector<Graphics::Texture>& textures)
	    {
	        static_cast<BaseAPI*>(this)->loadTexturesImpl(textures); 
	    }


	    void drawElements(int count, bool unbind = true)
	    {
                static_cast<BaseAPI*>(this)->drawElementsImpl(count, unbind); 
	    } 

	private: 	
    
    };


    class OpenGLRenderAPI : public RenderAPI<OpenGLRenderAPI>
    {
        public: 
	    // Defined methods that align with render api implementation
            void clearImpl();
	    void loadDataImpl(
			    std::vector<Graphics::Vertex>& vertices, 
			    std::vector<unsigned int>& indices, 
			    std::string shaderName);
            void loadTexturesImpl(std::vector<Graphics::Texture>& textures);
	    void drawElementsImpl(int count, bool unbind = true);

        private:
	    // Private methods needed to get render api implementation done
            Graphics::RenderResource& generateRenderResource(
			    VAO_TYPE vaoType, 
			    std::string shaderName);
            
	    Graphics::RenderResource& getRenderResource(
		Graphics::VAO_TYPE vaoType, 
		std::string shaderName);

    	    std::map<std::string, Graphics::RenderResource> RenderResources;
    	    VAO_TYPE CurrentVao = NONE;
    };
}

#endif
