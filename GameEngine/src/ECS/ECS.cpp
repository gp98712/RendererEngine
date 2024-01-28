#include "ECS.h"
#include "../Logger/Logger.h"
#include <algorithm>
#include <iterator>

int IComponent::nextId = 0;

int Entity::GetId() const {
    return id;
}

void Entity::Kill() {
    registry->KillEntity(*this);
}

void Entity::Tag(const std::string& tag) {
	registry->TagEntity(*this, tag);
}

bool Entity::HasTag(const std::string& tag) const {
	return registry->EntityHasTag(*this, tag);
}

void Entity::Group(const std::string& group) {
	registry->GroupEntity(*this, group);
}

bool Entity::BelongsToGroup(const std::string& group) const {
	return registry->EntityBelongsToGroup(*this, group);
}

void System::AddEntityToSystem(Entity entity) {
    entities[entity.GetId()] = entity;
}

void System::RemoveEntityFromSystem(Entity entity) {
    entities.erase(entity.GetId());
}

std::unordered_map<int,Entity> System::GetSystemEntities() const{
    return entities;
}

Entity* System::getEntityById(int id) {
    auto entityMap = entities.find(id);
    if (entityMap != entities.end()) {
		return &entityMap->second;
	}

    return nullptr;
}

const Signature& System::GetComponentSignature() const {
    return componentSignature;
}

Entity Registry::CreateEntity() {
    int entityId;

    if (freeIds.empty()) {
        // If there are no free ids waiting to be reused
        entityId = numEntities++;
        if (entityId >= entityComponentSignatures.size()) {
            entityComponentSignatures.resize(entityId + 1);
        }
    } else {
        // Reuse an id from the list of previously removed entities
        entityId = freeIds.front();
        freeIds.pop_front();
    }

    Entity entity(entityId);
    entity.registry = this;
    entitiesToBeAdded.insert(entity);
    Logger::Log("Entity created with id " + std::to_string(entityId));

    return entity;
}

void Registry::KillEntity(Entity entity) {
    entitiesToBeKilled.insert(entity);
    Logger::Log("Entity " + std::to_string(entity.GetId()) + " was killed");
}

void Registry::AddEntityToSystems(Entity entity) {
    // Entity ������Ʈ �ñ״�ó�� System�� ������Ʈ �ñ״�ó�� ��ġ�ϸ� System�� Entity�� �߰��Ѵ�.
    // ����! ������ Entity�� System�� �߰���Ű�� ���� ��.��.�� Registry�� ���ؼ� �߰��϶�.
    const auto entityId = entity.GetId();

    const auto& entityComponentSignature = entityComponentSignatures[entityId];
    
    for (auto& system: systems) {
        const auto& systemComponentSignature = system.second->GetComponentSignature();

        bool isInterested = (entityComponentSignature & systemComponentSignature) == systemComponentSignature;

        if (isInterested) {
            system.second->AddEntityToSystem(entity);
        }
    }
}

void Registry::RemoveEntityFromSystems(Entity entity) {
    for (auto system: systems) {
        system.second->RemoveEntityFromSystem(entity);
    }
}

void Registry::TagEntity(Entity entity, const std::string& tag) {
    entityPerTag.emplace(tag, entity);
    tagPerEntity.emplace(entity.GetId(), tag);
}

bool Registry::EntityHasTag(Entity entity, const std::string& tag) const {
	if (tagPerEntity.find(entity.GetId()) == tagPerEntity.end()) {
		return false;
	}
	return entityPerTag.find(tag)->second == entity;
}

Entity Registry::GetEntityByTag(const std::string& tag) const {
    return entityPerTag.at(tag);
}

void Registry::RemoveEntityTag(Entity entity) {
	auto taggedEntity = tagPerEntity.find(entity.GetId());
    if (taggedEntity != tagPerEntity.end()) {
        auto tag = taggedEntity->second;
        entityPerTag.erase(tag);
        tagPerEntity.erase(taggedEntity);
    }
}

void Registry::GroupEntity(Entity entity, const std::string& group) {
    entitiesPerGroup.emplace(group, std::set<Entity>());
    entitiesPerGroup[group].emplace(entity);
    groupPerEntity.emplace(entity.GetId(), group);
}

bool Registry::EntityBelongsToGroup(Entity entity, const std::string& group) const {
    if (entitiesPerGroup.find(group) == entitiesPerGroup.end()) {
        return false;
    }
	auto groupEntities = entitiesPerGroup.at(group);
    return groupEntities.find(entity.GetId()) != groupEntities.end();
}

std::vector<Entity> Registry::GetEntitiesByGroup(const std::string& group) const {
    auto& setOfEntities = entitiesPerGroup.at(group);
    return std::vector<Entity>(setOfEntities.begin(), setOfEntities.end());
}

void Registry::RemoveEntityGroup(Entity entity) {
    // if in group, remove entity from group management
    auto groupedEntity = groupPerEntity.find(entity.GetId());
    if (groupedEntity != groupPerEntity.end()) {
        auto group = entitiesPerGroup.find(groupedEntity->second);
        if (group != entitiesPerGroup.end()) {
            auto entityInGroup = group->second.find(entity);
            if (entityInGroup != group->second.end()) {
                group->second.erase(entityInGroup);
            }
        }
        groupPerEntity.erase(groupedEntity);
    }
}

void Registry::Update() {
    for (auto entity: entitiesToBeAdded) {
        AddEntityToSystems(entity);
    }
    entitiesToBeAdded.clear();

    /*
    * �������� �͵� ����Ʈ
    * 1. componentPools
    * 2. entityComponentSignatures
    * 3. freeIds Ǫ��
    * 4. entitiesToBeKilled
    * 5. entityPerTag
    * 6. tagPerEntity
    * 7. entitiesPerGroup
    * 8. groupPerEntity
    */
    for (auto entity: entitiesToBeKilled) {
        RemoveEntityFromSystems(entity);
        entityComponentSignatures[entity.GetId()].reset();

        // Remove entity from component pools
        for (auto pool: componentPools) {
            if (pool) {
                pool->RemoveEntityFromPool(entity.GetId());
            }
        }

        freeIds.push_back(entity.GetId());

	    RemoveEntityTag(entity);
	    RemoveEntityGroup(entity);
    }
    entitiesToBeKilled.clear();
}
