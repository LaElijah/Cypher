#ifndef RENDER_API_H
#define RENDER_API_H

#include <memory>
#include <map>
#include <vector>
#include <string>
#include "Primitives.h"
#include "ResourceManager.h"
#include "Shader.h"
#include <cstring>

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
            static_cast<T *>(this)->clearImpl();
        }

        void loadData(Graphics::RenderBatch &batch)
        {
            static_cast<T *>(this)->loadDataImpl(batch);
        }

        void loadTextures(std::vector<Graphics::TextureInfo> &textures)
        {
            static_cast<T *>(this)
                ->loadTexturesImpl(textures);
        }

        // void drawElements(int count, bool unbind = false)
        void drawElements(int numCalls, bool unbind = false)
        {
            static_cast<T *>(this)
                ->drawElementsImpl(numCalls, unbind);
        }

        std::shared_ptr<Graphics::OpenGLShader> getShader(std::string name)
        {
            return static_cast<T *>(this)
                ->getShaderImpl(name);
        }

        void loadShader(Graphics::ShaderInfo &info)
        {
            static_cast<T *>(this)
                ->loadShaderImpl(info);
        }

        void loadShaders(std::map<std::string, Graphics::ShaderInfo> &infoArray)
        {
            static_cast<T *>(this)
                ->loadShadersImpl(infoArray);
        }

        int flush(Graphics::RenderBatch &batch)
        {
            return static_cast<T *>(this)
                ->flushImpl(batch);

        }

	unsigned int getBufferID(std::string name)
	{
            return static_cast<T*>(this)
		    ->getBufferIDImpl(name);
	}

	std::shared_ptr<Graphics::BufferRecord> insert(void* data, int byteSize, std::string name)
	{
            return static_cast<T*>(this)
		->insertImpl(data, byteSize, name);
	}
	bool createNamedBuffer(int byteSize, std::string name)
	{
            return static_cast<T*>(this)
		    ->createNamedBufferImpl(byteSize, name);
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
        unsigned int IBO[2];
        unsigned int SSBO[2];
    };

    class OpenGLBuffer
    {
	public:
	    OpenGLBuffer(int size, std::string name)
	    {
                buffer = std::make_unique<Graphics::Buffer>(Graphics::Buffer(size, name));


		glCreateBuffers(1, &id);
		glNamedBufferData(id, size, NULL, GL_DYNAMIC_DRAW);
	    }
	    std::unique_ptr<Graphics::Buffer> buffer = nullptr;   

	    bool load(void* data, std::shared_ptr<Graphics::BufferRecord> record)
	    {
	        void* ptr = glMapNamedBuffer(id, GL_WRITE_ONLY);
		if (ptr)
		{
                    memcpy((char*)ptr + record->start, data, record->size);
		    glUnmapNamedBuffer(id);
		    return true;
		}

		return false; 

          
	    }

	    unsigned int getID()
	    {
                return id;
	    }

	    void clear()
	    {
                return buffer->clear();
	    }
	private:
	    unsigned int id;
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

	bool clearBuffer(std::string name);
	bool createNamedBufferImpl(int byteSize, std::string name);

	unsigned int getBufferIDImpl(std::string name);
	std::shared_ptr<Graphics::BufferRecord> insertImpl(void* data, int byteSize, std::string name);
        // Defined methods that align with render api implementation
        void clearImpl();
        void loadDataImpl(Graphics::RenderBatch &batch);

        unsigned int loadTextureImpl(Graphics::TextureInfo &info);

        // TODO: rename to bindTexturesImpl
        void loadTexturesImpl(std::vector<Graphics::TextureInfo> &textures);
        void drawElementsImpl(int numCalls, bool unbind);
        void loadShaderImpl(Graphics::ShaderInfo &info);
        void replaceShaderImpl(Graphics::ShaderInfo &info);
        void loadShadersImpl(std::map<std::string, Graphics::ShaderInfo> &infoData);
        std::shared_ptr<Graphics::OpenGLShader> getShaderImpl(std::string name);
        int flushImpl(Graphics::RenderBatch &batch);
        unsigned int loadFileTexture(Graphics::TextureInfo &texture);

    private:
	std::map<std::string, std::shared_ptr<Graphics::OpenGLBuffer>> buffers;
        size_t CURRENT_FORMAT;
        std::map<std::string, unsigned int> loadedTextureData;
        std::set<uint64_t> residentHandles;

        unsigned int loadTextureData(Graphics::TextureInfo &info);
        std::vector<GLuint64> loadTextureHandles(std::vector<Graphics::TextureInfo> &textureInfo);

        std::map<unsigned int, uint64_t> loadedTextureHandles;
        void offloadTextureHandles(Graphics::RenderBatch &batch);

        std::map<std::string, Graphics::RenderConfig> m_RenderConfigs;

        std::map<std::string, std::shared_ptr<Graphics::OpenGLShader>> m_Shaders;

        Graphics::RenderConfig &generateRenderConfig(
            size_t format,
            std::string shaderName);

        Graphics::RenderConfig &getRenderConfig(
            size_t format,
            std::string shaderName);

        void resetFormat();
    };
}

#endif
