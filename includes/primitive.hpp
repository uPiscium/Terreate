#ifndef __TERREATE_PRIMITIVE_HPP__
#define __TERREATE_PRIMITIVE_HPP__

#include <core/math.hpp>
#include <entity.hpp>
#include <graphic/buffer.hpp>
#include <types.hpp>

namespace Terreate {
using namespace Terreate::Types;
using namespace Terreate::Math;

class Point : public Interface::IEntity {
private:
  Core::UUID mUUID;
  Graphic::Buffer mBuffer;
  Transform mTransform;
  Uint mPointSize;

private:
  void SetupBuffer();

public:
  Point() : mPointSize(1) { this->SetupBuffer(); }
  Point(vec3 const &position) : mTransform(position), mPointSize(1) {
    this->SetupBuffer();
  }
  Point(vec3 const &position, Uint const &size)
      : mTransform(position), mPointSize(size) {
    this->SetupBuffer();
  }
  ~Point() {}

  Core::UUID const &GetUUID() const override { return mUUID; }
  Transform GetTransform() const override { return mTransform; }

  void SetTransform(Transform const &transform) override {
    mTransform = transform;
  }
  void Update(Double const &tick) override {}
  void Draw(Resource<Renderer> &renderer) const override;
};
} // namespace Terreate

#endif // __TERREATE_PRIMITIVE_HPP__
