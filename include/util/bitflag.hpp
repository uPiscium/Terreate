#pragma once
#include "../decl/type.hpp"

namespace Terreate::Core {
template <typename T>
concept enum_type = std::is_enum_v<T>;

template <enum_type T> class Bitflag {
public:
  using underlying_type = std::underlying_type_t<T>;

private:
  underlying_type mFlags;

public:
  Bitflag() : mFlags(0) {}
  Bitflag(T flag) : mFlags(static_cast<underlying_type>(flag)) {}

  Bitflag &operator~() {
    mFlags = ~mFlags;
    return *this;
  }
  Bitflag &operator|=(T flag) {
    mFlags |= static_cast<underlying_type>(flag);
    return *this;
  }
  Bitflag &operator&=(T flag) {
    mFlags &= static_cast<underlying_type>(flag);
    return *this;
  }
  Bitflag &operator^=(T flag) {
    mFlags ^= static_cast<underlying_type>(flag);
    return *this;
  }
  Bitflag &operator|=(Bitflag<T> const &other) {
    mFlags |= static_cast<underlying_type>(other);
    return *this;
  }
  Bitflag &operator&=(Bitflag<T> const &other) {
    mFlags &= static_cast<underlying_type>(other);
    return *this;
  }
  Bitflag &operator^=(Bitflag<T> const &other) {
    mFlags ^= static_cast<underlying_type>(other);
    return *this;
  }
  Bitflag &operator=(T flag) {
    return Bitflag(static_cast<underlying_type>(flag));
  }
  Bitflag &operator=(Bitflag<T> const &other) {
    mFlags = static_cast<underlying_type>(other);
    return *this;
  }

  explicit operator bool() const { return mFlags != 0; }
  explicit operator underlying_type() const { return mFlags; }
  explicit operator T() const { return static_cast<T>(mFlags); }
};

template <enum_type T> inline Bitflag<T> operator|(Bitflag<T> lhs, T rhs) {
  lhs |= rhs;
  return lhs;
}
template <enum_type T> inline Bitflag<T> operator&(Bitflag<T> lhs, T rhs) {
  lhs &= rhs;
  return lhs;
}
template <enum_type T> inline Bitflag<T> operator^(Bitflag<T> lhs, T rhs) {
  lhs ^= rhs;
  return lhs;
}
template <enum_type T>
inline Bitflag<T> operator|(Bitflag<T> lhs, Bitflag<T> rhs) {
  lhs |= rhs;
  return lhs;
}
template <enum_type T>
inline Bitflag<T> operator&(Bitflag<T> lhs, Bitflag<T> rhs) {
  lhs &= rhs;
  return lhs;
}
template <enum_type T>
inline Bitflag<T> operator^(Bitflag<T> lhs, Bitflag<T> rhs) {
  lhs ^= rhs;
  return lhs;
}
template <enum_type T> inline Bitflag<T> operator|(T lhs, T rhs) {
  return Bitflag<T>(static_cast<typename Bitflag<T>::underlying_type>(lhs) |
                    static_cast<typename Bitflag<T>::underlying_type>(rhs));
}
template <enum_type T> inline Bitflag<T> operator&(T lhs, T rhs) {
  return Bitflag<T>(static_cast<typename Bitflag<T>::underlying_type>(lhs) &
                    static_cast<typename Bitflag<T>::underlying_type>(rhs));
}
template <enum_type T> inline Bitflag<T> operator^(T lhs, T rhs) {
  return Bitflag<T>(static_cast<typename Bitflag<T>::underlying_type>(lhs) ^
                    static_cast<typename Bitflag<T>::underlying_type>(rhs));
}

template <enum_type T> inline bool operator==(Bitflag<T> lhs, Bitflag<T> rhs) {
  return static_cast<typename Bitflag<T>::underlying_type>(lhs) ==
         static_cast<typename Bitflag<T>::underlying_type>(rhs);
}
template <enum_type T> inline bool operator!=(Bitflag<T> lhs, Bitflag<T> rhs) {
  return !(lhs == rhs);
}

} // namespace Terreate::Core
