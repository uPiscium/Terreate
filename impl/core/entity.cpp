#include <core/entity.hpp>

namespace Terreate::Core {
shared<Entity> EntityManager::get(UUID const &id) const {
  auto it = mEntities.find(id);
  if (it != mEntities.end()) {
    return it->second;
  }
  return nullptr; // Return nullptr if entity not found
}

shared<Entity> EntityManager::create() {
  auto entity = std::make_shared<Entity>(mComponentRegistry);
  mEntities[entity->getID()] = entity;
  return entity;
}

void EntityManager::destroy(UUID const &id) {
  auto it = mEntities.find(id);
  if (it != mEntities.end()) {
    mEntities.erase(it);
  } else {
    throw std::runtime_error("Entity with given ID does not exist.");
  }
}
} // namespace Terreate::Core
