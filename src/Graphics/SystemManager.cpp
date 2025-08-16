#include "SystemManager.h"

void Graphics::SystemManager::update()
{
    renderSystem.updateImpl(m_RenderBatches);
}

Graphics::Entity Graphics::SystemManager::createModel(Graphics::ModelInfo &info, Graphics::Entity parent)
{

    Graphics::Entity entity = entityManager.createEntity();
    renderSystem.loadModel(entity, info, parent);
    m_RenderBatches[info.DEFAULT_SHADER].alertChange();
    return entity;
}

void Graphics::RenderSystem::loadModel(Graphics::Entity entity, Graphics::ModelInfo &info, Graphics::Entity parent)
{

    componentManager
        .add<Renderable>
        (
            entity, 
            Graphics::Renderable(info.path)
        );

    componentManager
        .add<Graphics::Transform>
        (
            entity,
            Graphics::Transform(glm::vec3(1.0f))
        );

    sceneGraph.at("ROOT")->insert(entity, glm::mat4(1.0f), parent);
}
