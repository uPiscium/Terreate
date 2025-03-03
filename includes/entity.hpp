#ifndef __TERREATE_ENTITY_HPP__
#define __TERREATE_ENTITY_HPP__

#include <core/uuid.hpp>
#include <types.hpp>

namespace Terreate {
using namespace Terreate::Types;

class IEntity {
public:
  virtual ~IEntity() = default;
  virtual Core::UUID const &GetUUID() const = 0;
  virtual Bool operator==(IEntity const &other) const {
    return this->GetUUID() == other.GetUUID();
  }
  virtual Bool operator!=(IEntity const &other) const {
    return !(*this == other);
  }
};

class IRenderable {
public:
  virtual ~IRenderable() = default;
  virtual void Render() = 0;
};

} // namespace Terreate

#endif // __TERREATE_ENTITY_HPP__
