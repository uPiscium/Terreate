#pragma once

#include "../common/bitflag.hpp"
#include "../common/type.hpp"

#include "../opengl/buffer.hpp"

#include "interface.hpp"

namespace Terreate::Resource {
enum class MeshFlags : u8 {
  NONE = 0,
  POSITION = 1 << 0,
  NORMAL = 1 << 1,
  MAIN_UV = 1 << 2,
  LIGHTING_UV = 1 << 3,
  TILING_UV = 1 << 4,
  TANGENT = 1 << 5,
  BITANGENT = 1 << 6,
};

struct VertexSet {
  i32 position = -1;   // Index in position array
  i32 mainUV = -1;     // Index in main UV array
  i32 lightingUV = -1; // Index in lighting UV array
  i32 tilingUV = -1;   // Index in tiling UV array
  i32 normal = -1;     // Index in normal array
  i32 tangent = -1;    // Index in tangent array
  i32 bitangent = -1;  // Index in bitangent array
};

struct GeometryData {
  vec<vec3> position;
  vec<vec3> mainUV;     // last component is the index of layer
  vec<vec3> lightingUV; // ...
  vec<vec3> tilingUV;   // ...
  vec<vec3> normal;
  vec<vec4> tangent;
  vec<vec4> bitangent;
  vec<VertexSet> vertices;
  vec<u32> indices;
  Bitflag<MeshFlags> flags;
  DrawMode drawMode = DrawMode::TRIANGLES;
};

struct Vertex {
  vec4 position;   // (x, y, z) + padding * 1
  vec4 mainUV;     // (u, v, layer) + padding * 1
  vec4 lightingUV; // (u, v, layer) + padding * 1
  vec4 tilingUV;   // (u, v, layer) + padding * 1
  vec4 normal;     // (x, y, z) + padding * 1
  vec4 tangent;    // (x, y, z, w) where w is the handedness
  vec4 bitangent;  // (x, y, z, w) where w is the handedness
};

struct RawMesh {
  vec<Vertex> vertices;
  vec<u32> indices;
  Bitflag<MeshFlags> flags;
  DrawMode drawMode = DrawMode::TRIANGLES;
};

class IPrimitive {
public:
  virtual ~IPrimitive() = default;
  virtual GeometryData getMesh() const = 0;
};

class Triangle : public IPrimitive {
private:
  GeometryData mMesh;

public:
  Triangle(vec2 const &a, vec2 const &b, vec2 const &c);
  ~Triangle() override = default;

  GeometryData getMesh() const override { return mMesh; }
};

class Rectangle : public IPrimitive {
private:
  GeometryData mMesh;

public:
  Rectangle(float width, float height);
  ~Rectangle() override = default;

  GeometryData getMesh() const override { return mMesh; }
};

class Square : public Rectangle {
public:
  Square(float size) : Rectangle(size, size) {}
  ~Square() override = default;

  GeometryData getMesh() const override { return Rectangle::getMesh(); }
};

class ConvexPolygon : public IPrimitive {
private:
  GeometryData mMesh;

public:
  ConvexPolygon(vec<vec2> const &vertices);
  ~ConvexPolygon() override = default;

  GeometryData getMesh() const override { return mMesh; }
};

class Circle : public IPrimitive {
private:
  GeometryData mMesh;

public:
  Circle(float radius, u32 numSubdivision = 32);
  ~Circle() override = default;

  GeometryData getMesh() const override { return mMesh; }
};

class Point : public IPrimitive {
private:
  GeometryData mMesh;

public:
  Point(vec3 const &position);
  ~Point() override = default;

  GeometryData getMesh() const override { return mMesh; }
};

class Line : public IPrimitive {
private:
  GeometryData mMesh;

public:
  Line(vec3 const &start, vec3 const &end);
  ~Line() override = default;

  GeometryData getMesh() const override { return mMesh; }
};

class LineStrip : public IPrimitive {
private:
  GeometryData mMesh;

public:
  LineStrip(vec<vec2> const &points);
  ~LineStrip() override = default;

  GeometryData getMesh() const override { return mMesh; }
};

class Cube : public IPrimitive {
private:
  GeometryData mMesh;

public:
  Cube(float size);
  ~Cube() override = default;

  GeometryData getMesh() const override { return mMesh; }
};

// class Sphere : public IPrimitive {
// private:
//   typedef pair<u32> Edge;
//   typedef array<u32, 3> Face;

// private:
//   GeometryData mMesh;

// private:
//   struct EdgeHash {
//     size_t operator()(Edge const &edge) const {
//       return std::hash<u32>()(edge.first ^ (edge.second << 16));
//     }
//   };

//   u32 getMiddlePoint(u32 p1, u32 p2, vec<vec3> &positions,
//                      umap<Edge, u32, EdgeHash> &edgeMap) const;
//   void subdivide(vec<vec3> &positions, vec<Face> &faces);

// public:
//   Sphere(float radius, u32 numSubdivision = 4);
//   ~Sphere() override = default;

//   GeometryData getMesh() const override { return mMesh; }
// };

// class Cylinder : public IPrimitive {
// private:
//   GeometryData mMesh;

// public:
//   Cylinder(float radius, float height, u32 numSubdivision = 32);
//   ~Cylinder() override = default;

//   GeometryData getMesh() const override { return mMesh; }
// };

// class Cone : public IPrimitive {
// private:
//   GeometryData mMesh;

// public:
//   Cone(float radius, float height, u32 numSubdivision = 32);
//   ~Cone() override = default;

//   GeometryData getMesh() const override { return mMesh; }
// };

// class Torus : public IPrimitive {
// private:
//   GeometryData mMesh;

// public:
//   Torus(float radius, float tubeRadius, u32 numSubdivision = 32);
//   ~Torus() override = default;

//   GeometryData getMesh() const override { return mMesh; }
// };

// class Capsule : public IPrimitive {
// private:
//   GeometryData mMesh;

// public:
//   Capsule(float radius, float height, u32 numSubdivision = 32);
//   ~Capsule() override = default;

//   GeometryData getMesh() const override { return mMesh; }
// };

// class Plane : public IPrimitive {
// private:
//   GeometryData mMesh;

// public:
//   Plane(float width, float height, u32 numSubdivision = 32);
//   ~Plane() override = default;

//   GeometryData getMesh() const override { return mMesh; }
// };

class Mesh : public IResource {
private:
  UUID mID;
  bool mLoaded = false;
  RawMesh mRawMesh;
  GeometryData mGeometryData;
  shared<OpenGL::Buffer> mBuffer;

public:
  Mesh() { mBuffer = std::make_shared<OpenGL::Buffer>(); }
  ~Mesh() override = default;

  UUID const &getID() const override { return mID; }

  void loadMesh(GeometryData const &data);
  void draw();

public:
  static RawMesh construct(GeometryData const &data);
  static vec<float> flatten(vec<Vertex> const &vertices);
};

class MeshManager : public IResourceManager {
private:
  UUID mID;
  shared<ResourceRegistry> mRegistry;

public:
  MeshManager();
  ~MeshManager() override = default;

  UUID const &getID() const override { return mID; }

  void setRegistry(shared<ResourceRegistry> registry) override {
    mRegistry = registry;
  }
};
} // namespace Terreate::Resource
