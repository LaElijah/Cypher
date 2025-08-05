#ifndef APPLICATION_H
#define APPLICATION_H



namespace Core {



    /**
     * This class handles basic application initialization
     * and start logic. Implements CRTP to support different
     * window/context apis. 
     *
     * Takes in a type matching the inherited subclass to 
     * dispatch function calls to the subclass instance 
     * for polymorphic contexts.
     *
     * CURRENTLY SUPPORTED WINDOW API's
     * - GLFW
     */	
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


    /**
     * This class inherits from and passes its own 
     * type to its parent, provides window and context
     * functionality according to Application interface.
     */
    class GLFWApplication : public Application<GLFWApplication>
    {
        public: 
	    void initImpl();

	    /**
	     * Starts the application 
	     */
	    void startImpl();	
    };
}


#endif
