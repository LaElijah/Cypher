#ifndef COMPONENT_ITERATORS
#define COMPONENT_ITERATORS


#include <optional> 
#include <memory>
#include <vector>

namespace Graphics
{
    template <typename T>
    struct ContainerIterator
    {
        ContainerIterator(std::shared_ptr<void> data, int size)
        {
            m_DATA = data; 
    	SIZE = size;
        }
    
    
    
        bool next()
        {
    	if (INDEX < SIZE) 
                return true;	
    	else
    	    return false;	
        }
    
        T get()
        {
            T it = std::static_pointer_cast<std::vector<T>>(m_DATA)
    	    ->at(((INDEX == 1) && (SIZE = 1)) ? INDEX - 1 : INDEX);
    
            if (next())
                INDEX = INDEX + 1;	
    
    	return it;
        }
    
        int INDEX = 0;
        unsigned int SIZE;
        std::shared_ptr<void> m_DATA;
    };
    
    
    template <typename T>
    struct WideIterator
    {
        WideIterator(std::vector<ContainerIterator<T>> data)
        {
            m_DATA = data; 
    	SIZE = data.size();
    
        }
    
    
    
        bool next()
        {
    	if (!m_DATA[INDEX].next())
    	    INDEX = INDEX + 1;
    
    	if (INDEX < SIZE) 
                return true;	
    	else
    	    return false;	
    
        }
    /*
        std::optional<T> get()
        {
    	if (INDEX == -1)
    	    return std::nullopt;
    
    	while (!m_DATA[INDEX].next() && (INDEX < SIZE - 1))
    	    INDEX = INDEX + 1;
    
    	if (!m_DATA[INDEX].next())
    	{
    	    return std::nullopt;
    	}
    
            return m_DATA[INDEX].get();
        }
    
    */
        std::optional<T> get()
        {
    	if (INDEX == -1)
    	    return std::nullopt;
    
    	while (!m_DATA[INDEX].next() && (INDEX < SIZE - 1))
    	    INDEX = INDEX + 1;
    
    	if (!m_DATA[INDEX].next())
    	{
    	    return std::nullopt;
    	}
    
            return m_DATA[INDEX].get();
        }
    
        int INDEX = -1;
        unsigned int SIZE;
        std::vector<ContainerIterator<T>> m_DATA;
    
    };
}











#endif
