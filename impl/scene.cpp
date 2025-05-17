#include <exceptions.hpp>
#include <scene.hpp>

namespace Terreate {

void Scene::Add(Entity const &entity) {
  if (mEntities.find(entity->GetUUID()) != mEntities.end()) {
    throw Exceptions::EntityException("Entity already exists in scene");
  }
  mEntities[entity->GetUUID()] = entity;
}

void Scene::Remove(Entity const &entity) {
  if (mEntities.find(entity->GetUUID()) == mEntities.end()) {
    throw Exceptions::EntityException("Entity does not exist in scene");
  }
  mEntities.erase(entity->GetUUID());
}

void Scene::Draw(Resource<Renderer> &renderer) const {
  for (auto const &[uuid, entity] : mEntities) {
    entity->Draw(renderer);
  }
}

} // namespace Terreate
