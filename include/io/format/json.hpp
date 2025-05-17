#ifndef __TERREATE_IO_FORMAT_JSON_HPP__
#define __TERREATE_IO_FORMAT_JSON_HPP__

#include <core/type.hpp>

namespace Terreate::IO::JSON {
using namespace Terreate::Types;

class JSON;
typedef Vec<JSON> Array;
typedef Map<Str, JSON> Object;

enum class Type { NONE, BOOL, NUMBER, STRING, ARRAY, OBJECT };

union Value {
  bool boolean;
  double number;
  Str *string;
  Array *array;
  Object *object;
};

class JSON {
private:
  Value mValue;
  Type mType;

private:
  void Copy(JSON const &other);
  void Clear();

public:
  JSON() : mType(Type::NONE) { mValue.number = 0; }
  JSON(bool const &v) : mType(Type::BOOL) { mValue.boolean = v; }
  JSON(double const &v) : mType(Type::NUMBER) { mValue.number = v; }
  JSON(Str const &v) : mType(Type::STRING) { mValue.string = new Str(v); }
  JSON(Array const &v) : mType(Type::ARRAY) { mValue.array = new Array(v); }
  JSON(Object const &v) : mType(Type::OBJECT) { mValue.object = new Object(v); }
  JSON(JSON const &other) { this->Copy(other); }

  Type const &getType() const { return mType; }
  Value const &getValue() const { return mValue; }
  void get(bool &v);
  void get(double &v);
  void get(Str &v);
  void get(Array &v);
  void get(Object &v);

  void set(bool const &v);
  void set(double const &v);
  void set(Str const &v);
  void set(Array const &v);
  void set(Object const &v);

  bool isNone() const { return mType == Type::NONE; }
  bool isBool() const { return mType == Type::BOOL; }
  bool isNumber() const { return mType == Type::NUMBER; }
  bool isString() const { return mType == Type::STRING; }
  bool isArray() const { return mType == Type::ARRAY; }
  bool isObject() const { return mType == Type::OBJECT; }

  JSON &operator=(bool const &v);
  JSON &operator=(double const &v);
  JSON &operator=(Str const &v);
  JSON &operator=(Array const &v);
  JSON &operator=(Object const &v);

  operator bool() const;
  operator double() const;
  operator Str() const;
  operator Array() const;
  operator Object() const;
};

} // namespace Terreate::IO::JSON

#endif // __TERREATE_IO_FORMAT_JSON_HPP__
