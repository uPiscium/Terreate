#pragma once
#include "type.hpp"

namespace Terreate {
static umap<str, u32> const GLSL_ATTRIBUTE_LOCATIONS = {
    {"vPosition", 0}, {"vMainUV", 1},  {"vLightingUV", 2}, {"vTilingUV", 3},
    {"vNormal", 4},   {"vTangent", 5}, {"vBitangent", 6},
};

static umap<str, Attribute> const GLSL_INPUT_ATTRIBUTES = {
    {"vPosition", {4, 112, 0}},    {"vMainUV", {4, 112, 16}},
    {"vLightingUV", {4, 112, 32}}, {"vTilingUV", {4, 112, 48}},
    {"vNormal", {4, 112, 64}},     {"vTangent", {4, 112, 80}},
    {"vBitangent", {4, 112, 96}},
};
} // namespace Terreate
