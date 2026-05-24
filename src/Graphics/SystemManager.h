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
			modelLoader = std::make_shared<Graphics::ModelLoader<AssimpImporter>>();
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
			std::function<nlohmann::json(std::shared_ptr<SceneNode>, nlohmann::json)>
				handleNodeJson = [this](std::shared_ptr<SceneNode> node, nlohmann::json data)
			{
	
				if (node->ENTITY == 0)
				    return nlohmann::json();

				
				auto transform = componentManager->get<Graphics::Transform>(node->ENTITY);
				//auto renderable = componentManager->get<Graphics::Renderable>(node->ENTITY);

				std::stringstream entity;
				entity << node->ENTITY;

				
				data["transform"]["position"]["x"] = glm::float64(transform.position.x);
				data["transform"]["position"]["y"] = glm::float64(transform.position.y);
				data["transform"]["position"]["z"] = glm::float64(transform.position.z);
				data["entity"]["id"] = entity.str();
				data["entity"]["name"] = "renderable";


				return data;
			};

			// TODO: INDEX BATCHES IN AN ARRAY BY SHADER NAME

			std::function<
			    bool
			    (
			        std::shared_ptr<Graphics::SceneNode> node,
				glm::mat4 & parentTransform)>
				handleNode = 
				[this, &RenderBatches]
				(
				    std::shared_ptr<Graphics::SceneNode> node,
				    glm::mat4 &parentTransform
				) mutable
			{
			    // LOAD DATA INTO BUFFERS
			    Graphics::Entity entity = node->ENTITY;

			    //auto renderable = componentManager->get<Graphics::Renderable>(entity);
			    auto transform = componentManager->get<Graphics::Transform>(entity);

			    node->globalTransform = transform.localTransform * parentTransform;
			    return true;
			};

			// Searches for dirty nodes and 
			// will update all children of dirty nodes 
			if 
			(
			    sceneGraph.at("ROOT")
			        ->traverse_preorder
			        (
				    sceneGraph.at("ROOT"),
				    handleNode
				)
			)
			{
		            jsonGraph = sceneGraph.at("ROOT")
			        ->traverse_postorder
			        (
			            sceneGraph.at("ROOT"),
			            handleNodeJson
			        );

			}
		    	
			auto it = componentManager
			->getAllContainingMulti
			<
			Graphics::Renderable,
                        Graphics::Transform
			>();


			while (it.next())
			{
                            auto val = it.get();

			    auto& [renderable, transform] = *val;

			    //std::cout << "CURRENT TRANSFORM: " << transform.position.x << std::endl;
			    auto [vertexRecord, indexRecord] = modelLoader->getModelRecords(renderable.path);

			    //std::cout << "RECORDS: " << vertexRecord->start << std::endl;

			    /*
			    if (drawData.count(renderable.path) != 0)
			    {
			        Graphics::RenderDrawData data = drawData.at(renderable.path);
				data.

			    }
			    */
			}
		}

		std::shared_ptr<Graphics::ModelLoader<AssimpImporter>> getResourceManager()
		{
		    return modelLoader;
		}

		std::shared_ptr<Graphics::ModelLoader<AssimpImporter>> modelLoader;
		void loadMesh(Graphics::Entity entity, Graphics::ModelInfo &info, Graphics::Entity parent);

		void loadModel(Entity entity, Graphics::Entity parent);

		std::unordered_map<std::string, std::shared_ptr<Graphics::SceneNode>> sceneGraph;

		nlohmann::json jsonGraph;
	};

	class SystemManager
	{
	public:
		std::pair<bool, nlohmann::json> getJSONGraph()
		{
			return renderSystem.getJSONGraph();
		};

		
	
		std::shared_ptr<Graphics::ModelLoader<AssimpImporter>> getResourceManager()
		{
                    return renderSystem.getResourceManager();
		};

		std::shared_ptr<Graphics::ComponentManager> getComponentManager()
		{
			return renderSystem.getComponentManager();
		};

		void update();

		Entity createModel(Graphics::Entity parent = 0);



		Graphics::Entity createMesh(Graphics::ModelInfo &info, Graphics::Entity parent);

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
