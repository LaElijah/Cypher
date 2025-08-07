#ifndef OBJECT_INFO_H
#define OBJECT_INFO_H


#include <map> 
#include <set>
#include <typeindex>
//include <memory> 

#include <iostream>

#include "Primitives.h"

namespace Graphics
{
    /**
     * Object Info defines the id and component make up 
     *
     *
     */
    class ObjectInfo
    {
        public: 
    	ObjectInfo()
    	{
    	
    	}
    
    	ObjectInfo(ObjectInfo& info)
    	{
                copy(info);	
    	}
    
    	auto getID()
    	{
                return ID;	
    	}
    
    
    	template <typename T> 
    	Graphics::OBJECT_ID static getType()
    	{
                return 
    	    (
    	        Graphics::OBJECT_ID()
    		    .flip
    		    (
    		        m_LOADED_TYPES.at(std::type_index(typeid(T))
    			    .name())
    		    )
    	    );	
    	}
           
    
    
    	Graphics::OBJECT_ID static getType(std::string type)
    	{
                return 
    	    (
    	        Graphics::OBJECT_ID()
    		    .flip(m_LOADED_TYPES.at(type))
    	    );	
    	}
            
    
    	auto getTypes()
    	{
                return m_SUPPORTED_TYPES;	
    	}
    
    
    	bool isZero()
    	{
                return ID.none();	
    	}
    
    
    
    	bool contains(std::string type)
    	{
                return (m_SUPPORTED_TYPES.count(type) != 0);	
    	}
    
    	template <typename T> 
    	bool contains()
    	{
                return (
    		       m_SUPPORTED_TYPES.count
    		       (
    		           std::type_index(typeid(T)).name()
    		       ) != 0
    		   );	
    	}
    
    
    
    
    
    	template <typename ... T>
    	Graphics::OBJECT_ID& add()
    	{
                (insertType<T>(), ...);	
    	    return ID;
    	}
    
    
    	template <typename ... T>
    	Graphics::OBJECT_ID& remove()
    	{
                (removeType<T>(), ...);	
    	    return ID;
    	}
    
    	void remove(std::string type)
    	{
                ID.flip(m_LOADED_TYPES.at(type)); 
    	}
    
    
    
            template <typename T>	
    	static unsigned int getSize()
    	{
                return m_TYPE_SIZES.at(std::type_index(typeid(T)).name());	
    	}
    
           
    	static unsigned int getSize(std::string type)
    	{
                return m_TYPE_SIZES.at(type);	
    	}
    
    
    	// debug
    	void print()
    	{
                std::cout << ID.to_string() << std::endl;	
    	}
    
        private:
    
            template <typename T>
    	void insertType()
    	{
                std::string type = std::type_index(typeid(T)).name();
    
                if (m_LOADED_TYPES.count(type) == 0)
    	    {
    	        m_LOADED_TYPES.insert(std::make_pair(type, COUNT));
    	        m_TYPE_SIZES.insert(std::make_pair(type, sizeof(T)));
    		COUNT++;
    	    }
    
    	    if (m_SUPPORTED_TYPES.find(type) == m_SUPPORTED_TYPES.end())
    	    {
    		ID.flip(m_LOADED_TYPES.at(type)); 
    	        m_SUPPORTED_TYPES.insert(type);	
    	    }
    	}
    
    
    	template <typename T>
            void removeType()
    	{
                std::string type = std::type_index(typeid(T)).name();
                
    	    if (m_SUPPORTED_TYPES.find(type) != m_SUPPORTED_TYPES.end())
    	    {
    	        ID.flip(m_LOADED_TYPES.at(type));	    
    		m_SUPPORTED_TYPES.erase(type);
    	    }
    	}
    
    
    	/**
    	 * For copy constructor 
    	 *
    	 */
    	void copy(ObjectInfo& info)
    	{
                m_SUPPORTED_TYPES = info.getTypes();	
    	    ID = info.getID();
    	}
    
    
    	Graphics::OBJECT_ID ID;
    	std::set<std::string> m_SUPPORTED_TYPES;
    	
    	static inline std::map<std::string, unsigned int> m_LOADED_TYPES;
    	static inline std::map<std::string, unsigned int> m_TYPE_SIZES;
    
    	static inline unsigned int COUNT = 0;
    };
}



#endif
