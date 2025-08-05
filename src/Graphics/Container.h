#ifndef VOID_CONTAINER_H
#define VOID_CONTAINER_H

#include <memory>
#include <vector>
#include <cstring>
#include "Iterators.h"


namespace Graphics
{
    class VoidContainer 
    {
        public:
    	VoidContainer()
    	{
                INITIALIZED = false;	
    	}
    
    	template <typename T>
    	T get(unsigned int index)
    	{
    	    initialize<T>();
    	    return static_cast<std::vector<T>*>(m_DATA.get())
    	        ->at(index);
    	}
    
    	void getVoid
    	(
    	    std::vector<std::byte>* vector,
    	    unsigned int byteSize, 
    	    unsigned int index
    	)
    	{
    	    for (int i = 0; i < byteSize; i++)
    	        vector->push_back
    		(
    		    static_cast<std::vector<std::byte>*>(m_DATA.get())
    		        ->at((byteSize * index) + i)
    		);
    	}
    
    	template <typename T> 
    	ContainerIterator<T> getIterator()
    	{
                return ContainerIterator<T>(m_DATA, CONTAINER_INDEX); 	
    	}
    
    	unsigned int getSize()
    	{
                return CONTAINER_INDEX;	
    	}
    
    	std::shared_ptr<void> getData()
    	{
                return m_DATA;	
    	}
    
    
    
    
    	template <typename T>
    	std::shared_ptr<std::vector<T>> getAll()
    	{
    	    if (INITIALIZED != true)
    	        return NULL;
    
                return std::static_pointer_cast<std::vector<T>>(m_DATA);	
    	}
    
    	template <typename T>
            unsigned int add(T& element)
            {
    	    if (INITIALIZED != true)
                    initialize<T>();
    
    	    std::vector<T>* vector = static_cast<std::vector<T>*>
    	        (m_DATA.get());
    	    
    	    vector->push_back(element);
    	    CONTAINER_INDEX++;
    
    	    return CONTAINER_INDEX - 1;
    	}
    
    	/*
    	template <typename T>
            unsigned int add(T&& element)
            {
    	    if (INITIALIZED != true)
                    initialize<T>();
    
    	    std::vector<T>* vector = static_cast<std::vector<T>*>
    		(m_DATA.get());
    	    
    	    vector->push_back(element);
    	    CONTAINER_INDEX++;
    
    	    return CONTAINER_INDEX - 1;
    	}
    	*/
    	int addVoid(std::vector<std::byte>* byteVector)
    	{
                if (INITIALIZED != true)
                    initialize<std::byte>();
    
    
    	    std::vector<std::byte>* vector = static_cast
    	        <std::vector<std::byte>*>
    		    (m_DATA.get());
    
    
    	    auto it = byteVector->begin();
                while (it != byteVector->end())
                {
    	        vector->push_back(*it);	  
    	        it++;	
    	    } 
     
    	    CONTAINER_INDEX++;
    	    return CONTAINER_INDEX - 1;
    	}
    
    
    	int remove(unsigned int index, unsigned int byteSize)
    	{
    	    auto vector = static_cast
    	        <std::vector<std::byte>*>
    		    (m_DATA.get());
    	   
    	    // Change INDEX to CONTAINER_INDEX 
    	    if (index < 0 || CONTAINER_INDEX <= index)
    	        return -2;
    
    	    // is it both not the first and not the last?
    	    if (CONTAINER_INDEX > 1 && index != (CONTAINER_INDEX - 1))
    	    {
    	        std::byte* startingByte = static_cast
    		    <std::vector<std::byte>*>(m_DATA.get())
    		        ->data() + (index * byteSize);
    
    	        std::byte* lastStartingByte = static_cast
    		    <std::vector<std::byte>*>(m_DATA.get())
    		        ->data() + ((CONTAINER_INDEX - 1) * byteSize);
    		
    		memcpy
    	        (
    	            static_cast<void*>(startingByte), 
    	            static_cast<void*>(lastStartingByte),
    	            byteSize
    	        );	
    
    	        for (int i = 0; i < byteSize; i++)	
    	            vector->pop_back(); 
                }
    	    else
    	        for (int i = 0; i < byteSize; i++)	
    	            vector->pop_back(); 
    
    	    CONTAINER_INDEX--;
    
    	    if (CONTAINER_INDEX == 0)
    	        return 0;
    
                if (index == CONTAINER_INDEX)
    	        return -1; 
    	    
    	    return CONTAINER_INDEX;
    	}
    
    	template <typename T>
    	bool replace(unsigned int index, T& element)
    	{
    	    if (index >= CONTAINER_INDEX)
    	        return false;
    
    	    std::vector<T>* vector = static_cast<std::vector<T>*>
    	        (m_DATA.get());
    	    
                vector->at(index) = element;	    
    	    return true;
    	}
    
    
    
        private: 
    	template <typename T>
    	void initialize()
    	{
    	    if (INITIALIZED != true)
    	    {
                    m_DATA = std::make_shared<std::vector<T>>(); 
    	        INITIALIZED = true;	
    	    }
    	}
    
    	int CONTAINER_INDEX = 0;
    	bool INITIALIZED;
    
            std::shared_ptr<void> m_DATA;
    };
}

#endif


/*
	template <typename T>
	int remove(unsigned int index)
	{
	    auto vector = static_cast<std::vector<T>*>(DATA.get());

	    if (INDEX <= 0 || INDEX <= index)
	        return -1;

	    if (INDEX > 1 && index != (INDEX - 1))
	    {
	        (*vector)[index] = (*vector)[INDEX - 1]; 
	        vector->pop_back(); 
            }
	    else
	        vector->pop_back(); 

	    INDEX--; 
	    return INDEX;
	}
*/


/*
template <typename T>
class Container
{
    public: 
        template <typename U>
        void add(U&& component)
	{
            static_cast<T*>(this)->addImpl(component);	
	}
	
        template <typename U>
	void add(U& component)
	{
            static_cast<T*>(this)->addImpl(component);	
	}


        template <typename U>
        auto& get(unsigned int index)
	{
            return static_cast<T*>(this)->template getImpl<U>(index);	
	}

        template <typename U>
	int remove(int index)
	{
            return static_cast<T*>(this)->template removeImpl<U>(index);	
	}


        template <typename U>
	auto getAll()
	{
            return static_cast<T*>(this)->template getAllImpl<U>();	
	}


	template <typename U> 
        auto getIterator()
	{
            return static_cast<T*>(this)->template getIteratorImpl<U>();	
	}



};
*/


