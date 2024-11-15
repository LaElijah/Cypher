#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include "Primitives.h"
#include <vector>
#include <typeindex>
#include <unordered_map>
#include <memory>
#include <iostream>

namespace Graphics 
{
    struct Component
    {
    };
    
    struct RenderComponent : public Component
    {
	RenderComponent() {};

	RenderComponent(Graphics::Model model) : model(model) {};
        Graphics::Model model;
    };
    
    
    class Store
    {
    
    };
    
    
    template <typename T>
    class ComponentStore : public Store 
    {
        public: 
    	std::vector<T>& getAll()
    	{
                return m_Components;	
    	}
    
            T& get(Graphics::Entity entity)
            {
        	//newComponent.x = "HEY";
        	//return static_cast<T&>(newComponent);
                return m_Components[indices[entity]]; 
            }
        
            void add(Graphics::Entity entity, T component)
            {
                indices[entity] = SIZE; 
        	    m_Components.resize(SIZE + 1);
        	    m_Components[SIZE] = std::move(component);
        
        	    SIZE++;
            }
    
    
        private: 
            unsigned int SIZE = 0;
    
            std::vector<T> m_Components;	  
            std::unordered_map<Graphics::Entity, unsigned int> indices;  
    
    };
    
    
    
    class ComponentManager
    {
        public: 
            template <typename T>
    	std::vector<T>& getAll()
    	{
    	    return std::static_pointer_cast<ComponentStore<T>>
    	    (
    	        m_ComponentStores[std::type_index(typeid(T))]
    	    )
    	    ->getAll();
    
    	}
            
    	template <typename T>
            T& get(Graphics::Entity entity)
            {
        	if (!isStore<T>())
        	    return NULL; // Create a new component store
        	else
                return std::static_pointer_cast<ComponentStore<T>>
    	    (
    	        m_ComponentStores[std::type_index(typeid(T))]
    	    )
    	    ->get(entity);
        	    
            }
    
            template <typename T> 
            bool add(Graphics::Entity entity, T component)
            {
                if (!isStore<T>())
        	    m_ComponentStores[std::type_index(typeid(T))] = std::static_pointer_cast<Store>
    	    (
    	        std::make_shared<ComponentStore<T>>()
    	    );
    
        	    std::static_pointer_cast<ComponentStore<T>>
    	    (
    	        m_ComponentStores[std::type_index(typeid(T))]
    	    )
    	    ->add(entity, component);
            
    	    return true;	    
            }
    
        private:
            template <typename T>
            bool isStore()
            {
                return m_ComponentStores.find(std::type_index(typeid(T))) != m_ComponentStores.end();
            }
    
            std::unordered_map<std::type_index, std::shared_ptr<Store>> m_ComponentStores;	    
    };
}

#endif
