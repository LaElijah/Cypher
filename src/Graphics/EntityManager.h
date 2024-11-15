#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include "Primitives.h"
#include <set>

namespace Graphics
{
    class EntityManager
    {
        public:
	    Graphics::Entity createEntity();

	    bool removeEntity(Entity entity);

	private: 
	    Entity HEAD_ENTITY = 0;
	    // Entities in use
            std::set<Entity> m_ActiveEntities;

	    // Where destroyed entity IDs are created to be prioritized for reuse 
   	    std::set<Entity> m_InactiveEntities;
    
    
    };



}


#endif
