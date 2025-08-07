#include "SystemManager.h"

void Graphics::SystemManager::update()
{
    renderSystem.updateImpl(m_RenderBatches);
}

Graphics::Entity Graphics::SystemManager::createModel(Graphics::ModelInfo &info)
{

    Graphics::Entity entity = entityManager.createEntity();
    renderSystem.loadModel(entity, info);
    m_RenderBatches[info.DEFAULT_SHADER].alertChange();
    return entity;
}

void Graphics::RenderSystem::loadModel(Graphics::Entity entity, Graphics::ModelInfo &info)
{
    componentManager
        .add<RenderComponent>
        (
            entity, 
            Graphics::RenderComponent(modelLoader.load(info))
        );
}
