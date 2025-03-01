#ifndef __TERREATE_CORE_OBJECT_HPP__
#define __TERREATE_CORE_OBJECT_HPP__

#include <types.hpp>

namespace Terreate::Core {
using namespace Terreate::Types;

class Object {
private:
  Shared<TCu32> mObject;

public:
  Object() { mObject = std::make_shared<TCu32>(0); };
  explicit Object(TCu32 const &object) {
    mObject = std::make_shared<TCu32>(object);
  }
  Object(Object const &other) : mObject(other.mObject) {}
  Object(Object &&other) : mObject(std::move(other.mObject)) {
    other.mObject = nullptr;
  }
  ~Object() {}

  TCu32 *Ptr();
  TCu32 const *Ptr() const;
  TCu32 &Ref();
  TCu32 const &Ref() const;

  Uint Count() const { return mObject.use_count(); }
  void Delete();

  TCu32 operator*() const;
  Object &operator=(Object const &other);
  Object &operator=(Object &&other);
  Object &operator=(TCu32 const &object);

  Bool operator==(TCu32 const &object) const { return this->Ref() == object; }
  Bool operator!=(TCu32 const &object) const { return this->Ref() != object; }

  operator TCu32 *() { return this->Ptr(); }
  operator TCu32 const *() const { return this->Ptr(); }
  operator TCu32 &() { return this->Ref(); }
  operator TCu32 const &() const { return this->Ref(); }
  operator Bool() const { return mObject != nullptr; }
};
} // namespace Terreate::Core

#endif // __TERREATE_CORE_OBJECT_HPP__
