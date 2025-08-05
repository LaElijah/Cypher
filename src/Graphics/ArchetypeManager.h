#ifndef ARCHETYPE_MANAGER_H
#define ARCHETYPE_MANAGER_H


#include <unordered_map>
#include <typeindex>
#include "Iterators.h"
#include "Primitives.h"
#include "Archetype.h"

namespace Graphics
{
    class ArchetypeManager
    {
        public:
            template <typename T>
            T get(Graphics::Entity entity)
            {
                return ARCHETYPES[ENTITY_TYPES[entity].getID()]
    	        ->template get<T>(entity);
            }
    
    
    
    	template <typename T>
    	Graphics::ContainerIterator<T> getAll()
    	{
                return ARCHETYPES[Graphics::ObjectInfo::getType<T>()]->template getIterator<T>(); 
    	}
    
    	template <typename T>
    	Graphics::WideIterator<T> getAllContaining()
    	{
            Graphics::ObjectInfo info;
    	    info.add<T>();	    
    
    	    std::vector<ContainerIterator<T>> iterators;
    
    	    for 
	    (
	        std::pair
		<
		    Graphics::OBJECT_ID, 
		    std::shared_ptr<Graphics::Archetype>
		> pair : ARCHETYPES
	    )
    	    {
    		auto archetype = pair.second;
    		if (archetype->getInfo().contains<T>())
    	            iterators.push_back
    		    (
    		        archetype-> template getIterator<T>()
    		    );
    	    }
    	    return WideIterator(iterators);
    	}
    
            template <typename T> 
            bool add(Graphics::Entity entity, T component)
            {
    	    
                if (!ENTITY_TYPES.count(entity))
                    ENTITY_TYPES[entity] = Graphics::ObjectInfo();
    
		Graphics::ObjectInfo& entityInfo = ENTITY_TYPES[entity];
                Graphics::ObjectInfo archetypeInfo(entityInfo);
                archetypeInfo.add<T>();
    
    	    int index;
    
    	    Graphics::OBJECT_ID entityID = entityInfo.getID();
            Graphics::OBJECT_ID archetypeID = archetypeInfo.getID();
                
                if 
                (
                    archetypeID != entityID 
                    && ARCHETYPES.count(archetypeID) == 0
                )
                    ARCHETYPES.emplace
                    (
                        archetypeID, 
                        std::make_shared<Graphics::Archetype>(archetypeInfo)
                    );
      
                if (!entityInfo.contains(std::type_index(typeid(T)).name()))
    	    {
                    if (!entityInfo.isZero()) // is this a new entity? 
                    {
    
    		    // Cleanup 
                        std::vector<std::byte>* byteVector = new 
                            std::vector<std::byte>(); // storage for component copy
    
                        for(std::string type : entityInfo.getTypes())
                        {
    
                            // Grab each component and store in byte vector
                            ARCHETYPES[entityID]->getVoid
                    	    (
                    	        byteVector,
    	            	        type,
                    	        ObjectInfo::getSize(type),
                                    ARCHETYPES[entityID]
                    	            ->getIndex(entity)
                    	    );
    
                            // Store void data into archetype container
                            index = ARCHETYPES[archetypeID]->addVoid
                    	    (
                    	        entity, 
                                    byteVector,
    	            	        type
                    	    );
    
                    	    // Clear the vector
                            byteVector->clear();	    
                        }
                        delete byteVector;
                        // Done with cleanup
    		    
                        ARCHETYPES[entityID]
                            ->remove(entity, ENTITY_TYPES[entity]);
                         
                        ARCHETYPES[entityID]
                            ->linkAdd(archetypeID, ARCHETYPES[archetypeID]);
    
                        ARCHETYPES[archetypeID]
                            ->linkRemove(entityID, ARCHETYPES[entityID]);
    	        }
    
    	        ARCHETYPES[archetypeID]->add(entity, component);
    
    	        entityInfo.add<T>();
    	    }
    	    else
    	        ARCHETYPES[archetypeID]->replace(entity, component);
    	        
    	    return index;
            };
    
    
    
    	template <typename T>
            bool remove(Graphics::Entity entity)
            {
                if (!ENTITY_TYPES.count(entity))
                    return false;
                
                Graphics::ObjectInfo& entityInfo = ENTITY_TYPES[entity];
                Graphics::ObjectInfo archetypeInfo(entityInfo);
                archetypeInfo.remove<T>();
                
    		
    	    Graphics::OBJECT_ID entityID = entityInfo.getID();
    	    Graphics::OBJECT_ID archetypeID = archetypeInfo.getID();
                
    	    std::string removedType = std::type_index(typeid(T)).name();
    
                if (archetypeInfo.isZero() && !entityInfo.isZero())
    	    {
                    //ARCHETYPES[entityID]->remove(entity, ENTITY_TYPES[entity]);
                    ARCHETYPES[entityID]->remove(entity, entityInfo);
    		entityInfo.remove<T>();
    	        return false;
    	    }
    
                if 
                (
                    archetypeInfo.getID() != entityInfo.getID()
                    && ARCHETYPES.count(archetypeID) == 0
                )
                    ARCHETYPES.emplace
                    (
                        archetypeID, 
                        std::make_shared<Graphics::Archetype>(archetypeInfo)
                    );
       
                if (!entityInfo.isZero()) // does entity have component(s)
                {
                    std::vector<std::byte>* byteVector = new 
                        std::vector<std::byte>(); // storage for component copy
    
                    for(std::string type : entityInfo.getTypes())
                    {
                	    if (type != removedType)
                	    {
                            // Grab each component and store in byte vector
                            ARCHETYPES[entityID]->getVoid
                	        (
                	            byteVector,
    			    type,
                	            ObjectInfo::getSize(type),
                                ARCHETYPES[entityID]
                	                ->getIndex(entity)
                	        );
    
    
                            // Store void data into archetype container
                            ARCHETYPES[archetypeID]
    			    ->addVoid
    			    (
    			        entity,
    			        byteVector,
    				type
    			    );
    
                	        // Clear the vector
                            byteVector->clear();	    
                	    }
                    } // for each end
    
                    delete byteVector;
    
                    ARCHETYPES[entityID]
    		    ->remove
    		    (
    		        entity,
    			ENTITY_TYPES[entity]
    		    );
                     
                    ARCHETYPES[archetypeID]
                        ->linkAdd(entityID, ARCHETYPES[entityID]);
    
                    ARCHETYPES[entityID]
                        ->linkRemove(archetypeID, ARCHETYPES[archetypeID]);
                }
    	    else 
    	        return false;
            	    
                entityInfo.remove<T>();
                return true;
            }
    
            void trash(Graphics::Entity entity)
            {
                 Graphics::ObjectInfo& entityInfo = ENTITY_TYPES[entity];
                 ARCHETYPES[entityInfo.getID()]
                    ->remove(entity, entityInfo);
            }
    
    
    
    
        private:
            static inline std::unordered_map<Graphics::OBJECT_ID, std::shared_ptr<Graphics::Archetype>> ARCHETYPES;
            static inline std::unordered_map<Graphics::Entity, Graphics::ObjectInfo> ENTITY_TYPES;
    
    };
}
#endif
