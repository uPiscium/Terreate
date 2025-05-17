#ifndef __TERREATE_SCENE_HPP__
#define __TERREATE_SCENE_HPP__

#include <entity.hpp>
#include <resource.hpp>
#include <types.hpp>

namespace Terreate {
using namespace Terreate::Types;

typedef Resource<Interface::IEntity> Entity;

class Scene : public Interface::IEntity {
private:
  Map<Core::UUID, Entity> mEntities;

public:
  Scene() = default;
  ~Scene() = default;

  void Add(Entity const &entity);
  void Remove(Entity const &entity);
  void Draw(Resource<Renderer> &renderer) const override;
};

} // namespace Terreate

#endif // __TERREATE_SCENE_HPP__
