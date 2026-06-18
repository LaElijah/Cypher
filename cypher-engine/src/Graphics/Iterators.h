#ifndef COMPONENT_ITERATORS
#define COMPONENT_ITERATORS

#include <optional>
#include <memory>
#include <vector>
#include <iostream> 

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
            if (SIZE == 0)
                return false;


            if (INDEX < SIZE)
                return true;
            else
                return false;
        }

        T get()
        {
            std::cout << "INDEX GET: " << INDEX << std::endl;

            T it = static_cast<std::vector<T>*>(m_DATA.get())
                       ->at(INDEX);



            if (next())
                INDEX++;

                //std::cout << it.model.info.path << " INDEX: " << INDEX << std::endl;
            return it;
        }

        int INDEX = 0;
        unsigned int SIZE = 0;
        std::shared_ptr<void> m_DATA;
    };

template <typename T> 
class WideIterator
{
    public:
        WideIterator
        (
            std::vector<std::shared_ptr<void>> data, 
            std::vector<unsigned int> sizes
        )
        {
            m_DATA = data;
            m_SIZES = sizes;

            SIZE = sizes.size();

            
            INDEX = 0;
            POINTER_INDEX = 0;	

        }

        std::optional<T> get()
        {
            if (SIZE == 0)
                return std::nullopt;

            T it = static_cast<std::vector<T>*>(m_DATA.at(INDEX).get())
                ->at(POINTER_INDEX);



            // If end is reached and there is a next pointer
            // swap to it
            if (next() && ((POINTER_INDEX + 1) >= m_SIZES.at(INDEX)))
            {
                POINTER_INDEX = -1;
                INDEX++;	    
            }

            POINTER_INDEX++;
            return it;
        }

        // CASES: NEXT, SAME INDEX
        // CASES: NEXT, LAST POINTER_INDEX, CHANGE INDEX
        // CASES: NO NEXT, LAST POINTER_INDEX, LAST INDEX
        bool next()
        {
            if (SIZE == 0)
                return false;
            else if (INDEX == (SIZE - 1))
                if (POINTER_INDEX < m_SIZES.at(INDEX))
                    return true;
            else if (INDEX < (SIZE - 1))
                return true;
            
            
            return false;


        }

    private:
        std::vector<std::shared_ptr<void>> m_DATA;
        std::vector<unsigned int> m_SIZES;

        unsigned int INDEX;
        unsigned int SIZE;

        int POINTER_INDEX;

};

template <typename>
using VoidPtr = std::shared_ptr<void>;

template <typename... T>
class PackIterator
{
public:
    // One entry per archetype: a tuple of pointers, one per component type
    using DataTuple = std::tuple<VoidPtr<T>...>;

    PackIterator(
        std::vector<DataTuple>       data,
        std::vector<unsigned int>    sizes)
        : m_DATA(std::move(data))
        , m_SIZES(std::move(sizes))
        , SIZE(m_SIZES.size())
    {}

    bool next() const
    {
        if (SIZE == 0 || INDEX >= SIZE)
            return false;
        if (INDEX == SIZE - 1)
            return POINTER_INDEX < static_cast<int>(m_SIZES[INDEX]);
        return true;
    }

    // Returns references into the live contiguous arrays — zero copy.
    // std::optional cannot hold references directly, so we use a pointer
    // sentinel: nullopt when exhausted, tuple of refs otherwise.
    std::optional<std::tuple<T&...>> get()
    {
        if (!next())
            return std::nullopt;

        auto result = fetchComponents(std::index_sequence_for<T...>{});

        // Advance; move to next archetype when current one is exhausted
        if (POINTER_INDEX + 1 >= static_cast<int>(m_SIZES[INDEX]))
        {
            POINTER_INDEX = 0;
            ++INDEX;
        }
        else
        {
            ++POINTER_INDEX;
        }

        return result;
    }

private:
    // Index into each type's vector at POINTER_INDEX using pack expansion.
    // std::tie returns a tuple of lvalue references — no copies.
    template <std::size_t... Is>
    std::tuple<T&...> fetchComponents(std::index_sequence<Is...>)
    {
        return std::tie(
            static_cast<std::vector<T>*>(
                std::get<Is>(m_DATA[INDEX]).get()
            )->at(POINTER_INDEX)...
        );
    }

    std::vector<DataTuple>      m_DATA;
    std::vector<unsigned int>   m_SIZES;

    unsigned int  SIZE          = 0;
    unsigned int  INDEX         = 0;
    int           POINTER_INDEX = 0;
};

    /*
    template <typename T>
    struct WideIterator
    {
        WideIterator(std::vector<std::shared_ptr<void>> data)
        {
            for (std::shared_ptr<void> pointer : data)
                m_DATA.push_back(ContainerIterator<T>(pointer, data.size()));

            SIZE = data.size();

            std::cout << "WIDE SIZE: " << SIZE << std::endl;
        }

        bool next()
        {

            if (SIZE == 0)
            {
                std::cout << "NEXT VALUE: FALSE SIZE ZERO" << std::endl;
                return false;
            } 
            std::cout << "PRE NEXT: " << SIZE << std::endl;
            if (!m_DATA[INDEX].next())
                INDEX = INDEX + 1;

            if (INDEX < SIZE)
            {

                std::cout << "NEXT VALUE: TRUE, SIZE: " << SIZE << std::endl;
                return true;
            }
            else
            {
                std::cout << "NEXT VALUE: FALSE, SIZE: " << SIZE << " INDEX: " << INDEX << std::endl;
                return false;
            }


            
        }
        
       //     std::optional<T> get()
       //     {
       //     if (INDEX == -1)
       //         return std::nullopt;

       //     while (!m_DATA[INDEX].next() && (INDEX < SIZE - 1))
       //         INDEX = INDEX + 1;

       //     if (!m_DATA[INDEX].next())
       //     {
       //         return std::nullopt;
       //     }

       //         return m_DATA[INDEX].get();
       //     }

        
        std::optional<T> get()
        {

            std::cout << "WIDE GET" << std::endl;

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
    */
}

#endif
