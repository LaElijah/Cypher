#include "SystemManager.h"

void Graphics::SystemManager::update()
{
    renderSystem.updateImpl(m_RenderBatches);
}

Graphics::Entity Graphics::SystemManager::createMesh(Graphics::ModelInfo &info, Graphics::Entity parent)
{
    Graphics::Entity entity = entityManager.createEntity();
    renderSystem.loadMesh(entity, info, parent);


    return entity;
}


void Graphics::RenderSystem::loadMesh(Graphics::Entity entity, Graphics::ModelInfo &info, Graphics::Entity parent)
{
    std::stringstream name;
    name << "ENTITY-" << entity;

    Graphics::Renderable renderable(info.path, info.path);
    renderable.meshIndex = info.meshIndex;
    componentManager
	    ->add<Graphics::Renderable>(entity, renderable);

    componentManager
        ->add<Graphics::Transform>(
            entity,
            Graphics::Transform(glm::vec3(1.0f)));


            
    // TODO: Remove transform from scene node definition 
    sceneGraph.at("ROOT")->insert(entity, glm::mat4(1.0f), parent);
}
Graphics::Entity Graphics::SystemManager::createModel(Graphics::Entity parent)
{

    Graphics::Entity entity = entityManager.createEntity();
    renderSystem.loadModel(entity, parent);

    return entity;
}

void Graphics::RenderSystem::loadModel(Graphics::Entity entity, Graphics::Entity parent)
{

    std::stringstream name;
    name << "ENTITY-" << entity;

       componentManager
        ->add<Graphics::Transform>(
            entity,
            Graphics::Transform(glm::vec3(1.0f)));


            

    sceneGraph.at("ROOT")->insert(entity, glm::mat4(1.0f), parent);
}

