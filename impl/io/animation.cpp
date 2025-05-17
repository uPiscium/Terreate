#include <io/animation.hpp>

namespace Terreate::IO {
void Animation::setFrames(Vec<AnimationFrame> const &frames,
                          Vec<float> const &times) {
  mFrames = frames;
  mTimes = times;
}

void Animation::setTransforms(Vec<mat4> const &transforms,
                              Vec<float> const &times) {
  for (int i = 0; i < transforms.size(); ++i) {
    this->addTransform(transforms[i], times[i]);
  }
}

void Animation::addFrame(AnimationFrame const &frame, float const &time) {
  mFrames.push_back(frame);
  mTimes.push_back(time);
}

void Animation::addTransform(mat4 const &transform, float const &time) {
  vec3 scaling;
  quat rotation;
  vec3 translation;

  Animation::decompose(transform, scaling, rotation, translation);
  AnimationFrame frame = {scaling, rotation, translation};
  mFrames.push_back(frame);
  mTimes.push_back(time);
}

void Animation::decompose(mat4 const &transform, vec3 &scaling, quat &rotation,
                          vec3 &translation) {
  scaling.x = length(vec3(transform[0]));
  scaling.y = length(vec3(transform[1]));
  scaling.z = length(vec3(transform[2]));

  mat3 rotationMatrix = mat3(transform);
  rotation = quat_cast(rotationMatrix);

  translation = vec4(transform[3]);
}
} // namespace Terreate::IO
