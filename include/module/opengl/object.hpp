#pragma once

#include "../common/type.hpp"

namespace Terreate::OpenGL {
class Object {
private:
  shared<u32> mObject;

public:
  Object() { mObject = std::make_shared<u32>(0); };
  explicit Object(u32 const &object) {
    mObject = std::make_shared<u32>(object);
  }
  Object(Object const &other) : mObject(other.mObject) {}
  Object(Object &&other) : mObject(std::move(other.mObject)) {
    other.mObject = nullptr;
  }
  ~Object() {}

  u32 *Ptr();
  u32 const *Ptr() const;
  u32 &Ref();
  u32 const &Ref() const;

  u32 Count() const { return mObject.use_count(); }
  void Delete();

  u32 operator*() const;
  Object &operator=(Object const &other);
  Object &operator=(Object &&other);
  Object &operator=(u32 const &object);

  bool operator==(u32 const &object) const { return this->Ref() == object; }
  bool operator!=(u32 const &object) const { return this->Ref() != object; }

  operator u32 *() { return this->Ptr(); }
  operator u32 const *() const { return this->Ptr(); }
  operator u32 &() { return this->Ref(); }
  operator u32 const &() const { return this->Ref(); }
  operator bool() const { return mObject != nullptr; }
};
} // namespace Terreate::OpenGL
