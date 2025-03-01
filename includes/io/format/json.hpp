#ifndef __TERREATE_IO_FORMAT_JSON_HPP__
#define __TERREATE_IO_FORMAT_JSON_HPP__

#include <types.hpp>

namespace Terreate::IO::JSON {
using namespace Terreate::Types;

class JSON;
typedef Vec<JSON> Array;
typedef Map<Str, JSON> Object;

enum class Type { NONE, BOOL, NUMBER, STRING, ARRAY, OBJECT };

union Value {
  Bool boolean;
  Double number;
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
  JSON(Bool const &v) : mType(Type::BOOL) { mValue.boolean = v; }
  JSON(Double const &v) : mType(Type::NUMBER) { mValue.number = v; }
  JSON(Str const &v) : mType(Type::STRING) { mValue.string = new Str(v); }
  JSON(Array const &v) : mType(Type::ARRAY) { mValue.array = new Array(v); }
  JSON(Object const &v) : mType(Type::OBJECT) { mValue.object = new Object(v); }
  JSON(JSON const &other) { this->Copy(other); }

  Type const &GetType() const { return mType; }
  Value const &GetValue() const { return mValue; }
  void Get(Bool &v);
  void Get(Double &v);
  void Get(Str &v);
  void Get(Array &v);
  void Get(Object &v);

  void Set(Bool const &v);
  void Set(Double const &v);
  void Set(Str const &v);
  void Set(Array const &v);
  void Set(Object const &v);

  Bool IsNone() const { return mType == Type::NONE; }
  Bool IsBool() const { return mType == Type::BOOL; }
  Bool IsNumber() const { return mType == Type::NUMBER; }
  Bool IsString() const { return mType == Type::STRING; }
  Bool IsArray() const { return mType == Type::ARRAY; }
  Bool IsObject() const { return mType == Type::OBJECT; }

  JSON &operator=(Bool const &v);
  JSON &operator=(Double const &v);
  JSON &operator=(Str const &v);
  JSON &operator=(Array const &v);
  JSON &operator=(Object const &v);

  operator Bool() const;
  operator Double() const;
  operator Str() const;
  operator Array() const;
  operator Object() const;
};

} // namespace Terreate::IO::JSON

#endif // __TERREATE_IO_FORMAT_JSON_HPP__
