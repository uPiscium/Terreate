#include <core/exception.hpp>
#include <core/object.hpp>

namespace Terreate::Core {
u32 *Object::ptr() {
  if (mObject == nullptr) {
    throw Exceptions::NullObjectException();
  }
  return mObject.get();
}

u32 const *Object::ptr() const {
  if (mObject == nullptr) {
    throw Exceptions::NullObjectException();
  }
  return mObject.get();
}

u32 &Object::ref() {
  if (mObject == nullptr) {
    throw Exceptions::NullObjectException();
  }
  return *mObject;
}

u32 const &Object::ref() const {
  if (mObject == nullptr) {
    throw Exceptions::NullObjectException();
  }
  return *mObject;
}

void Object::dispose() {
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
} // namespace Terreate::Core
