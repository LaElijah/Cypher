#ifndef RENDER_API_H
#define RENDER_API_H

#include <map>
#include <vector>
#include <string>
#include "MeshTypes.h"
#include "ResourceManager.h"
#include <memory>
#include "Shader.h"

namespace Graphics
{


    struct RenderConfig {
    	VAO_TYPE vaoType;
    	std::string shaderName;
    	unsigned int VAO;
    	unsigned int VBO; 
    	unsigned int EBO;     
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

	    std::shared_ptr<Graphics::OpenGLShader> getShader(std::string name)
	    {
	    
                return static_cast<BaseAPI*>(this)->getShaderImpl(name); 
	    }

            void loadShader(Graphics::ShaderInfo& info)
	    {
                static_cast<BaseAPI*>(this)->loadShaderImpl(info);
	    }

            void loadShaders(std::map<std::string, Graphics::ShaderInfo>& infoArray )
	    {
                static_cast<BaseAPI*>(this)->loadShadersImpl(infoArray);
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
	    std::shared_ptr<Graphics::OpenGLShader> getShaderImpl(std::string name);


            void loadShaderImpl(Graphics::ShaderInfo& info);
            void loadShadersImpl(std::map<std::string, Graphics::ShaderInfo>& infoData);

        private:
	    // Private methods needed to get render api implementation done
            Graphics::RenderConfig& generateRenderConfig(
			    VAO_TYPE vaoType, 
			    std::string shaderName);
            
	    Graphics::RenderConfig& getRenderConfig(
		Graphics::VAO_TYPE vaoType, 
		std::string shaderName);

    	    std::map<std::string, Graphics::RenderConfig> m_RenderConfigs;
    	    VAO_TYPE CurrentVao = NONE;
	    std::string currentShader;

	    std::map<std::string, std::shared_ptr<Graphics::OpenGLShader>> m_Shaders;
    };
}

#endif
