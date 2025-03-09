#include <transform.hpp>

#include <iomanip>

namespace Terreate {
Transform::Transform() {
  matrix = Math::mat4(1.0f);
  position = Math::vec3(0.0f);
  front = Math::vec3(0.0f, 0.0f, -1.0f);
  up = Math::vec3(0.0f, 1.0f, 0.0f);
  right = Math::vec3(1.0f, 0.0f, 0.0f);
}

Transform::Transform(Math::vec3 const &position) : Transform() {
  this->position = position;
  matrix = Math::translate(matrix, position);
}

Transform::Transform(Math::vec3 const &position, Math::vec3 const &front)
    : Transform(position) {
  this->front = Math::normalize(front);
  this->right = Math::normalize(Math::cross(Transform::YAXIS, this->front));
  this->up = Math::normalize(Math::cross(this->front, this->right));
  matrix[0] = Math::vec4(this->right, 0.0f);
  matrix[1] = Math::vec4(this->up, 0.0f);
  matrix[2] = Math::vec4(this->front, 0.0f);
}

} // namespace Terreate

Terreate::Transform operator*(Terreate::Math::mat4 const &lhs,
                              Terreate::Transform const &rhs) {
  Terreate::Transform result = rhs;
  result.matrix = lhs * rhs.matrix;
  result.position = Terreate::Math::vec3(result.matrix[3]);
  result.front =
      Terreate::Math::normalize(Terreate::Math::vec3(result.matrix[2]));
  result.up = Terreate::Math::normalize(Terreate::Math::vec3(result.matrix[1]));
  result.right =
      Terreate::Math::normalize(Terreate::Math::vec3(result.matrix[0]));
  return result;
}

std::ostream &operator<<(std::ostream &os, Terreate::Math::vec2 const &vec) {
  os << std::fixed << std::setprecision(6) << "(" << vec.x << ", " << vec.y
     << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, Terreate::Math::vec3 const &vec) {
  os << std::fixed << std::setprecision(6) << "(" << vec.x << ", " << vec.y
     << ", " << vec.z << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, Terreate::Math::vec4 const &vec) {
  os << std::fixed << std::setprecision(6) << "(" << vec.x << ", " << vec.y
     << ", " << vec.z << ", " << vec.w << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, Terreate::Math::quat const &quat) {
  os << std::fixed << std::setprecision(6) << "(" << quat.x << ", " << quat.y
     << ", " << quat.z << ", " << quat.w << ")";
  return os;
}

std::ostream &operator<<(std::ostream &os, Terreate::Math::mat2 const &mat) {
  os << std::fixed << std::setprecision(6);
  os << "[[" << mat[0][0] << ", " << mat[0][1] << "]" << std::endl;
  os << " [" << mat[1][0] << ", " << mat[1][1] << "]]";
  return os;
}

std::ostream &operator<<(std::ostream &os, Terreate::Math::mat3 const &mat) {
  os << std::fixed << std::setprecision(6);
  os << "[[" << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << "]"
     << std::endl;
  os << " [" << mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << "]"
     << std::endl;
  os << " [" << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << "]]";
  return os;
}

std::ostream &operator<<(std::ostream &os, Terreate::Math::mat4 const &mat) {
  os << std::fixed << std::setprecision(6);
  os << "[[" << mat[0][0] << ", " << mat[0][1] << ", " << mat[0][2] << ", "
     << mat[0][3] << "]" << std::endl;
  os << " [" << mat[1][0] << ", " << mat[1][1] << ", " << mat[1][2] << ", "
     << mat[1][3] << "]" << std::endl;
  os << " [" << mat[2][0] << ", " << mat[2][1] << ", " << mat[2][2] << ", "
     << mat[2][3] << "]" << std::endl;
  os << " [" << mat[3][0] << ", " << mat[3][1] << ", " << mat[3][2] << ", "
     << mat[3][3] << "]]";
  return os;
}

std::ostream &operator<<(std::ostream &os,
                         Terreate::Transform const &transform) {
  os << "Position: " << transform.position << std::endl;
  os << "Front: " << transform.front << std::endl;
  os << "Up: " << transform.up << std::endl;
  os << "Right: " << transform.right << std::endl;
  os << "Matrix: " << std::endl << transform.matrix;
  return os;
}
