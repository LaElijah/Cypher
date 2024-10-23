#ifndef APPLICATION_H
#define APPLICATION_H




namespace Core {


   
    template <typename T> 	
    class Application 
    {
	public:

            void init()
	    {
	        static_cast<T*>(this)->initImpl(); 
	    }

            void start()
	    {
	        static_cast<T*>(this)->startImpl(); 
	    }	    
    };


    class GLFWApplication : public Application<GLFWApplication>
    {
        public: 
	    void initImpl();

	    void startImpl();	


    };


}




#endif
