#ifndef __TERREATE_SCENE_HPP__
#define __TERREATE_SCENE_HPP__

#include <entity.hpp>
#include <resource.hpp>
#include <types.hpp>

namespace Terreate {
using namespace Terreate::Types;

typedef Resource<IEntity> Entity;

class Scene {
private:
  Vec<Entity> mEntities;

public:
  Scene() = default;
  ~Scene() = default;
};
} // namespace Terreate

#endif // __TERREATE_SCENE_HPP__
