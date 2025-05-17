#ifndef __TERREATE_CORE_OBJECT_HPP__
#define __TERREATE_CORE_OBJECT_HPP__

#include <core/type.hpp>

namespace Terreate::Core {
using namespace Terreate::Types;

class Object {
private:
  Shared<u32> mObject;

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

  u32 *ptr();
  u32 const *ptr() const;
  u32 &ref();
  u32 const &ref() const;

  u32 count() const { return mObject.use_count(); }
  void dispose();

  u32 operator*() const;
  Object &operator=(Object const &other);
  Object &operator=(Object &&other);
  Object &operator=(u32 const &object);

  bool operator==(u32 const &object) const { return this->ref() == object; }
  bool operator!=(u32 const &object) const { return this->ref() != object; }

  operator u32 *() { return this->ptr(); }
  operator u32 const *() const { return this->ptr(); }
  operator u32 &() { return this->ref(); }
  operator u32 const &() const { return this->ref(); }
  operator bool() const { return mObject != nullptr; }
};
} // namespace Terreate::Core

#endif // __TERREATE_CORE_OBJECT_HPP__
