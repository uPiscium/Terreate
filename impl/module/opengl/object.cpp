#include "../../../include/module/common/exception.hpp"
#include "../../../include/module/opengl/object.hpp"

namespace Terreate::OpenGL {
u32 *Object::Ptr() {
  if (mObject == nullptr) {
    throw Exception::NullObjectException();
  }
  return mObject.get();
}

u32 const *Object::Ptr() const {
  if (mObject == nullptr) {
    throw Exception::NullObjectException();
  }
  return mObject.get();
}

u32 &Object::Ref() {
  if (mObject == nullptr) {
    throw Exception::NullObjectException();
  }
  return *mObject;
}

u32 const &Object::Ref() const {
  if (mObject == nullptr) {
    throw Exception::NullObjectException();
  }
  return *mObject;
}

void Object::Delete() {
  if (mObject != nullptr) {
    mObject = nullptr;
  }
}

Object &Object::operator=(Object const &other) {
  mObject = other.mObject;
  return *this;
}

Object &Object::operator=(Object &&other) {
  mObject = std::move(other.mObject);
  other.mObject = nullptr;
  return *this;
}

Object &Object::operator=(u32 const &other) {
  mObject = std::make_shared<u32>(other);
  return *this;
}
} // namespace Terreate::OpenGL
