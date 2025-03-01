#ifndef __TERREATEIO_CONTAINERS_SKELETON_HPP__
#define __TERREATEIO_CONTAINERS_SKELETON_HPP__

#include <core/math.hpp>
#include <core/uuid.hpp>
#include <types.hpp>

namespace Terreate::IO {
using namespace Terreate::Types;
using namespace Terreate::Math;

struct Joint {
  Uint child = 0u;
  Uint sibling = 0u;
  mat4 inverseBindMatrix = identity<mat4>();
  mat4 initialPose = identity<mat4>();
};

class Skeleton {
private:
  Vec<Joint> mJoints;          // Root joint is always the first element.
  Vec<Uint> mJointConnections; // Each element is the index of the parent joint.
  Core::UUID mSkeletonID;

public:
  Skeleton() = default;
  Skeleton(Skeleton const &skeleton);
  ~Skeleton() = default;

  /*
   * @brief: Get joint data array.
   * @return: Joint data array.
   */
  Vec<Joint> const &GetJoints() const { return mJoints; }
  /*
   * @brief: Get number of joints.
   * @return: Number of joints.
   */
  Uint GetJointCount() const { return mJoints.size(); }
  /*
   * @brief: Get joint connections.
   * @return: Joint connections.
   */
  Vec<Uint> const &GetJointConnections() const { return mJointConnections; }
  /*
   * @brief: Get joint data.
   * @param: index: Index of joint data.
   * @return: Joint data.
   */
  Joint const &GetJoint(Uint index) const { return mJoints[index]; }

  /*
   * @brief: Add joint data.
   * @param: joint: Joint data.
   */
  void AddJoint(Joint const &joint) { mJoints.push_back(joint); }
};
} // namespace Terreate::IO

#endif // __TERREATEIO_CONTAINERS_SKELETON_HPP__
