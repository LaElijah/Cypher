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
		void updateImpl(std::map<std::string, Graphics::RenderBatch> &RenderBatches)
		{
			/*
						 RenderComponent component;
						 component.x = "HI";
							 componentManager.add<RenderComponent>(0, component);

						 auto test = componentManager.get<RenderComponent>(0); // Get a reference
							 std::cout << test.x << std::endl;
		 */
			// TODO: INDEX BATCHES IN AN ARRAY BY SHADER NAME

			auto it = componentManager.getAllContaining<RenderComponent>();


			for (std::pair<std::string, Graphics::RenderBatch> pair : RenderBatches)
			{


			}





			while (it.next())
			{

				
				std::optional<RenderComponent> isComponent = it.get();

				if (!isComponent.has_value())
					continue;

				RenderComponent component = isComponent.value();
				// Prior to handling, sort by shader then organize each shader group by texture

				bool shaderFound;
				for (Graphics::Mesh mesh : component.model.meshes)
				{
					if (batch.isChanged == false)

					if (RenderBatches.find(mesh.shader) == RenderBatches.end() && !shaderFound)
					{
					    RenderBatches.emplace(mesh.shader, Graphics::RenderBatch());
						shaderFound = true;
					}

					Graphics::RenderBatch& batch = RenderBatches[mesh.shader];

					/*
					if (workingShader != mesh.shader)
					{
						workingShader = mesh.shader;
					}

					*/

					//batch.reset();

					batch.vertexData.insert(
						batch.vertexData.end(),
						mesh.vertices.begin(),
						mesh.vertices.end());

					batch.indexData.insert(
						batch.indexData.end(),
						mesh.indices.begin(),
						mesh.indices.end());

					batch.counts.push_back(mesh.vertices.size());
					batch.indexCounts.push_back(mesh.indices.size());
					batch.textureInfo["diffuse"]
						.insert(
							batch.textureInfo["diffuse"].end(),
							mesh.textureInfo.begin(),
							mesh.textureInfo.end());

					batch.shader = mesh.shader;
					shaderCounts[mesh.shader]++;
				}
			}

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
