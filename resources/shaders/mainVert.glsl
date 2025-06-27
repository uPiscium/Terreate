#version 430 core
layout(location=0) in vec3 iPosition;
layout(location=1) in vec2 iUV;
layout(location=2) in vec3 iColor;

out vec3 vNormal;
out vec2 vUV;
out vec3 vColor;

layout(std140) uniform Matrices {
  mat4 uTransform;
  mat4 uModel;
  mat4 uView;
  mat4 uProjection;
};

void main() {
  gl_Position = uProjection * uView * uTransform * uModel * vec4(iPosition, 1.0f);
  vNormal = vec3(0.0f, 0.0f, 0.0f);
  vUV = iUV;
  vColor = iColor;
}
