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
    
            template <typename... T>
            Graphics::PackIterator<T...> getAllContainingMulti()
            {
                return archetypeManager.getAllContainingMulti<T...>(); // was missing ...
            }   
            template <typename T>
            T get(Graphics::Entity entity)
            {
                return archetypeManager.get<T>(entity);
            }
  

	    template <typename ... T>
	    void subscribe(std::function<void(Graphics::Entity)> signalResponse)
	    {
		Graphics::ObjectInfo ID;
		(ID.add<T>(), ...);

		subscribers.emplace(ID.getID().to_string(), signalResponse);
	    }
    
  
	    template <typename ... T>
	    void signal(Graphics::Entity entity)
	    {
		Graphics::ObjectInfo ID;
		(ID.add<T>(), ...);
		for (auto& [key, value] : subscribers)
		{
		    Graphics::OBJECT_ID cID(key);
                    if ((cID & ID.getID()) == ID.getID())
		        value(entity);
		}
	    }
    
    	template <typename T> 
    	bool remove(Graphics::Entity entity)
    	{
		signal<T>(entity);
                return archetypeManager.remove<T>(entity);	
    	}
    	
    
    
    	// Adds a component to an entity and possibly adds 
    	// a new archetype including the component type set 
            template <typename T>  
           	bool add(Graphics::Entity entity, T component)
            {
		signal<T>(entity);
                return archetypeManager.add(entity, component); 
    	} 
    
    
    
        private:
	    static inline std::unordered_map<std::string, std::function<void(Graphics::Entity)>> subscribers;
            Graphics::ArchetypeManager archetypeManager;
    };

}

#endif
         
