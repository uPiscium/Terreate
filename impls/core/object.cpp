#include <core/object.hpp>
#include <exceptions.hpp>

namespace Terreate::Core {
TCu32 *Object::Ptr() {
  if (mObject == nullptr) {
    throw Exceptions::NullObjectException();
  }
  return mObject.get();
}

TCu32 const *Object::Ptr() const {
  if (mObject == nullptr) {
    throw Exceptions::NullObjectException();
  }
  return mObject.get();
}

TCu32 &Object::Ref() {
  if (mObject == nullptr) {
    throw Exceptions::NullObjectException();
  }
  return *mObject;
}

TCu32 const &Object::Ref() const {
  if (mObject == nullptr) {
    throw Exceptions::NullObjectException();
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

Object &Object::operator=(TCu32 const &other) {
  mObject = std::make_shared<TCu32>(other);
  return *this;
}
} // namespace Terreate::Core
