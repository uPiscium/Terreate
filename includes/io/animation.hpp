#ifndef __TERREATEIO_ANIMATION_HPP__
#define __TERREATEIO_ANIMATION_HPP__

#include <core/math.hpp>
#include <core/uuid.hpp>
#include <types.hpp>

namespace Terreate::IO {
using namespace Terreate::Types;
using namespace Terreate::Math;

struct AnimationFrame {
  vec3 scaling;
  quat rotation;
  vec3 translation;
  Float time;
};

class Animation {
private:
  Vec<AnimationFrame> mFrames;
  Vec<Float> mTimes;
  Core::UUID mAnimationID;

public:
  Animation() = default;
  Animation(Animation const &animation);
  ~Animation() = default;

  /*
   * @brief: Get frame data array.
   * @return: Frame data array.
   */
  Vec<AnimationFrame> GetFrames() const { return mFrames; }
  /*
   * @brief: Get time data array.
   * @return: Time data array.
   */
  Vec<Float> const &GetTimes() const { return mTimes; }

  /*
   * @brief: Set frame data array.
   * @param: frames: Frame data array.
   * @param: times: Time data array.
   */
  void SetFrames(Vec<AnimationFrame> const &frames, Vec<Float> const &times);
  /*
   * @brief: Set frame data array.
   * @param: transforms: Transform data array.
   * @param: times: Time data array.
   */
  void SetTransforms(Vec<mat4> const &transforms, Vec<Float> const &times);

  /*
   * @brief: Add frame data.
   * @param: frame: Frame data.
   * @param: time: Time data.
   */
  void AddFrame(AnimationFrame const &frame, Float const &time);
  /*
   * @brief: Add frame data.
   * @param: transform: Transform data.
   * @param: time: Time data.
   */
  void AddTransform(mat4 const &transform, Float const &time);

public:
  /*
   * @brief: Decompose transform matrix into scaling, rotation, and translation.
   * @param: transform: Transform matrix.
   * @param: scaling: Scaling vector.
   * @param: rotation: Rotation quaternion.
   * @param: translation: Translation vector.
   */
  static void Decompose(mat4 const &transform, vec3 &scaling, quat &rotation,
                        vec3 &translation);
};
} // namespace Terreate::IO

#endif // __TERREATEIO_ANIMATION_HPP__
