#ifndef __TERREATEIO_CONTAINERS_MODEL_HPP__
#define __TERREATEIO_CONTAINERS_MODEL_HPP__

#include <core/math.hpp>
#include <io/material.hpp>
#include <io/skeleton.hpp>
#include <types.hpp>

namespace Terreate::IO {
using namespace Terreate::Types;
using namespace Terreate::Math;

typedef Vec<vec3> PositionArray;
typedef Vec<vec3> NormalArray;
typedef Vec<vec4> ColorArray;
typedef Vec<vec4> TangentArray;
typedef Vec<vec4> BitangentArray;
typedef Vec<vec2> UVArray;
typedef Vec<Vec<Uint>> ConstructionOrder;
typedef Vec<Vec<Uint>> FaceArray;

struct VertexDataSet {
  PositionArray positions;
  NormalArray normals;
  ColorArray colors;
  TangentArray tangents;
  BitangentArray bitangents;
  Vec<UVArray> uvs;
};

struct FaceDataSet {
  ConstructionOrder indices;
  FaceArray faces;
};

struct MorphDataSet {
  Vec<PositionArray> positions;
  Vec<NormalArray> normals;
  Vec<TangentArray> tangents;
};

struct JointDataSet {
  Vec<Uint> joints;
  Vec<Float> weights;
};

class Part {
private:
  VertexDataSet mVertexData;
  MorphDataSet mMorphData;
  FaceDataSet mFaceData;
  JointDataSet mJointData;
  Vec<Mapping> mTextureMappings;
  Material mMaterial;
  Skeleton mSkeleton;

public:
  Part() = default;
  Part(VertexDataSet const &vertexData, FaceDataSet const &faceData,
       JointDataSet const &jointData, Material const &material,
       Skeleton const &skeleton)
      : mVertexData(vertexData), mFaceData(faceData), mJointData(jointData),
        mMaterial(material), mSkeleton(skeleton) {}
  ~Part() = default;

  /*
   * @brief: Get vertex data of the model
   * @return: Vertex data of the model
   */
  VertexDataSet const &GetVertexData() const { return mVertexData; }
  /*
   * @brief: Get morph data of the model
   * @return: Morph data of the model
   */
  MorphDataSet const &GetMorphData() const { return mMorphData; }
  /*
   * @brief: Get face data of the model
   * @return: Face data of the model
   */
  FaceDataSet const &GetFaceData() const { return mFaceData; }
  /*
   * @brief: Get joint data of the model
   * @return: Joint data of the model
   */
  JointDataSet const &GetJointData() const { return mJointData; }
  /*
   * @brief: Get texture mappings of the model
   * @return: Texture mappings of the model
   */
  Vec<Mapping> const &GetTextureMappings() const { return mTextureMappings; }
  /*
   * @brief: Get material of the model
   * @return: Material of the model
   */
  Material const &GetMaterial() const { return mMaterial; }
  /*
   * @brief: Get skeleton of the model
   * @return: Skeleton of the model
   */
  Skeleton const &GetSkeleton() const { return mSkeleton; }

  /*
   * @brief: Set vertex data of the model
   * @param: vertexData: Vertex data of the model
   */
  void SetVertexData(VertexDataSet const &vertexData) {
    mVertexData = vertexData;
  }
  /*
   * @brief: Set morph data of the model
   * @param: morphData: Morph data of the model
   */
  void SetMorphData(MorphDataSet const &morphData) { mMorphData = morphData; }
  /*
   * @brief: Set face data of the model
   * @param: faceData: Face data of the model
   */
  void SetFaceData(FaceDataSet const &faceData) { mFaceData = faceData; }
  /*
   * @brief: Set joint data of the model
   * @param: jointData: Joint data of the model
   */
  void SetJointData(JointDataSet const &jointData) { mJointData = jointData; }
  /*
   * @brief: Set texture mappings of the model
   * @param: textureMappings: Texture mappings of the model
   */
  void SetTextureMappings(Vec<Mapping> const &textureMappings) {
    mTextureMappings = textureMappings;
  }
  /*
   * @brief: Set material of the model
   * @param: material: Material of the model
   */
  void SetMaterial(Material const &material) { mMaterial = material; }
  /*
   * @brief: Set skeleton of the model
   * @param: skeleton: Skeleton of the model
   */
  void SetSkeleton(Skeleton const &skeleton) { mSkeleton = skeleton; }
};

class Model {
private:
  Vec<Part> mParts;

public:
  Model() = default;
  Model(Vec<Part> const &parts) : mParts(parts) {}
  ~Model() = default;

  /*
   * @brief: Get parts of the model
   * @return: Parts of the model
   */
  Vec<Part> const &GetParts() const { return mParts; }
  /*
   * @brief: Get part of the model by index
   * @param: index: Index of the part
   * @return: Part of the model
   */
  Part const &GetPart(Uint index) const { return mParts.at(index); }
  /*
   * @brief: Get number of parts in the model
   * @return: Number of parts in the model
   */
  Uint GetPartCount() const { return mParts.size(); }

  /*
   * @brief: Set parts of the model
   * @param: parts: Parts of the model
   */
  void SetParts(Vec<Part> const &parts) { mParts = parts; }
  /*
   * @brief: Set part of the model by index
   * @param: index: Index of the part
   * @param: part: Part of the model
   */
  void SetPart(Uint index, Part const &part) { mParts.at(index) = part; }

  /*
   * @brief: Add part to the model
   * @param: part: Part to add
   */
  void AddPart(Part const &part) { mParts.push_back(part); }
  /*
   * @brief: Remove part from the model
   * @param: index: Index of the part to remove
   */
  void RemovePart(Uint index) { mParts.erase(mParts.begin() + index); }
};
} // namespace Terreate::IO

#endif // __TERREATEIO_CONTAINERS_MODEL_HPP__
