#pragma once
#include "../decl/exception.hpp"
#include "../decl/type.hpp"

namespace Terreate::Util {
template <typename T> class Property {
private:
  using Getter = Type::function<T &(T *)>;
  using Setter = Type::function<void(T *, T const &)>;

private:
  T *mValue = nullptr;

public:
  static Getter DEFAULT_GETTER;
  static Setter DEFAULT_SETTER;

public:
  Getter getter = DEFAULT_GETTER;
  Setter setter = DEFAULT_SETTER;

public:
  Property() : mValue(nullptr) {}
  explicit Property(T *value) : mValue(value) {}
  ~Property() { mValue = nullptr; }

  void set(T *value) { mValue = value; }
  T const &get() const {
    if (this->getter)
      return this->getter(mValue);
    throw Exception::NotImplementedException();
  }
  T &get() {
    if (this->getter)
      return this->getter(mValue);
    throw Exception::NotImplementedException();
  }

  operator T const() const {
    if (this->getter)
      return this->getter(mValue);
    throw Exception::NotImplementedException();
  }
  operator T() {
    if (this->getter)
      return this->getter(mValue);
    throw Exception::NotImplementedException();
  }

  Property &operator=(T const &value) {
    if (this->setter) {
      this->setter(mValue, value);
    } else {
      throw Exception::NotImplementedException();
    }
    return *this;
  }
};

template <typename T>
Property<T>::Getter Property<T>::DEFAULT_GETTER = [](T *value) -> T & {
  if (value)
    return *value;
  throw Exception::NullReferenceException("Property referenced value is null.");
};

template <typename T>
Property<T>::Setter Property<T>::DEFAULT_SETTER = [](T *value, T const &val) {
  if (value)
    *value = val;
  else
    throw Exception::NullReferenceException(
        "Property referenced value is null.");
};

} // namespace Terreate::Util

template <typename T>
std::ostream &operator<<(std::ostream &os,
                         Terreate::Util::Property<T> const &prop) {
  os << *prop;
  return os;
}
