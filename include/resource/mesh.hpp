#pragma once

#include "../common/bitflag.hpp"
#include "../common/type.hpp"

#include "../opengl/buffer.hpp"

#include "interface.hpp"

namespace Terreate::Resource {
enum class MeshFlags : u32 {
  NONE = 0,
  NORMAL = 1 << 0,
  UV = 1 << 1,
  COLOR = 1 << 2,
  TANGENT = 1 << 3,
  BITANGENT = 1 << 4
};

struct MeshData {
  vec<vec3> positions;
  vec<vec3> normals;
  vec<vec3> uvs;
  vec<vec4> colors;
  vec<vec4> tangents;
  vec<vec4> bitangents;
  vec<vec<u32>> faces;
  vec<u32> indices;
  Bitflag<MeshFlags> flags = MeshFlags::NONE;
};

class MeshResource : public IResource {
private:
  UUID mID;
  shared<OpenGL::Buffer> mVertexBuffer;

public:
  MeshResource() = default;
  ~MeshResource() override = default;

  UUID const &getID() const override { return mID; }

  void loadMesh(MeshData const &data);
};
} // namespace Terreate::Resource
