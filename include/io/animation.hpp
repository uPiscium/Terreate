#ifndef __TERREATEIO_ANIMATION_HPP__
#define __TERREATEIO_ANIMATION_HPP__

#include <core/math.hpp>
#include <core/type.hpp>
#include <core/uuid.hpp>

namespace Terreate::IO {
using namespace Terreate::Types;
using namespace Terreate::Math;

struct AnimationFrame {
  vec3 scaling;
  quat rotation;
  vec3 translation;
  float time;
};

class Animation {
private:
  Vec<AnimationFrame> mFrames;
  Vec<float> mTimes;
  Core::UUID mAnimationID;

public:
  Animation() = default;
  Animation(Animation const &animation);
  ~Animation() = default;

  /*
   * @brief: Get frame data array.
   * @return: Frame data array.
   */
  Vec<AnimationFrame> getFrames() const { return mFrames; }
  /*
   * @brief: Get time data array.
   * @return: Time data array.
   */
  Vec<float> const &getTimes() const { return mTimes; }

  /*
   * @brief: Set frame data array.
   * @param: frames: Frame data array.
   * @param: times: Time data array.
   */
  void setFrames(Vec<AnimationFrame> const &frames, Vec<float> const &times);
  /*
   * @brief: Set frame data array.
   * @param: transforms: Transform data array.
   * @param: times: Time data array.
   */
  void setTransforms(Vec<mat4> const &transforms, Vec<float> const &times);

  /*
   * @brief: Add frame data.
   * @param: frame: Frame data.
   * @param: time: Time data.
   */
  void addFrame(AnimationFrame const &frame, float const &time);
  /*
   * @brief: Add frame data.
   * @param: transform: Transform data.
   * @param: time: Time data.
   */
  void addTransform(mat4 const &transform, float const &time);

public:
  /*
   * @brief: Decompose transform matrix into scaling, rotation, and translation.
   * @param: transform: Transform matrix.
   * @param: scaling: Scaling vector.
   * @param: rotation: Rotation quaternion.
   * @param: translation: Translation vector.
   */
  static void decompose(mat4 const &transform, vec3 &scaling, quat &rotation,
                        vec3 &translation);
};
} // namespace Terreate::IO

#endif // __TERREATEIO_ANIMATION_HPP__
