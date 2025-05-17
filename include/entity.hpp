#ifndef __TERREATE_ENTITY_HPP__
#define __TERREATE_ENTITY_HPP__

#include <core/math.hpp>
#include <core/uuid.hpp>
#include <renderer.hpp>
#include <resource.hpp>
#include <transform.hpp>

namespace Terreate::Interface {

struct IEntity {
public:
  virtual Core::UUID GetUUID() const = 0;
  virtual Transform GetTransform() const = 0;
  virtual void SetTransform(Transform const &transform) = 0;
  virtual void Update(Double const &tick) = 0;
  virtual void Draw(Resource<Renderer> &renderer) const = 0;
};
} // namespace Terreate::Interface

#endif // __TERREATE_ENTITY_HPP__
