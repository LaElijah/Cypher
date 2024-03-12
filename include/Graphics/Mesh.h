#ifndef MESH_H
#define MESH_H




namespace Graphics {
  class Mesh {
    unsigned int VBO;
    unsigned int EBO;
    unsigned int VAO;
    unsigned int Textures[2];



    public: 
    Mesh(float *verticies, unsigned int *indices);
    unsigned int getVAO();


    private:

    void loadVBO(float *verticies);
    void loadEBO(unsigned int *indices);    
    void loadVAO(); 
    void loadTexture(unsigned int activeTexture, unsigned int texture, const char *path);
  };
}





#endif
