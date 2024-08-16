



namespace Graphics {
   
    enum RESOURCE_TYPE {
        NONE,
        STANDARD_VAO
    }

    struct Resource {
        RESOURCE_TYPE type,
	unsigned int ID
    }

    class ResourceManager {
        public:
	    unsigned int& getResource(RESOURCE_TYPE type)
	    {
	         
	    }

        private: 
	    std::vector<Resource> Resources;
                 

