#ifndef SCENE_GRAPH_H
#define SCENE_GRAPH_H

#include <vector>
#include "Primitives.h"
#include <glm/glm.hpp>

namespace Graphics
{

    struct SceneNode
    {

        // Tree Identity
        Graphics::Entity entity;
        std::vector<std::shared_ptr<SceneNode>> children;

        // Scene Node data
        glm::vec3 position;


        // Global data
        bool dirty = true;
        glm::mat4 globalTransform;


        //Functions


        void traverseScene
        (
            std::shared_ptr<Graphics::SceneNode> node;
        )
        {

            if (!node.dirty)
                for (std::shared_ptr<Graphics::SceneNode> childNode : node.children)
                    traverseScene(childNode); 
        }

    };


}


#endif