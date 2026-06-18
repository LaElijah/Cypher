#include "EntityManager.h"


Graphics::Entity Graphics::EntityManager::createEntity()
{
    Entity newEntity;
    if (m_InactiveEntities.size() > 0)
    {
        newEntity = *m_InactiveEntities.begin();
        m_InactiveEntities.erase(newEntity);
        return newEntity;
    }
    else 
    {
        newEntity = HEAD_ENTITY;
        HEAD_ENTITY++;
        m_ActiveEntities.insert(newEntity);
        return newEntity;
    }
}

bool Graphics::EntityManager::removeEntity(Entity entity)
{
    if (m_ActiveEntities.find(entity) != m_ActiveEntities.end())
    {
        m_ActiveEntities.erase(entity);
        m_InactiveEntities.insert(entity);
        return true;	
    }
    else 
        return false;
}






