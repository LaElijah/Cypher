#ifndef SYSTEM_MANAGER_H
#define SYSTEM_MANAGER_H

#include "EntityManager.h"
#include "ComponentManager.h"
#include "Primitives.h"
#include <map>
#include <optional>
#include "ModelLoader.h"
#include "SceneNode.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

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
		RenderSystem()
		{
			sceneGraph.emplace("ROOT", std::make_shared<Graphics::SceneNode>());
		}

		std::unordered_map<std::string, Graphics::RenderBatch> createdBatches;
		void createBatch(
			std::string shader,
			std::map<
				std::string,
				Graphics::RenderBatch> &RenderBatches)
		{
			if (RenderBatches.find(shader) == RenderBatches.end())
				RenderBatches.emplace(shader, Graphics::RenderBatch());
		}



		void updateImpl(
			std::map<
				std::string,
				Graphics::RenderBatch> &RenderBatches)
		{

			// TODO: this currently sets rendering to the scene manager, this 
			// needs to be done in a linear fashion.

			sceneGraph.at("ROOT")->dirty=true;
			std::unordered_map<std::string, unsigned int> shaderCounts;
			// TODO: INDEX BATCHES IN AN ARRAY BY SHADER NAME

			// auto it = componentManager
			//    .getAllContaining<RenderComponent>();

			for (std::pair<std::string, Graphics::RenderBatch> pair : RenderBatches)
			{
				// Handle what happens when a buffer is changed
				if (pair.second.isChanged())
					pair.second.reset();
			}

			std::function<
				bool(
					std::shared_ptr<Graphics::SceneNode> node,
					glm::mat4 & parentTransform)>
				handleNode = [this, &RenderBatches, &shaderCounts](
								 std::shared_ptr<Graphics::SceneNode> node,
								 glm::mat4 &parentTransform) mutable
			{

				Graphics::Entity entity = node->ENTITY;
				std::cout << "ENTITY: " << entity << std::endl;

				auto renderable = componentManager.get<Graphics::Renderable>(entity);
				auto transform = componentManager.get<Graphics::Transform>(entity);

				node->globalTransform = glm::translate(glm::mat4(1.0f), transform.position) * parentTransform;


				// Start of render duties
				Graphics::ModelInfo info;
				info.path = renderable.path;

				std::cout << "Entity: " << node->ENTITY << std::endl;
				auto model = modelLoader.load(info);


				for (Graphics::Mesh mesh : model->meshes)
				{
					if (RenderBatches.find(mesh.shader) == RenderBatches.end())
						RenderBatches.emplace(mesh.shader, Graphics::RenderBatch());

					Graphics::RenderBatch &batch = RenderBatches.at(mesh.shader);

					std::cout << "BATCH SHADER PROCESS: " << batch.shader << std::endl;
					std::cout << "MESH SHADER PROCESS: " << mesh.shader << std::endl;
					if (!batch.isChanged())
						continue;

					batch.insert(
						mesh,
						node->globalTransform);

						std::cout << batch.vertexData.size() << std::endl;

					// std::cout << "SHADER: " << shaderCounts[mesh.shader] << std::endl;
					shaderCounts[mesh.shader]++;
				}
				return true;
			};

			sceneGraph.at("ROOT")->traverse(
				sceneGraph.at("ROOT"),
				handleNode);


				for (auto& batch : RenderBatches)
				{
                    std::cout << "BATCH TRANSFORM SIZE: " << batch.second.transforms.size() << std::endl;          
                    std::cout << "BATCH TEXTURE SIZE: " << batch.second.textureInfo.size() << std::endl;          

				}

			for (
				std::pair<std::string, unsigned int> pair : shaderCounts)
				{
				    RenderBatches[pair.first].finish();
				}

			/*
			while (it.next())
			{
			/*
				std::optional<RenderComponent> isComponent = it.get();

				if (!isComponent.has_value())
					continue;

				RenderComponent component = isComponent.value();
				// Prior to handling, sort by shader then
			// organize each shader group by texture

				for (Graphics::Mesh mesh : model.meshes)
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

					batch.insert
					(
						mesh,
						position
					);

					std::cout << "SHADER: " << shaderCounts[mesh.shader] << std::endl;
					shaderCounts[mesh.shader]++;
				}
			}


*/

		}

		void loadModel(Entity entity, Graphics::ModelInfo &info, Graphics::Entity parent);

		std::unordered_map<std::string, std::shared_ptr<Graphics::Model>> cachedModels;
		std::unordered_map<std::string, std::shared_ptr<Graphics::SceneNode>> sceneGraph;
		Graphics::ModelLoader<AssimpImporter> modelLoader;
	};

	class SystemManager
	{
	public:
		void update();

		Entity createModel(Graphics::ModelInfo &info, Graphics::Entity parent = -1);
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
