#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

// #include "Primitives.h"
// #include <vector>
// #include <typeindex>
// #include <unordered_map>
// #include <memory>
// #include <iostream>
#include "Iterators.h"
#include "ArchetypeManager.h"


namespace Graphics
{

    class ComponentManager
    {
        public:
            template <typename T>
            Graphics::WideIterator<T> getAllContaining()
            {
                return archetypeManager.getAllContaining<T>();
            }
    
    
            template <typename T>
            T get(Graphics::Entity entity)
            {
                return archetypeManager.get<T>(entity);
            }
    
    
    
    
    	template <typename T> 
    	bool remove(Graphics::Entity entity)
    	{
                return archetypeManager.remove<T>(entity);	
    	}
    	
    
    
    	// Adds a component to an entity and possibly adds 
    	// a new archetype including the component type set 
            template <typename T>  
           	bool add(Graphics::Entity entity, T component)
            {
                return archetypeManager.add(entity, component); 
    	} 
    
    
    
        private:
            Graphics::ArchetypeManager archetypeManager;
    };

}

#endif
         
