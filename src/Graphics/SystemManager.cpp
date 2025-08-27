#include "SystemManager.h"

void Graphics::SystemManager::update()
{
    renderSystem.updateImpl(m_RenderBatches);
}

Graphics::Entity Graphics::SystemManager::createModel(Graphics::ModelInfo &info, Graphics::Entity parent)
{

    Graphics::Entity entity = entityManager.createEntity();
    m_RenderBatches[info.DEFAULT_SHADER].alertChange();
    renderSystem.loadModel(entity, info, parent);

    return entity;
}

void Graphics::RenderSystem::loadModel(Graphics::Entity entity, Graphics::ModelInfo &info, Graphics::Entity parent)
{

    std::stringstream name;
    name << "ENTITY-" << entity;

    componentManager
        ->add<Renderable>(
            entity,
            Graphics::Renderable(info.path, name.str()));

    componentManager
        ->add<Graphics::Transform>(
            entity,
            Graphics::Transform(glm::vec3(1.0f)));

            std::cout << "ADDED ENTITY:" << entity << std::endl;

            

    sceneGraph.at("ROOT")->insert(entity, glm::mat4(1.0f), parent);
}
