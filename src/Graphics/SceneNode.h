#ifndef SCENE_NODE_H
#define SCENE_NODE_H

#include <vector>
#include "Primitives.h"
#include <glm/glm.hpp>
#include <memory>
#include <nlohmann/json.hpp>

namespace Graphics
{

	class SceneNode
	{
	public:
		SceneNode(
			Graphics::Entity entity,
			glm::mat4 transform,
			Graphics::Entity parent)
		{
			ENTITY = entity;
			PARENT = parent;
			globalTransform = transform;
		}

		// Root node constructor
		SceneNode(glm::mat4 transform = glm::mat4(1.0))
		{
			ENTITY = 0;
			PARENT = 0;

			globalTransform = transform;
		}

		static bool updateNode(
			std::shared_ptr<SceneNode> node,
			glm::mat4 &parentTransform)
		{
			node->globalTransform = node->globalTransform * parentTransform;
			return true;
		}

		static nlohmann::json updateJSON(
			std::shared_ptr<SceneNode> node,
			nlohmann::json data)
		{


			return data;
		}

		nlohmann::json traverse_postorder(
			std::shared_ptr<SceneNode> node,
			std::function<
				nlohmann::json(
					std::shared_ptr<SceneNode> node,
					nlohmann::json data)>
				update = updateJSON,
			glm::mat4 &parentTransform = rootTransform,
			bool wasDirty = false)
		{


			nlohmann::json data;

			for (auto child : (node->ENTITY == 0) ? root : node->children)
			{

			    std::stringstream stream, childStream;
				stream << "ENTITY-" << node->ENTITY;
				childStream << "ENTITY-" << child.second->ENTITY;

				data[stream.str()]["children"][childStream.str()] = traverse_postorder(
					child.second,
					update,
					node->globalTransform,
					(node->dirty || wasDirty));
			}

		    if (node->ENTITY != 0)
			    data = update(node, data);

		
			return data;
		}

		bool traverse_preorder(
			std::shared_ptr<SceneNode> node,
			std::function<
				bool(
					std::shared_ptr<SceneNode> node,
					glm::mat4 &parentTransform)>
				update = updateNode,
			glm::mat4 &parentTransform = rootTransform,
			bool wasDirty = false,
			bool searching = true)
		{

			bool changed = false;
			bool val = false;
			bool next = false;

			if (node->dirty || wasDirty)
			{

				if (node->ENTITY != 0)
				{
					next = update(node, parentTransform);
					changed = true;
				}
				else
					next = searching;

				node->dirty = false;
				wasDirty = true;
			}

			if (next || searching) // If searching, will iterate through full graph
				for (auto child : (node->ENTITY == 0) ? root : node->children)
				{
					val = traverse_preorder(
						child.second,
						update,
						node->globalTransform,
						(node->dirty || wasDirty));

					changed = (val || changed) ? true : false;
				}

			return changed;
		}

		std::vector<std::shared_ptr<Graphics::SceneNode>> getNodes()
		{
			std::vector<std::shared_ptr<Graphics::SceneNode>> pointers;

			for (auto pair : entities)
			{
				if (!pair.second.expired())
					pointers.push_back(pair.second.lock());
				else
					entities.erase(pair.first);
			}

			return pointers;
		}

		void remove(Graphics::Entity entity)
		{
			if (validate(entity))
			{
				std::shared_ptr<SceneNode> node = entities.at(entity).lock();

				Graphics::Entity parent = node->PARENT;

				if (parent == 0)
				{
					root.erase(entity);
					entities.erase(entity);
					return;
				}
				else if (validate(parent))
				{
					std::shared_ptr<SceneNode> parentNode = entities.at(parent).lock();
					parentNode->children.erase(entity);
					entities.erase(entity);
				}
			}
		}

		void insert(
			Graphics::Entity entity,
			glm::mat4 transform,
			Graphics::Entity parent = 0)
		{
			std::shared_ptr<SceneNode> node = std::make_shared<SceneNode>(
				entity,
				transform,
				parent);

			std::weak_ptr<SceneNode> weak = node;

			if (parent == 0)
				root.insert(std::make_pair(entity, node));
			else if (validate(parent))
				entities.at(parent).lock()->children.insert(std::make_pair(entity, node));
			else
			{
				std::cout << "PARENT NOT FOUND IN SCENE" << std::endl;
				return;
			}

			entities.insert(std::make_pair(entity, weak));
		}

		bool dirty = true;
		glm::mat4 globalTransform;
		size_t ENTITY;
		size_t PARENT;

	protected:
		std::unordered_map<
			Graphics::Entity,
			std::shared_ptr<SceneNode>>
			children;

	private:
		bool validate(Graphics::Entity entity)
		{
			if (!entities.count(entity) == 0)
			{
				std::cout << "NODE DOESNT EXIST" << std::endl;
				return false;
			}

			if (!entities.at(entity).expired())
			{
				std::cout << "NODE DELETED ALREADY" << std::endl;
				entities.erase(entity);
				return false;
			}
			return true;
		}

		static inline std::unordered_map<
			Graphics::Entity,
			std::shared_ptr<SceneNode>>
			root;

		static inline glm::mat4 rootTransform = glm::mat4(1.0f);

		static inline std::unordered_map<
			Graphics::Entity,
			std::weak_ptr<SceneNode>>
			entities;
	};
}

#endif
