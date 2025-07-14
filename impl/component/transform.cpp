#include <component/transform.hpp>

namespace Terreate::Component {
mat4 TransformComponent::getTransformMatrix() const {
  mat4 model = mat4(1.0f);
  model = Terreate::translate(model, mPosition);
  mat4 rotationMatrix = glm::mat4_cast(mRotation);
  model = model * rotationMatrix;
  model = Terreate::scale(model, mScale);
  return model;
}
} // namespace Terreate::Component
