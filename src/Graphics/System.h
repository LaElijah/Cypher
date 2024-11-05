#ifndef SYSTEM_H
#define SYSTEM_H


template <typename T>
class System
{
    public:
        void update()
        {
            static_cast<T*>(this)->updateImpl();	
        }	

    
    protected:
        ComponentManager componentManager;
        // Add the component manager
};


class RenderSystem : public System<RenderSystem>
{
    public: 
        void updateImpl()
        {

    	    RenderComponent component;
    	    component.x = "HI";
	    componentManager.add<RenderComponent>(0, component);
              
    	    auto test = componentManager.get<RenderComponent>(0); // Get a reference
            std::cout << test.x << std::endl;
        }
};


#endif
