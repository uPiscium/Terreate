#include <io/animation.hpp>

namespace Terreate::IO {
void Animation::SetFrames(Vec<AnimationFrame> const &frames,
                          Vec<Float> const &times) {
  mFrames = frames;
  mTimes = times;
}

void Animation::SetTransforms(Vec<mat4> const &transforms,
                              Vec<Float> const &times) {
  for (int i = 0; i < transforms.size(); ++i) {
    this->AddTransform(transforms[i], times[i]);
  }
}

void Animation::AddFrame(AnimationFrame const &frame, Float const &time) {
  mFrames.push_back(frame);
  mTimes.push_back(time);
}

void Animation::AddTransform(mat4 const &transform, Float const &time) {
  vec3 scaling;
  quat rotation;
  vec3 translation;

  Animation::Decompose(transform, scaling, rotation, translation);
  AnimationFrame frame = {scaling, rotation, translation};
  mFrames.push_back(frame);
  mTimes.push_back(time);
}

void Animation::Decompose(mat4 const &transform, vec3 &scaling, quat &rotation,
                          vec3 &translation) {
  scaling.x = length(vec3(transform[0]));
  scaling.y = length(vec3(transform[1]));
  scaling.z = length(vec3(transform[2]));

  mat3 rotationMatrix = mat3(transform);
  rotation = quat_cast(rotationMatrix);

  translation = vec4(transform[3]);
}
} // namespace Terreate::IO
