



namespace Graphics
{
    class System
    {
        public:
	    void update();

	protected:
	    void tick();

	private: 
    
    };


    class RenderSystem : System
    {
        public: 
            void update();

	    std::vector<Graphics::RenderBatch&>& getBatches()
	    {
	        return batches; 
	    }

         private:  
	    std::vector<Graphics::RenderBatch&> batches;    
    
    };
};
