#pragma once

#include "../common/math.hpp"
#include "../common/type.hpp"
#include "../common/uuid.hpp"

#include "entity.hpp"

namespace Terreate::Core {
class Scene {
private:
  UUID mID;
  str mName = "";
  umap<UUID, shared<Entity>> mEntities;
  umap<str, shared<Scene>> mSubScenes;

public:
  Transform transform;

public:
  Scene(str const &name) : mName(name) {}
  ~Scene() = default;

  UUID const &getID() const { return mID; }
  str const &getName() const { return mName; }
  umap<UUID, shared<Entity>> const &getEntities() const { return mEntities; }
  umap<str, shared<Scene>> const &getSubScenes() const { return mSubScenes; }

  void setName(str const &name) { mName = name; }

  void addEntity(shared<Entity> const &entity);
  shared<Scene> createSubScene(str const &name);
};
} // namespace Terreate::Core
