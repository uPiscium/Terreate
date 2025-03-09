#ifndef __TERREATE_TRANSFORM_HPP__
#define __TERREATE_TRANSFORM_HPP__

#include <core/math.hpp>
#include <types.hpp>

namespace Terreate {
using namespace Terreate::Types;

struct Transform {
public:
  constexpr static Math::vec3 const XAXIS = Math::vec3(1.0f, 0.0f, 0.0f);
  constexpr static Math::vec3 const YAXIS = Math::vec3(0.0f, 1.0f, 0.0f);
  constexpr static Math::vec3 const ZAXIS = Math::vec3(0.0f, 0.0f, 1.0f);

public:
  Math::vec3 position = Math::vec3(0.0f);
  Math::vec3 front = Math::vec3(0.0f, 0.0f, -1.0f);
  Math::vec3 up = Math::vec3(0.0f, 1.0f, 0.0f);
  Math::vec3 right = Math::vec3(1.0f, 0.0f, 0.0f);
  Math::mat4 matrix = Math::identity<Math::mat4>();

public:
  Transform();
  Transform(Math::vec3 const &position);
  Transform(Math::vec3 const &position, Math::vec3 const &front);

public:
  static Math::mat4 Identity() { return Math::identity<Math::mat4>(); }
  static Math::mat4 Scale(Math::vec3 const &scale) {
    return Math::scale(Transform::Identity(), scale);
  }
  static Math::quat RotateQuat(Math::vec3 const &axis, Float const angle) {
    return Math::angleAxis(Math::radians(angle), Math::normalize(axis));
  }
  static Math::mat4 Rotate(Math::quat const &rotation) {
    return glm::mat4_cast(rotation);
  }
  static Math::mat4 Rotate(Float const &angle, Math::vec3 const &axis) {
    return Math::rotate(Transform::Identity(), Math::radians(angle),
                        Math::normalize(axis));
  }
  static Math::mat4 Translate(Math::vec3 const &position) {
    return Math::translate(Transform::Identity(), position);
  }
  static Math::mat4 LookAt(Math::vec3 const &position, Math::vec3 const &target,
                           Math::vec3 const &up) {
    return Math::lookAt(position, target, up);
  }
  static Math::mat4 LookAt(Math::vec3 const &position,
                           Math::vec3 const &target) {
    return Math::lookAt(position, target, Transform::YAXIS);
  }
  static Math::mat4 LookAt(Transform const &transform) {
    return Math::lookAt(transform.position,
                        transform.position + transform.front, transform.up);
  }
  static Math::mat4 Perspective(Float const fov, Float const aspect,
                                Float const near, Float const far) {
    return Math::perspective(fov, aspect, near, far);
  }
  static Math::mat4 Orthographic(Float const left, Float const right,
                                 Float const bottom, Float const top,
                                 Float const near, Float const far) {
    return Math::ortho(left, right, bottom, top, near, far);
  }
};

} // namespace Terreate

Terreate::Transform operator*(Terreate::Math::mat4 const &lhs,
                              Terreate::Transform const &rhs);
std::ostream &operator<<(std::ostream &os, Terreate::Math::vec2 const &vec);
std::ostream &operator<<(std::ostream &os, Terreate::Math::vec3 const &vec);
std::ostream &operator<<(std::ostream &os, Terreate::Math::vec4 const &vec);
std::ostream &operator<<(std::ostream &os, Terreate::Math::quat const &quat);
std::ostream &operator<<(std::ostream &os, Terreate::Math::mat2 const &mat);
std::ostream &operator<<(std::ostream &os, Terreate::Math::mat3 const &mat);
std::ostream &operator<<(std::ostream &os, Terreate::Math::mat4 const &mat);
std::ostream &operator<<(std::ostream &os,
                         Terreate::Transform const &transform);

#endif // __TERREATE_TRANSFORM_HPP__
