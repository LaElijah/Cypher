



namespace Utils {

    

    template<typename T>
    class SparseSet
    {
        public:
	    T get(Entity entity);
            boolean insert(Entity entity, T component);
	    boolean remove(Entity entity);
            	
        private:
	    T dense[]; // Where the entities are stored to point to sparse index
            int denseIndex[]; // An index list holds the index to the dense array
	    int sparse[]; // Where the data is held		
	    int denseN; // the current index to set for the next component added
			// it is incremented or decremented on insert or remove 
    }

    // Entity is the index like in a sparse set
    // it holds the model path in its 
    template<typename T>  
    class MapTable 
    {
        public: 
	    T get(Entity entity);
            boolean insert(Entity entity, T component);
	    bool remove(Entity entity)

	private: 	
    
    }

}
