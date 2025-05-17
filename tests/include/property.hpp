// #pragma once

// #include <event.hpp>
// #include <exception.hpp>
// #include <type.hpp>

// namespace Terreate::Core {
// using namespace Terreate::Type;

// template <typename T> class Property {
// private:
//   using Getter = function<T &(T *)>;
//   using Setter = function<void(T *, T const &)>;

// private:
//   T *mValue = nullptr;

// public:
//   Getter getter = [](T *value) -> T & {
//     if (value)
//       return *value;
//     throw Exception::NullReferenceException("Value is null");
//   };
//   Setter setter = [](T *value, T const &val) {
//     if (value)
//       *value = val;
//     else
//       throw Exception::NullReferenceException("Value is null");
//   };

// public:
//   Property() : mValue(nullptr) {}
//   explicit Property(T *value) : mValue(value) {}
//   ~Property() { mValue = nullptr; }

//   void set(T *value) { mValue = value; }
//   T const &get() const {
//     if (this->getter)
//       return this->getter(mValue);
//     throw Exception::NotImplementedException();
//   }
//   T &get() {
//     if (this->getter)
//       return this->getter(mValue);
//     throw Exception::NotImplementedException();
//   }

//   operator T const() const {
//     if (this->getter)
//       return this->getter(mValue);
//     throw Exception::NotImplementedException();
//   }
//   operator T() {
//     if (this->getter)
//       return this->getter(mValue);
//     throw Exception::NotImplementedException();
//   }

//   Property &operator=(T const &value) {
//     if (this->setter) {
//       this->setter(mValue, value);
//     } else {
//       throw Exception::NotImplementedException();
//     }
//     return *this;
//   }
// };

// } // namespace Terreate::Core

// template <typename T>
// std::ostream &operator<<(std::ostream &os,
//                          Terreate::Core::Property<T> const &prop) {
//   os << *prop;
//   return os;
// }
