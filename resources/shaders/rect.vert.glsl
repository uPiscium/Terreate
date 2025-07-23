#version 460 core
layout(location=0) in vec4 vPosition;
layout(location=1) in vec4 vMainUV;
layout(location=2) in vec4 vLightingUV;
layout(location=3) in vec4 vTilingUV;
layout(location=4) in vec4 vNormal;
layout(location=5) in vec4 vTangent;
layout(location=6) in vec4 vBitangent;

out vec3 fMainUV;

// layout(std140) uniform Matrices {
//   mat4 uTransform;
//   mat4 uModel;
//   mat4 uView;
//   mat4 uProjection;
// };

void main() {
  gl_Position = vec4(vPosition.xyz, 1.0f);
  fMainUV = vMainUV.xyz;
}
