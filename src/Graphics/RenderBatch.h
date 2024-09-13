

namespace Graphics {

    template<T>
    struct RenderBatch 
    {
        public: 
	    Shader shader;
            unsigned int VAO;
            unsigned int VBO;
            std::vector<T> textureIndices;

	    // add texture function that takes in 
	    // the current index of meshes, and 
	    // uses that as the index to the array of 
	    // vectors, then it submits the texture 
	    // to the binding function which assigns the textures to 
	    // the appropriate resolution scale, and then assigns the 
	    // vec component x to the resolution 
	    

    }

	


}
