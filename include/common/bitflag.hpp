#pragma once
#include "common/type.hpp"

namespace Terreate {

template <enumtype T> class Bitflag {
  using type = T;
  using utype = std::underlying_type_t<T>;

private:
  utype mFlags;

public:
  Bitflag() : mFlags(0) {}
  Bitflag(T flag) : mFlags(static_cast<utype>(flag)) {}
  Bitflag(int flags) : mFlags(static_cast<utype>(flags)) {}
  Bitflag(Bitflag<T> const &other) : mFlags(static_cast<utype>(other)) {}

  Bitflag &operator~();
  Bitflag &operator|=(T flag);
  Bitflag &operator&=(T flag);
  Bitflag &operator^=(T flag);
  Bitflag &operator|=(Bitflag<T> const &other);
  Bitflag &operator&=(Bitflag<T> const &other);
  Bitflag &operator^=(Bitflag<T> const &other);

  Bitflag &operator=(T flag) { return Bitflag(static_cast<utype>(flag)); }
  Bitflag &operator=(Bitflag<T> const &other);
  Bitflag &operator=(utype flags);

  explicit operator bool() const { return mFlags != 0; }
  explicit operator utype() const { return mFlags; }
  explicit operator T() const { return static_cast<T>(mFlags); }
};

template <enumtype T> Bitflag<T> operator|(Bitflag<T> lhs, T rhs);
template <enumtype T> Bitflag<T> operator&(Bitflag<T> lhs, T rhs);
template <enumtype T> Bitflag<T> operator^(Bitflag<T> lhs, T rhs);
template <enumtype T> Bitflag<T> operator|(Bitflag<T> lhs, Bitflag<T> rhs);
template <enumtype T> Bitflag<T> operator&(Bitflag<T> lhs, Bitflag<T> rhs);
template <enumtype T> Bitflag<T> operator^(Bitflag<T> lhs, Bitflag<T> rhs);

template <enumtype T> bool operator==(Bitflag<T> lhs, Bitflag<T> rhs);
template <enumtype T> inline bool operator!=(Bitflag<T> lhs, Bitflag<T> rhs);

} // namespace Terreate

// =================================================
// Implementation
// =================================================
namespace Terreate {
template <enumtype T> Bitflag<T> &Bitflag<T>::operator~() {
  mFlags = ~mFlags;
  return *this;
}

template <enumtype T> Bitflag<T> &Bitflag<T>::operator|=(T flag) {
  mFlags |= static_cast<utype>(flag);
  return *this;
}

template <enumtype T> Bitflag<T> &Bitflag<T>::operator&=(T flag) {
  mFlags &= static_cast<utype>(flag);
  return *this;
}

template <enumtype T> Bitflag<T> &Bitflag<T>::operator^=(T flag) {
  mFlags ^= static_cast<utype>(flag);
  return *this;
}

template <enumtype T>
Bitflag<T> &Bitflag<T>::operator|=(Bitflag<T> const &other) {
  mFlags |= static_cast<utype>(other);
  return *this;
}

template <enumtype T>
Bitflag<T> &Bitflag<T>::operator&=(Bitflag<T> const &other) {
  mFlags &= static_cast<utype>(other);
  return *this;
}

template <enumtype T>
Bitflag<T> &Bitflag<T>::operator^=(Bitflag<T> const &other) {
  mFlags ^= static_cast<utype>(other);
  return *this;
}

template <enumtype T>
Bitflag<T> &Bitflag<T>::operator=(Bitflag<T> const &other) {
  mFlags = static_cast<utype>(other);
  return *this;
}

template <enumtype T> Bitflag<T> &Bitflag<T>::operator=(utype flags) {
  mFlags = flags;
  return *this;
}

template <enumtype T> Bitflag<T> operator|(Bitflag<T> lhs, T rhs) {
  lhs |= rhs;
  return lhs;
}
template <enumtype T> Bitflag<T> operator&(Bitflag<T> lhs, T rhs) {
  lhs &= rhs;
  return lhs;
}
template <enumtype T> Bitflag<T> operator^(Bitflag<T> lhs, T rhs) {
  lhs ^= rhs;
  return lhs;
}
template <enumtype T> Bitflag<T> operator|(Bitflag<T> lhs, Bitflag<T> rhs) {
  lhs |= rhs;
  return lhs;
}
template <enumtype T> Bitflag<T> operator&(Bitflag<T> lhs, Bitflag<T> rhs) {
  lhs &= rhs;
  return lhs;
}
template <enumtype T> Bitflag<T> operator^(Bitflag<T> lhs, Bitflag<T> rhs) {
  lhs ^= rhs;
  return lhs;
}

template <enumtype T> bool operator==(Bitflag<T> lhs, Bitflag<T> rhs) {
  return static_cast<typename Bitflag<T>::underlying_type>(lhs) ==
         static_cast<typename Bitflag<T>::underlying_type>(rhs);
}
template <enumtype T> bool operator!=(Bitflag<T> lhs, Bitflag<T> rhs) {
  return !(lhs == rhs);
}

} // namespace Terreate
