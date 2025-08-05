#ifndef FRAMEBUFFER_H
#define FRAMEBUFFER_H


namespace Graphics 
{
    class FrameBuffer
    {
        public:
            FrameBuffer(float width, float height);
            
            ~FrameBuffer();
            unsigned int getFrameTexture();
            void RescaleFrameBuffer();
            void Bind() const;
            void Unbind() const;
            void updateWindowSize(float width, float height);
                
        private:
            float width;
            float height;
            unsigned int fbo;
            unsigned int texture;
            unsigned int rbo;
    };

}

#endif
