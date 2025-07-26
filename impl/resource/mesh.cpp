#include <cmath>
#include <resource/mesh.hpp>
#include <resource/registry.hpp>

namespace Terreate::Resource {
Triangle::Triangle(vec2 const &a, vec2 const &b, vec2 const &c) {
  mMesh.position = {vec3(a, 0.0f), vec3(b, 0.0f), vec3(c, 0.0f)};
  mMesh.mainUV = {vec3(0.0f, 0.0f, 0.0f), vec3(1.0f, 0.0f, 0.0f),
                  vec3(0.5f, 1.0f, 0.0f)};
  mMesh.normal = {vec3(0.0f, 0.0f, 1.0f)};
  mMesh.tangent = {vec4(1.0f, 0.0f, 0.0f, 0.0f)};
  mMesh.bitangent = {vec4(0.0f, 1.0f, 0.0f, 0.0f)};
  mMesh.vertices = {
      {0, 0, -1, -1, 0, 0, 0},
      {1, 1, -1, -1, 0, 0, 0},
      {2, 2, -1, -1, 0, 0, 0},
  };
  mMesh.indices = {0, 1, 2};

  mMesh.flags |= MeshFlags::POSITION;
  mMesh.flags |= MeshFlags::MAIN_UV;
  mMesh.flags |= MeshFlags::NORMAL;
  mMesh.flags |= MeshFlags::TANGENT;
  mMesh.flags |= MeshFlags::BITANGENT;
}

Rectangle::Rectangle(float width, float height) {
  mMesh.position = {
      vec3(-width / 2, -height / 2, 0.0f), // Bottom-left
      vec3(width / 2, -height / 2, 0.0f),  // Bottom-right
      vec3(width / 2, height / 2, 0.0f),   // Top-right
      vec3(-width / 2, height / 2, 0.0f)   // Top-left
  };
  mMesh.mainUV = {
      vec3(0.0f, 0.0f, 0.0f), // Bottom-left
      vec3(1.0f, 0.0f, 0.0f), // Bottom-right
      vec3(1.0f, 1.0f, 0.0f), // Top-right
      vec3(0.0f, 1.0f, 0.0f)  // Top-left
  };
  mMesh.normal = {vec3(0.0f, 0.0f, 1.0f)};
  mMesh.tangent = {vec4(1.0f, 0.0f, 0.0f, 0.0f)};
  mMesh.bitangent = {vec4(0.0f, 1.0f, 0.0f, 0.0f)};
  mMesh.vertices = {
      {0, 0, -1, -1, 0, 0, 0}, // Bottom-left
      {1, 1, -1, -1, 0, 0, 0}, // Bottom-right
      {2, 2, -1, -1, 0, 0, 0}, // Top-right
      {3, 3, -1, -1, 0, 0, 0}  // Top-left
  };
  mMesh.indices = {0, 1, 2, 2, 3, 0};

  mMesh.flags |= MeshFlags::POSITION;
  mMesh.flags |= MeshFlags::MAIN_UV;
  mMesh.flags |= MeshFlags::NORMAL;
  mMesh.flags |= MeshFlags::TANGENT;
  mMesh.flags |= MeshFlags::BITANGENT;
}

ConvexPolygon::ConvexPolygon(vec<vec2> const &vertices) {
  mMesh.position.reserve(vertices.size());
  for (auto const &v : vertices) {
    mMesh.position.push_back(vec3(v, 0.0f));
  }

  mMesh.normal = {vec3(0.0f, 0.0f, 1.0f)};
  mMesh.tangent = {vec4(1.0f, 0.0f, 0.0f, 0.0f)};
  mMesh.bitangent = {vec4(0.0f, 1.0f, 0.0f, 0.0f)};
  mMesh.vertices.reserve(vertices.size());
  for (i32 i = 0; i < vertices.size(); ++i) {
    // UV coordinates are not defined for this primitive
    mMesh.vertices.push_back({i, -1, -1, -1, 0, 0, 0});
  }

  for (u32 i = 1; i < vertices.size() - 1; ++i) {
    mMesh.indices.push_back(0);     // First vertex
    mMesh.indices.push_back(i);     // Current vertex
    mMesh.indices.push_back(i + 1); // Next vertex
  }

  mMesh.flags |= MeshFlags::POSITION;
  mMesh.flags |= MeshFlags::NORMAL;
  mMesh.flags |= MeshFlags::TANGENT;
  mMesh.flags |= MeshFlags::BITANGENT;
}

Circle::Circle(float radius, u32 numSubdivision) {
  u32 const numVertices = 1 << numSubdivision; // 2^numSubdivision vertices
  mMesh.position.reserve(numVertices);
  mMesh.indices.reserve(numVertices * 3);

  for (u32 i = 0; i < numVertices; ++i) {
    float angle = (2.0f * PI * i) / numVertices;
    mMesh.position.push_back(
        vec3(radius * cos(angle), radius * sin(angle), 0.0f));
  }
  mMesh.normal = {vec3(0.0f, 0.0f, 1.0f)};
  mMesh.tangent = {vec4(1.0f, 0.0f, 0.0f, 0.0f)};
  mMesh.bitangent = {vec4(0.0f, 1.0f, 0.0f, 0.0f)};

  for (i32 i = 0; i < numVertices; ++i) {
    mMesh.vertices.push_back({i, -1, -1, -1, 0, 0, 0});
  }

  for (i32 i = 1; i < numVertices - 1; ++i) {
    mMesh.indices.push_back(0);     // First vertex
    mMesh.indices.push_back(i);     // Current vertex
    mMesh.indices.push_back(i + 1); // Next vertex
  }

  mMesh.flags |= MeshFlags::POSITION;
  mMesh.flags |= MeshFlags::NORMAL;
  mMesh.flags |= MeshFlags::TANGENT;
  mMesh.flags |= MeshFlags::BITANGENT;
}

Point::Point(vec3 const &position) {
  mMesh.position = {position};
  mMesh.normal = {vec3(0.0f, 0.0f, 1.0f)};
  mMesh.tangent = {vec4(1.0f, 0.0f, 0.0f, 0.0f)};
  mMesh.bitangent = {vec4(0.0f, 1.0f, 0.0f, 0.0f)};
  mMesh.vertices = {{0, -1, -1, -1, 0, 0, 0}};
  mMesh.indices = {0};

  mMesh.drawMode = DrawMode::POINTS;

  mMesh.flags |= MeshFlags::POSITION;
  mMesh.flags |= MeshFlags::NORMAL;
  mMesh.flags |= MeshFlags::TANGENT;
  mMesh.flags |= MeshFlags::BITANGENT;
}

Line::Line(vec3 const &start, vec3 const &end) {
  mMesh.position = {start, end};
  mMesh.normal = {vec3(0.0f, 0.0f, 1.0f)};
  mMesh.tangent = {vec4(1.0f, 0.0f, 0.0f, 0.0f)};
  mMesh.bitangent = {vec4(0.0f, 1.0f, 0.0f, 0.0f)};
  mMesh.vertices = {{0, -1, -1, -1, 0, 0, 0}, {1, -1, -1, -1, 0, 0, 0}};
  mMesh.indices = {0, 1};

  mMesh.drawMode = DrawMode::LINES;

  mMesh.flags |= MeshFlags::POSITION;
  mMesh.flags |= MeshFlags::NORMAL;
  mMesh.flags |= MeshFlags::TANGENT;
  mMesh.flags |= MeshFlags::BITANGENT;
}

Cube::Cube(float size) {
  float halfSize = size / 2.0f;

  mMesh.position = {
      vec3(-halfSize, -halfSize, halfSize),  // Bottom-left-front
      vec3(halfSize, -halfSize, halfSize),   // Bottom-right-front
      vec3(halfSize, halfSize, halfSize),    // Top-right-front
      vec3(-halfSize, halfSize, halfSize),   // Top-left-front
      vec3(-halfSize, -halfSize, -halfSize), // Bottom-left-back
      vec3(halfSize, -halfSize, -halfSize),  // Bottom-right-back
      vec3(halfSize, halfSize, -halfSize),   // Top-right-back
      vec3(-halfSize, halfSize, -halfSize),  // Top-left-back
  };
  mMesh.mainUV = {
      vec3(0.0f, 0.0f, 0.0f), // Bottom-left
      vec3(1.0f, 0.0f, 0.0f), // Bottom-right
      vec3(1.0f, 1.0f, 0.0f), // Top-right
      vec3(0.0f, 1.0f, 0.0f), // Top-left
  };
  mMesh.normal = {
      vec3(0.0f, 0.0f, -1.0f), // Back face
      vec3(0.0f, 0.0f, 1.0f),  // Front face
      vec3(-1.0f, 0.0f, 0.0f), // Left face
      vec3(1.0f, 0.0f, 0.0f),  // Right face
      vec3(0.0f, -1.0f, 0.0f), // Bottom face
      vec3(0.0f, 1.0f, 0.0f)   // Top face
  };
  mMesh.tangent = {
      vec4(1.0f, 0.0f, 0.0f, 0.0f),  // Back face
      vec4(-1.0f, 0.0f, 0.0f, 0.0f), // Front face
      vec4(0.0f, 1.0f, 0.0f, 0.0f),  // Left face
      vec4(0.0f, -1.0f, 0.0f, 0.0f), // Right face
      vec4(1.0f, 0.0f, 0.0f, 0.0f),  // Bottom face
      vec4(1.0f, 0.0f, 0.0f, 0.0f)   // Top face
  };
  mMesh.bitangent = {
      vec4(0.0f, 1.0f, 0.0f, 0.0f),  // Back face
      vec4(0.0f, -1.0f, 0.0f, 0.0f), // Front face
      vec4(0.0f, 0.0f, 1.0f, 0.0f),  // Left face
      vec4(0.0f, 0.0f, -1.0f, 0.0f), // Right face
      vec4(0.0f, 1.0f, 0.0f, 0.0f),  // Bottom face
      vec4(1.0f, 1.0f, 1.0f, 1.0f)   // Top face
  };

  mMesh.vertices = {
      {0, 0, -1, -1, 1, 1, 1}, {1, 1, -1, -1, 1, 1, 1},
      {2, 2, -1, -1, 1, 1, 1}, {3, 3, -1, -1, 1, 1, 1},

      {1, 0, -1, -1, 3, 3, 3}, {5, 1, -1, -1, 3, 3, 3},
      {6, 2, -1, -1, 3, 3, 3}, {2, 3, -1, -1, 3, 3, 3},

      {5, 0, -1, -1, 0, 0, 0}, {4, 1, -1, -1, 0, 0, 0},
      {7, 2, -1, -1, 0, 0, 0}, {6, 3, -1, -1, 0, 0, 0},

      {4, 0, -1, -1, 2, 2, 2}, {0, 1, -1, -1, 2, 2, 2},
      {3, 2, -1, -1, 2, 2, 2}, {7, 3, -1, -1, 2, 2, 2},

      {0, 0, -1, -1, 4, 4, 4}, {4, 1, -1, -1, 4, 4, 4},
      {5, 2, -1, -1, 4, 4, 4}, {1, 3, -1, -1, 4, 4, 4},

      {2, 0, -1, -1, 5, 5, 5}, {6, 1, -1, -1, 5, 5, 5},
      {7, 2, -1, -1, 5, 5, 5}, {3, 3, -1, -1, 5, 5, 5},
  };

  mMesh.indices = {0,  1,  2,  2,  3,  0,  4,  5,  6,  6,  7,  4,
                   8,  9,  10, 10, 11, 8,  12, 13, 14, 14, 15, 12,
                   16, 17, 18, 18, 19, 16, 20, 21, 22, 22, 23, 20};

  mMesh.flags |= MeshFlags::POSITION;
  mMesh.flags |= MeshFlags::MAIN_UV;
  mMesh.flags |= MeshFlags::NORMAL;
  mMesh.flags |= MeshFlags::TANGENT;
  mMesh.flags |= MeshFlags::BITANGENT;
}

// u32 Sphere::getMiddlePoint(u32 p1, u32 p2, vec<vec3> &positions,
//                            umap<Edge, u32, EdgeHash> &edgeMap) const {
//   Edge key = std::minmax(p1, p2);
//   auto it = edgeMap.find(key);
//   if (it != edgeMap.end()) {
//     return it->second; // Return the cached middle point index
//   }

//   vec3 middle = normalize((positions[p1] + positions[p2]) / 2.0f);
//   u32 index = positions.size();
//   positions.push_back(middle);
//   edgeMap[key] = index; // Cache the new middle point index
//   return index;
// }

// void Sphere::subdivide(vec<vec3> &positions, vec<Face> &faces) {
//   umap<Edge, u32, EdgeHash> cache;
//   vec<Face> newFaces;
//   for (auto const &tri : faces) {
//     u32 a = getMiddlePoint(tri[0], tri[1], positions, cache);
//     u32 b = getMiddlePoint(tri[1], tri[2], positions, cache);
//     u32 c = getMiddlePoint(tri[2], tri[0], positions, cache);

//     newFaces.push_back({tri[0], a, c});
//     newFaces.push_back({tri[1], b, a});
//     newFaces.push_back({tri[2], c, b});
//     newFaces.push_back({a, b, c});
//   }
//   faces = std::move(newFaces);
// }

// Sphere::Sphere(float radius, u32 numSubdivision) {
//   float PHI = (1.0f + std::sqrt(5.0f)) * 0.5f;
//   float invLen = 1.0f / std::sqrt(1.0f + PHI * PHI);
//   float x = PHI * invLen;
//   float z = 1.0f * invLen;

//   mMesh.position = {{-x, 0, z}, {x, 0, z},  {-x, 0, -z}, {x, 0, -z},
//                     {0, z, x},  {0, z, -x}, {0, -z, x},  {0, -z, -x},
//                     {z, x, 0},  {-z, x, 0}, {z, -x, 0},  {-z, -x, 0}};

//   vec<Face> faces = {{0, 4, 1},  {0, 9, 4},  {9, 5, 4},  {4, 5, 8},
//                      {4, 8, 1},  {8, 10, 1}, {8, 3, 10}, {5, 3, 8},
//                      {5, 2, 3},  {2, 7, 3},  {7, 10, 3}, {7, 6, 10},
//                      {7, 11, 6}, {11, 0, 6}, {0, 1, 6},  {6, 1, 10},
//                      {9, 0, 11}, {9, 11, 2}, {9, 2, 5},  {7, 2, 11}};

//   for (u32 i = 0; i < numSubdivision; ++i) {
//     this->subdivide(mMesh.position, faces);
//   }

//   mMesh.normal.clear();
//   mMesh.vertices.clear();
//   mMesh.indices.clear();
//   umap<u32, u32> indexMap;
//   for (auto const &face : faces) {
//     for (u32 i : face) {
//       if (indexMap.count(i) == 0) {
//         vec3 pos = mMesh.position[i] * radius;
//         mMesh.normal.push_back(pos); // position is already normalized
//         indexMap[i] = (u32)mMesh.indices.size();
//         mMesh.vertices.push_back(
//             {(i32)i, -1, -1, -1, (i32)mMesh.normal.size() - 1, -1, -1});
//       }
//       mMesh.indices.push_back(indexMap[i]);
//     }
//   }

//   mMesh.drawMode = DrawMode::TRIANGLES;
//   mMesh.flags |= MeshFlags::POSITION;
//   mMesh.flags |= MeshFlags::NORMAL;
// }

// Cylinder::Cylinder(float radius, float height, u32 numSubdivision) {
//   float halfHeight = height / 2.0f;

//   // Side
//   for (u32 i = 0; i < numSubdivision; ++i) {
//     float angle = (TWO_PI * i) / numSubdivision;
//     float x = cos(angle);
//     float z = sin(angle);

//     mMesh.position.push_back(
//         vec3(radius * x, -halfHeight, radius * z)); // Bottom circle
//     mMesh.position.push_back(
//         vec3(radius * x, halfHeight, radius * z)); // Top circle
//     mMesh.normal.push_back(vec3(x, 0.0f, z));      // Normal for side

//     i32 normal = (i32)mMesh.normal.size() - 1;
//     i32 idx0 = (i32)i * 2;
//     mMesh.vertices.push_back({idx0, -1, -1, -1, normal, -1, -1});
//     mMesh.vertices.push_back({idx0 + 1, -1, -1, -1, normal, -1, -1});
//     mMesh.vertices.push_back({idx0 + 2, -1, -1, -1, normal, -1, -1});
//     mMesh.vertices.push_back({idx0 + 3, -1, -1, -1, normal, -1, -1});

//     mMesh.indices.push_back(idx0);
//     mMesh.indices.push_back(idx0 + 1);
//     mMesh.indices.push_back(idx0 + 2);
//     mMesh.indices.push_back(idx0 + 2);
//     mMesh.indices.push_back(idx0 + 1);
//     mMesh.indices.push_back(idx0 + 3);
//   }

//   u32 numVertices = (u32)mMesh.position.size();
//   mMesh.normal.push_back(vec3(0.0f, 1.0f, 0.0f));
//   mMesh.normal.push_back(vec3(0.0f, -1.0f, 0.0f));
//   for (i32 i = 0; i < numSubdivision; ++i) {
//     mMesh.vertices.push_back(
//         {i * 2, -1, -1, -1, (i32)mMesh.normal.size() - 2, 0, 0});
//     mMesh.vertices.push_back(
//         {i * 2, -1, -1, -1, (i32)mMesh.normal.size() - 1, 0, 0});
//   }

//   for (u32 i = 1; i < numSubdivision - 1; ++i) {
//     // Bottom circle
//     mMesh.indices.push_back(numVertices);
//     mMesh.indices.push_back(numVertices + i * 2);
//     mMesh.indices.push_back(numVertices + (i + 1) * 2);

//     // Top circle
//     mMesh.indices.push_back(numVertices + 1);
//     mMesh.indices.push_back(numVertices + i * 2 + 1);
//     mMesh.indices.push_back(numVertices + (i + 1) * 2 + 1);
//   }
// }

// Cone::Cone(float radius, float height, u32 numSubdivision) {}

// Torus::Torus(float radius, float tubeRadius, u32 numSubdivision) {}

// Capsule::Capsule(float radius, float height, u32 numSubdivision) {}

// Plane::Plane(float width, float height, u32 numSubdivision) {}

void Mesh::loadMesh(GeometryData const &data) {
  mRawMesh = Mesh::construct(data);
  vec<float> buffer = Mesh::flatten(mRawMesh.vertices);
  if (!mLoaded) {
    mBuffer->loadData(buffer);
    mBuffer->loadIndices(mRawMesh.indices);
    mLoaded = true;
  } else {
    mBuffer->reloadData(buffer);
    mBuffer->reloadIndices(mRawMesh.indices);
  }
}

void Mesh::draw() {
  if (mRawMesh.vertices.empty() || mRawMesh.indices.empty()) {
    return; // Nothing to draw
  }
  mBuffer->draw(mRawMesh.drawMode);
}

RawMesh Mesh::construct(GeometryData const &data) {
  RawMesh mesh;
  Vertex vertex;

  for (auto const &set : data.vertices) {
    vertex.position = data.flags & MeshFlags::POSITION
                          ? vec4(data.position[set.position], 1.0f)
                          : vec4(0.0f);
    vertex.mainUV = data.flags & MeshFlags::MAIN_UV
                        ? vec4(data.mainUV[set.mainUV], 1.0f)
                        : vec4(0.0f);
    vertex.lightingUV = data.flags & MeshFlags::LIGHTING_UV
                            ? vec4(data.lightingUV[set.lightingUV], 1.0f)
                            : vec4(0.0f);
    vertex.tilingUV = data.flags & MeshFlags::TILING_UV
                          ? vec4(data.tilingUV[set.tilingUV], 1.0f)
                          : vec4(0.0f);
    vertex.normal = data.flags & MeshFlags::NORMAL
                        ? vec4(data.normal[set.normal], 1.0f)
                        : vec4(0.0f);
    vertex.tangent = data.flags & MeshFlags::TANGENT ? data.tangent[set.tangent]
                                                     : vec4(0.0f);
    vertex.bitangent = data.flags & MeshFlags::BITANGENT
                           ? data.bitangent[set.bitangent]
                           : vec4(0.0f);
    mesh.vertices.push_back(vertex);
  }
  mesh.indices = data.indices;
  mesh.flags = data.flags;
  mesh.drawMode = data.drawMode;

  return mesh;
}

vec<float> Mesh::flatten(vec<Vertex> const &vertices) {
  vec<float> flatData;
  flatData.reserve(vertices.size() * sizeof(Vertex) / sizeof(float));

  for (auto const &v : vertices) {
    flatData.insert(flatData.end(), (float *)&v,
                    (float *)&v + sizeof(Vertex) / sizeof(float));
  }
  return flatData;
}

shared<Mesh> MeshManager::get(UUID const &id) const {
  if (this->has(id)) {
    return mRegistry->get<Mesh>(id);
  }
  return nullptr;
}

shared<Mesh> MeshManager::create() {
  shared<Mesh> mesh = std::make_shared<Mesh>();
  mRegistry->load(mesh);
  return mesh;
}

shared<Mesh> MeshManager::create(GeometryData const &data) {
  shared<Mesh> mesh = this->create();
  mesh->loadMesh(data);
  return mesh;
}

void MeshManager::destroy(UUID const &id) {
  if (this->has(id)) {
    mRegistry->unload<Mesh>(id);
  }
}
} // namespace Terreate::Resource
