#pragma once

#include <bits/stdc++.h>

namespace Terreate::Types {
static char const *VK_LAYER_KHRONOS_VALIDATION = "VK_LAYER_KHRONOS_validation";
static std::vector<char const *> const VALIDATION_LAYERS = {
    VK_LAYER_KHRONOS_VALIDATION};

typedef std::int8_t i8;
typedef std::int16_t i16;
typedef std::int32_t i32;
typedef std::int64_t i64;
typedef std::uint8_t u8;
typedef std::uint16_t u16;
typedef std::uint32_t u32;
typedef std::uint64_t u64;

typedef std::string str;

template <typename T> using vec = std::vector<T>;
template <typename T> using uset = std::unordered_set<T>;
template <typename T> using set = std::set<T>;
template <typename Key, typename Value>
using umap = std::unordered_map<Key, Value>;
template <typename Key, typename Value> using map = std::map<Key, Value>;

} // namespace Terreate::Types
