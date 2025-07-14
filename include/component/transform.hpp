#pragma once

#include "../common/math.hpp"

#include "interface.hpp"

namespace Terreate::Component {
class TransformComponent : public IComponent {
private:
  UUID mID;
  vec3 mPosition{0.0f, 0.0f, 0.0f};
  quat mRotation{0.0f, 0.0f, 0.0f, 1.0f};
  vec3 mScale{1.0f, 1.0f, 1.0f};

public:
  TransformComponent() = default;
  ~TransformComponent() override = default;

  UUID const &getID() const override { return mID; }
  vec3 const &getScale() const { return mScale; }
  quat const &getRotation() const { return mRotation; }
  vec3 const &getPosition() const { return mPosition; }
  mat4 getTransformMatrix() const;

  void setScale(vec3 const &scale) { mScale = scale; }
  void setRotation(quat const &rotation) { mRotation = rotation; }
  void setPosition(vec3 const &position) { mPosition = position; }

  void scale(float factor) { mScale *= factor; }
  void rotate(quat const &rotation) {
    mRotation = normalize(mRotation * rotation);
  }
  void move(vec3 const &offset) { mPosition += offset; }

  operator mat4() const { return this->getTransformMatrix(); }
};

class TransformSystem : public ISystem {
private:
  UUID mID;
  shared<Component::ComponentManager> mComponentManager;

public:
  TransformSystem() = default;
  ~TransformSystem() override = default;

  UUID const &getID() const override { return mID; }

  void
  setComponentManager(shared<Component::ComponentManager> manager) override {
    mComponentManager = manager;
  }

  void update(float const &delta) override;
};
} // namespace Terreate::Component
