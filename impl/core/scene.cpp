#include <core/scene.hpp>

namespace Terreate::Core {
void Scene::addEntity(shared<Entity> const &entity) {
  if (mEntities.contains(entity->getID())) {
    return; // Entity already exists
  }
  mEntities.insert({entity->getID(), entity});
}

shared<Scene> Scene::createSubScene(str const &name) {
  if (mSubScenes.contains(name)) {
    return mSubScenes[name]; // Return existing sub-scene
  }

  auto subScene = make_shared<Scene>(name);
  mSubScenes.insert({name, subScene});
  return subScene;
}
} // namespace Terreate::Core
