#include "SystemManager.h"


void Graphics::SystemManager::update()
{
    renderSystem.updateImpl(m_RenderBatches);
}


Graphics::Entity Graphics::SystemManager::createModel(Graphics::ModelInfo& info)
{
    Graphics::Entity entity = entityManager.createEntity();
    renderSystem.loadModel(entity, info);

    return entity;
}

void Graphics::RenderSystem::loadModel(Graphics::Entity entity, Graphics::ModelInfo& info)
{
    Graphics::RenderComponent component = Graphics::RenderComponent(modelLoader.load(info));
    componentManager.add<RenderComponent>(entity, component);
}
