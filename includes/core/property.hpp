#ifndef __TERREATE_PROPERTY_HPP__
#define __TERREATE_PROPERTY_HPP__

#include <exceptions.hpp>
#include <types.hpp>

namespace Terreate::Core {
using namespace Terreate::Types;

template <typename T> class Property {
public:
  using Type = T;
  using PointerGetter = Function<Type *()>;
  using Getter = Function<Type()>;
  using Setter = Function<void(Type)>;

private:
  Type *mValue = nullptr;

public:
  PointerGetter pgetter = [this]() -> Type * { return mValue; };
  Getter getter = [this]() -> Type {
    if (mValue)
      return *mValue;
    throw Exceptions::NullReferenceException("Value is null");
  };
  Setter setter = [this](Type value) {
    if (mValue)
      *mValue = value;
    else
      throw Exceptions::NullReferenceException("Value is null");
  };

public:
  Property() = default;
  explicit Property(Type *value) : mValue(value) {}
  ~Property() = default;

  void Set(Type *value) { mValue = value; }

  Type const &operator*() const {
    if (getter)
      return getter();
    throw Exceptions::NotImplementedException();
  }

  Type &operator*() {
    if (getter)
      return getter();
    throw Exceptions::NotImplementedException();
  }

  Type *operator->() const {
    if (pgetter)
      return pgetter();
    throw Exceptions::NotImplementedException();
  }

  operator Type() const {
    if (getter)
      return getter();
    throw Exceptions::NotImplementedException();
  }

  operator Type() {
    if (getter)
      return getter();
    throw Exceptions::NotImplementedException();
  }

  Property &operator=(T const &value) {
    if (setter) {
      setter(value);
    } else {
      throw Exceptions::NotImplementedException();
    }
    return *this;
  }
};

} // namespace Terreate::Core

template <typename T>
std::ostream &operator<<(std::ostream &os,
                         Terreate::Core::Property<T> const &prop) {
  os << *prop;
  return os;
}

#endif // __TERREATE_PROPERTY_HPP__
