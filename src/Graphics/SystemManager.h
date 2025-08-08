#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "EntityManager.h"
#include "ComponentManager.h"
#include "Primitives.h"
#include <map>
#include <optional>
#include "ModelLoader.h"

namespace Graphics
{
	template <typename T>
	class System
	{
	public:
		void update()
		{
			static_cast<T *>(this)->updateImpl();
		}

	protected:
		static inline Graphics::ComponentManager componentManager;
	};

	class RenderSystem : public System<RenderSystem>
	{
	    public:
		void updateImpl
		(
		    std::map
		        <
			    std::string, 
			    Graphics::RenderBatch
			> &RenderBatches
		)
		{
			
		    // TODO: INDEX BATCHES IN AN ARRAY BY SHADER NAME

		    auto it = componentManager
		        .getAllContaining<RenderComponent>();


		    for (std::pair<std::string, Graphics::RenderBatch> pair : RenderBatches)
		    {
		        // Handle what happens when a buffer is changed

			if (pair.second.isChanged())
			    pair.second.reset(); 
		    }


		    while (it.next())
		    {
		    	std::optional<RenderComponent> isComponent = it.get();

		    	if (!isComponent.has_value())
		    		continue;

		    	RenderComponent component = isComponent.value();
		    	// Prior to handling, sort by shader then 
			// organize each shader group by texture



		    	for (Graphics::Mesh mesh : component.model.meshes)
		    	{
		    		if (RenderBatches.find(mesh.shader) == RenderBatches.end() )
		    		{
		    		    RenderBatches
				        .emplace
					(
					    mesh.shader, 
					    Graphics::RenderBatch()
					);
		    		}

		    		Graphics::RenderBatch& batch = RenderBatches[mesh.shader];

		    		if (!batch.isChanged())
		    		    continue;

		    		batch.insert(mesh);
					std::cout << "SHADER: " << shaderCounts[mesh.shader] << std::endl;
		    		shaderCounts[mesh.shader]++;
		    	}
		    }



		    for 
		    (
		        std::pair
		        <std::string, unsigned int> pair 
		        : shaderCounts
		    )
		        RenderBatches[pair.first].finish();




		}	
			

		void loadModel(Entity entity, Graphics::ModelInfo &info);
        std::unordered_map<std::string, unsigned int> shaderCounts;
		Graphics::ModelLoader<AssimpImporter> modelLoader;
	};

	class SystemManager
	{
	public:
		void update();

		Entity createModel(Graphics::ModelInfo &info);
		std::vector<Graphics::RenderBatch> &getRenderBatches(std::vector<Graphics::RenderBatch> &values)
		{
			values.clear();

			for (const auto &pair : m_RenderBatches)
			{
				values.push_back(pair.second);
			}
			return values;
		}

	private:
		Graphics::EntityManager entityManager;
		Graphics::RenderSystem renderSystem;
		std::map<std::string, Graphics::RenderBatch> m_RenderBatches;
	};

}

#endif
