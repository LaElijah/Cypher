#ifndef RENDER_API_H
#define RENDER_API_H

#include <memory>
#include <map>
#include <vector>
#include <string>
#include "Primitives.h"
#include "ResourceManager.h"
#include "Shader.h"


namespace Graphics
{
    



	    struct ElementDrawCall
	    {
	        unsigned int count;
	        unsigned int instanceCount;
	        unsigned int firstIndex;
	        unsigned int baseVertex;
	        unsigned int baseInstance;
	        	
	    };


    /**
     * This class dispatches commands according to the interface
     * it provides onto its instances subclass that its type 
     * has (CRTP)
     */ 
    template <typename T>
    class RenderAPI
    {
        public:
	    void clear()
	    {
	        static_cast<T*>(this)->clearImpl(); 
            }

            void loadData
            (
                std::vector<Graphics::Vertex>& vertices, 
                std::vector<unsigned int>& indices, 
		std::vector<ElementDrawCall>& drawCalls,
                std::string shaderName
	    )
	    { 
                static_cast<T*>(this)
		    ->loadDataImpl
		    (
		        vertices, 
		        indices, 
			drawCalls,
		        shaderName
		    ); 
	    }

	    void loadTextures(std::vector<Graphics::Texture>& textures)
	    {
	        static_cast<T*>(this)
		    ->loadTexturesImpl(textures); 
	    }

	    //void drawElements(int count, bool unbind = false)
            void drawElements(int callNums, bool unbind = false)
	    {
                static_cast<T*>(this)
		    ->drawElementsImpl(callNums, unbind); 
	    }

	    std::shared_ptr<Graphics::OpenGLShader> getShader(std::string name)
	    {
                return static_cast<T*>(this)
		    ->getShaderImpl(name); 
	    }

            void loadShader(Graphics::ShaderInfo& info)
	    {
                static_cast<T*>(this)
		    ->loadShaderImpl(info);
	    }

            void loadShaders(std::map<std::string, Graphics::ShaderInfo>& infoArray )
	    {
                static_cast<T*>(this)
		    ->loadShadersImpl(infoArray);
	    }
    };


    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    ////////  OPENGL API  //////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    ////////////////////////////////////////////////////////////////
    

    /**
     * This struct provides all the information necessary for 
     * openGL to render at the basic level, containing
     * buffer location ids, shader names, and a format 
     * key for performance.
     */
    struct RenderConfig 
    {
	size_t format;
    	std::string shaderName;
    	unsigned int VAO;
    	unsigned int VBO; 
    	unsigned int EBO;     
    	unsigned int IBO;     
	unsigned int SSBO;
    };


    /**
     * This class inherits and passes its type to its parent
     * and provides direct graphics api control and management
     * working with graphics primitives in general with 
     * required implementations.
     */
    class OpenGLRenderAPI : public RenderAPI<OpenGLRenderAPI>
    {
        public: 
	    // Defined methods that align with render api implementation
	    
            void clearImpl();
	    void loadDataImpl
            (
                std::vector<Graphics::Vertex>& vertices, 
                std::vector<unsigned int>& indices, 
		std::vector<Graphics::ElementDrawCall>& drawCalls,
                std::string shaderName
	    );
	    void loadTexturesImpl(std::vector<Graphics::Texture>& textures);
	    //void drawElementsImpl(int count, bool unbind = true);
            void loadShaderImpl(Graphics::ShaderInfo& info);
            void loadShadersImpl(std::map<std::string, Graphics::ShaderInfo>& infoData);
	    std::shared_ptr<Graphics::OpenGLShader> getShaderImpl(std::string name);
        
            void drawElementsImpl(int callNums, bool unbind);
	private:
    	    size_t CURRENT_FORMAT;
    	    
	    std::map<std::string, Graphics::RenderConfig> m_RenderConfigs;
	    std::map<std::string, std::shared_ptr<Graphics::OpenGLShader>> m_Shaders;

	    Graphics::RenderConfig& generateRenderConfig
	    (
                size_t format, 
		std::string shaderName
	    );
            
	    Graphics::RenderConfig& getRenderConfig
            (
		size_t format, 
		std::string shaderName
	    );

            void resetFormat();
    };
}

#endif
