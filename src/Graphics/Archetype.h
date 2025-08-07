#ifndef ARCHETYPE_H
#define ARCHETYPE_H

#include "ObjectInfo.h"
#include "Container.h"
// #include <iostream>
#include <typeindex>
// #include <any>
#include <unordered_map>
#include "Primitives.h"

namespace Graphics
{
    class Archetype
    {
        public:
    
            Archetype(ObjectInfo& info)
            {
    	    ID = info;
    	    for (std::string type : ID.getTypes())
    	    {
    		CONTAINERS[type] = new VoidContainer(); 
    		// Creates containers
    	    }
            }
    
            ////////////////////////////////////////////////////////////
    	////////////////////////// EXTERNAL ////////////////////////
            ////////////////////////////////////////////////////////////
    	
    
    	ObjectInfo& getInfo()
    	{
                return ID;	
    	}
    
    	int getIndex(Graphics::Entity entity)
    	{
    	    if (ENTITY_INDEX.count(entity) == 0)
    	        return -1;
    
                return ENTITY_INDEX[entity];	
    	}
    
    	
    	template <typename T> 
    	ContainerIterator<T> getIterator()
    	{
                return (CONTAINERS[std::type_index(typeid(T)).name()])
    	        ->getIterator<T>();	
    	}
    
    	template <typename T>
    	T get(Graphics::Entity entity)
    	{
    	    return CONTAINERS.at(std::type_index(typeid(T)).name())
    	        ->get<T>(ENTITY_INDEX.at(entity));
    	}
    
    	void getVoid
    	(
    	    std::vector<std::byte>* byteVector, 
    	    std::string type,
    	    unsigned int byteSize,
    	    unsigned int index
    	)
    	{
                CONTAINERS[type]
    	        ->getVoid
    		(
    		    byteVector,
    		    byteSize,
    		    index
    		);
    	}
    
    
    
            ////////////////////////////////////////////////////////////
    	////////////////////////// INTERNAL ////////////////////////
            ////////////////////////////////////////////////////////////
    
    
    	template <typename T>
    	unsigned int addVoid(Graphics::Entity entity, T component)
    	{
    	    std::vector<T>* data = new std::vector<T>();
    	    data->push_back(component); 
    
    	    int index = CONTAINERS[std::type_index(typeid(T)).name()]
    	        ->addVoid(static_cast<std::vector<std::byte>*>(data));	
    
    	    delete data;   
    
                ENTITY_INDEX.emplace(entity, index);
                INDEX_MAP.emplace(index, entity);
    
    
    	    return index; 
    	}
    
    
    	unsigned int addVoid
    	(
    	    Graphics::Entity entity, 
    	    std::vector<std::byte>* data, 
    	    std::string type
    	)
    	{
    	    int index = CONTAINERS[type]
    	        ->addVoid(data);	
    
                ENTITY_INDEX.emplace(entity, index);
                INDEX_MAP.emplace(index, entity);
    	    
    	    return index; 
    	}
    
    	// TODO: Check if type is supported 
    	// TODO: Change to only allow a vector of components
    	// that matches the supported components
    	template <typename T> 
            void add(unsigned int entity, T component)
    	{
    	    unsigned int index = CONTAINERS
    	        [std::type_index(typeid(T)).name()]
    		    ->add(component);
    
                ENTITY_INDEX.emplace(entity, index);
                INDEX_MAP.emplace(index, entity);
    
    	}	
    
    	bool remove(Graphics::Entity entity, ObjectInfo& entityInfo)
    	{
    	    int lastPosition;
    	    int index = ENTITY_INDEX[entity]; 
    
                for (std::string type : entityInfo.getTypes())
    		lastPosition = CONTAINERS[type]
    		    ->remove
    		    (
    		        index, 
    		        ObjectInfo::getSize(type)
    		    ); 
     
    	    
    	    if (lastPosition == -2)
    	        return false;
    
    	    else if (lastPosition <= 0)
    	    {
    		INDEX_MAP.erase(index);
    		ENTITY_INDEX.erase(entity);
    	    } 
    	    else
                {
    		Graphics::Entity lastEntity = INDEX_MAP[lastPosition];
    		ENTITY_INDEX[lastEntity] = index;
    		INDEX_MAP[index] = lastEntity;
    		INDEX_MAP.erase(lastPosition);
                }
    
    	    ENTITY_INDEX.erase(entity);
    /*
    	    INDEX_MAP.erase(index);
    */	    
    	    return true;
    	}
    
     
    	template <typename T>
    	bool replace(Graphics::Entity entity, T component)
    	{
                CONTAINERS[std::type_index(typeid(T)).name()]
    		    ->replace(ENTITY_INDEX[entity], component);
    
    	    return true;
    	}
    
    
    	
    
    	void linkAdd
    	(
    	    std::bitset<256> id, 
    	    std::shared_ptr<Archetype> archetype
    	)
    	{
               addEdge[id] = archetype; 	
    	}
        
    
    	void linkRemove
    	(
    	    Graphics::OBJECT_ID id, 
    	    std::shared_ptr<Archetype> archetype
    	)
    	{
               removeEdge[id] = archetype; 	
    	}
        
        private:
    	// ID
    	ObjectInfo ID;
    
    	// INTERNAL RELATIONSHIP
    	std::map<std::string, VoidContainer*> CONTAINERS;
    	std::map<Graphics::Entity, unsigned int> ENTITY_INDEX;
    	std::map<unsigned int, Graphics::Entity> INDEX_MAP;
    
    	// THIS NODE RELATIONSHIPS
            std::unordered_map<Graphics::OBJECT_ID, std::shared_ptr<Archetype>> addEdge;
            std::unordered_map<Graphics::OBJECT_ID, std::shared_ptr<Archetype>> removeEdge;
    
    };
}
#endif




	///////////////////EXTERNAL/////////////////
/*
    	    Archetype(std::bitset<256> parentCode, std::bitset<256> childCode)
	{

// if a bitset of a xor between componentCode 
// and the old archetype is zero/none, that means all components 
// in component code is different, which means its an acceptable 
// new archetype 
//

	   
	    	

	    auto parent = ARCHETYPES[parentCode];

            ID = ComponentCode(parentCode || childCode);	
		   
	    for (std::string type : ID.getSupportedTypes())
	        CONTAINERS[type] = new TypedContainer();

	    remove.insert(parentCode, parent);
	    //parent->add.insert(ARCHETYPES[ID.getCode()],  )
	}



	template <typename T>
	Archetype addType()
	{
            ComponentCode code();
            code.add<T>(); 

	    add.insert(std::make_shared<Archetype>(Archetype(ID.getCode(), code.getCode()));
	
	}

	*/



/*
	// INTERNAL
	template <typename T>
	void remove(unsigned int entity)
	{
	    std::cout << "ENTITY INDEX: " << ENTITY_INDEX[entity] << std::endl;
	    int lastPosition = CONTAINERS
	        [std::type_index(typeid(T)).name()]
	            ->remove<T>(ENTITY_INDEX[entity]);

	    if (lastPosition > -1)
	    {
	        ENTITY_INDEX
		[INDEX_MAP[lastPosition]] = ENTITY_INDEX[entity];

		ENTITY_INDEX.erase(entity);
	    
	    }
	}
*/


