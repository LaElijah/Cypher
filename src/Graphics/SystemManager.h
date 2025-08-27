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
#include <nlohmann/json.hpp>
#include <GLFW/glfw3.h>

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

		std::shared_ptr<Graphics::ComponentManager> getComponentManager()
		{
			return componentManager;
		};



    protected:
	    static inline std::shared_ptr<Graphics::ComponentManager> componentManager;
	};

	class RenderSystem : public System<RenderSystem>
	{
	public:
		RenderSystem()
		{
			sceneGraph.emplace("ROOT", std::make_shared<Graphics::SceneNode>());
		}

		std::pair<bool, nlohmann::json> getJSONGraph()
		{
			return {true, jsonGraph};
		}

		void updateImpl(
			std::map<
				std::string,
				Graphics::RenderBatch> &RenderBatches)
		{
			// TODO: this currently sets rendering to the scene manager, this
			// needs to be done in a linear fashion.

			sceneGraph.at("ROOT")->dirty = true;
			std::unordered_map<std::string, unsigned int> shaderCounts;

			for (std::pair<std::string, Graphics::RenderBatch> pair : RenderBatches)
			{
				// Handle what happens when a buffer is changed
				if (pair.second.isChanged())
					pair.second.reset();
			}


			std::function<nlohmann::json(std::shared_ptr<SceneNode>, nlohmann::json)>
				handleNodeJson = [this](std::shared_ptr<SceneNode> node, nlohmann::json data)
			{
			
				auto transform = componentManager->get<Graphics::Transform>(node->ENTITY);
				auto renderable = componentManager->get<Graphics::Renderable>(node->ENTITY);




				std::stringstream entity;
				entity << node->ENTITY;
				
				data["transform"]["position"]["x"] = glm::float64(transform.position.x);
				data["transform"]["position"]["y"] = glm::float64(transform.position.y);
				data["transform"]["position"]["z"] = glm::float64(transform.position.z);
				data["entity"]["id"] = entity.str();
				data["entity"]["name"] = renderable.name;

				return data;
			};

			// TODO: INDEX BATCHES IN AN ARRAY BY SHADER NAME
			std::function<
				bool(
					std::shared_ptr<Graphics::SceneNode> node,
					glm::mat4 & parentTransform)>
				handleNode = [this, &RenderBatches, &shaderCounts](
								 std::shared_ptr<Graphics::SceneNode> node,
								 glm::mat4 &parentTransform) mutable
			{
				// LOAD INFO TO SCENE GRAPH JSON

				// LOAD DATA INTO BUFFERS
				Graphics::Entity entity = node->ENTITY;
				auto renderable = componentManager->get<Graphics::Renderable>(entity);
				auto transform = componentManager->get<Graphics::Transform>(entity);



				node->globalTransform = transform.localTransform * parentTransform;
				// std::string parent = "PARENT" + node->PARENT;
				// std::cout << parent << std::endl;

				// Start of render duties
				Graphics::ModelInfo info;
				info.path = renderable.path;
			 

				auto model = modelLoader.load(info);

				for (Graphics::Mesh& mesh : model->meshes)
				{
					if (RenderBatches.find(mesh.shader) == RenderBatches.end())
						RenderBatches.emplace(mesh.shader, Graphics::RenderBatch());

					Graphics::RenderBatch &batch = RenderBatches.at(mesh.shader);




					if (batch.transformMappings.contains(model->info.path))
					{

						unsigned int size = batch.transformMappings.at(model->info.path).first;
						unsigned int start = batch.transformMappings.at(model->info.path).second;

						unsigned int end = size + start;

						std::cout << "OFFSET: " << start << std::endl;
						std::cout << "SIZE: " << size << std::endl;
						std::cout << "END SIZE: " << end << std::endl;


						std::cout << "TRANSFORM SIZE: " << batch.transforms.size() << std::endl;
					    std::fill(
						batch.transforms.begin(), // Start of model block
						batch.transforms.end(),   // end of model block 
						node->globalTransform);
						//glm::translate(glm::mat4(1.0f), transform.position));
					}


					if (!batch.isChanged())
						continue;

					if (!batch.transformMappings.contains(model->info.path))
					{
						std::cout << "NEW MAP: CURRENT OFFSET: " << batch.TRANSFORM_OFFSET << std::endl;

						if (batch.transformMappings.size() > 0)
						{
						    batch.TRANSFORM_OFFSET += batch.lastModelSize;
							batch.lastModelSize = model->meshes.size();
						}
							std::cout << "MESH SIZE: " << model->meshes.size() << std::endl;
						std::cout << "NEW MAP: CHANGED OFFSET: " << batch.TRANSFORM_OFFSET << std::endl;
	                         	
							std::pair<unsigned int, unsigned int> pair = 
							{
								model->meshes.size(),
							    batch.TRANSFORM_OFFSET
							};

					    batch.transformMappings.emplace
						(
							model->info.path, 
							pair
						);

						batch.recentModel = model->info.path;
					}


					//batch.recentModel = model->info.path;

						//model->info.path

					batch.insert(
						mesh,
						node->globalTransform);

						std::cout << "TRANSFORM COUNT: " << batch.transforms.size() << std::endl;

            std::cout << "POST INSERT" << std::endl;
					shaderCounts[mesh.shader]++;
				}

            std::cout << "NODE FINISHED" << std::endl;
				return true;
			};

			if (sceneGraph.at("ROOT")->traverse_preorder(
					sceneGraph.at("ROOT"),
					handleNode))
			{

            std::cout << "POST RENDER TRAVERSAL" << std::endl;
				jsonGraph = sceneGraph.at("ROOT")->traverse_postorder(
					sceneGraph.at("ROOT"),
					handleNodeJson);



				/*
					*/
			}

			/*
			sceneGraph.at("ROOT")->traverse_postorder(
			sceneGraph.at("ROOT"),
			handleNodeJson);
*/

            std::cout << "FINISHING" << std::endl;
			for (std::pair<std::string, unsigned int> pair : shaderCounts)
				RenderBatches[pair.first].finish();
		}

		void loadModel(Entity entity, Graphics::ModelInfo &info, Graphics::Entity parent);

		std::unordered_map<std::string, std::shared_ptr<Graphics::SceneNode>> sceneGraph;
		Graphics::ModelLoader<AssimpImporter> modelLoader;

		nlohmann::json jsonGraph;
	};

	class SystemManager
	{
	public:
		std::pair<bool, nlohmann::json> getJSONGraph()
		{
			return renderSystem.getJSONGraph();
		};


		std::shared_ptr<Graphics::ComponentManager> getComponentManager()
		{
			return renderSystem.getComponentManager();
		};

		void update();

		Entity createModel(Graphics::ModelInfo &info, Graphics::Entity parent = 0);

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
