#ifndef APPLICATION_H
#define APPLICATION_H




namespace Core {


   
    template <typename T> 	
    class Application 
    {
	public:
            Application();

            void init()
	    {
	        static_cast<T*>(this)->initImpl(); 
	    }

	    template <typename U>
            void start(Graphics::RenderAPI<U>& api)
	    {
	        static_cast<T*>(this)->startImpl(api); 
	    }	    
    };


    class GLFWApplication : Application<GLFWApplication>
    {
        public: 
	    void initImpl();

            template <typename T>
	    void startImpl(Graphics::RenderAPI<T>& api);	

	private:
	    std::shared_ptrGraphics::GLFWCanvas Canvas;

    };


}




#endif
