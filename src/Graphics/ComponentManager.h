#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H


struct Component
{
};

struct RenderComponent : public Component
{
    Graphics::Model* model;
};


class Store
{

};


template <typename T>
class ComponentStore : public Store 
{
    public: 
        T& get(Entity entity)
        {
    	//newComponent.x = "HEY";
    	//return static_cast<T&>(newComponent);
            return m_Components[indices[entity]]; 
        }
    
        void add(Entity entity, T component)
        {
            indices[entity] = SIZE; 
    	m_Components.resize(SIZE + 1);
    	m_Components[SIZE] = std::move(component);
    
    	SIZE++;
        }


    private: 
        unsigned int SIZE = 0;

        std::vector<T> m_Components;	  
        std::unordered_map<Entity, unsigned int> indices;  

};



class ComponentManager
{
    public: 
        template <typename T>
        T& get(Entity entity)
        {
    	if (!isStore<T>())
    	    //return NULL;
    	else
            return std::static_pointer_cast<ComponentStore<T>>
	    (
	        m_ComponentStores[std::type_index(typeid(T))]
	    )
	    ->get(entity);
    	    
        }

        template <typename T> 
        bool add(Entity entity, T component)
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


#endif
