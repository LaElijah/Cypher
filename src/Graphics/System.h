#ifndef SYSTEM_H
#define SYSTEM_H

#include "EntityManager.h"
#include <vector>
#include "RenderBatch.h"
#include "Mesh.h"
#include "Model.h"


namespace Graphics 
{
    template <typename Derived>
    class System {
        public: 
	    void update(float deltaTime); 
	//protected:
	    //EntityManager m_EntityManager;
    };


    class RenderSystem //: public System<RenderSystem>
    {
	public: 
            void update(float deltaTime);
	    std::vector<RenderBatch> getBatches();
        private: 
	    void sortEntity(Entity entity);
            std::vector<RenderBatch> batches;	     
            Graphics::Model modela = Graphics::Model("/home/laelijah/Gengine/data/Models/room/scene.gltf");
    };
}

#endif
