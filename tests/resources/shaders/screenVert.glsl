#version 430 core
in vec3 iPosition;
in vec2 iUV;

out vec2 vUV;

layout(std140) uniform Matrices {
  mat4 uTransform;
  mat4 uModel;
  mat4 uView;
  mat4 uProjection;
};

void main() {
  gl_Position = vec4(iPosition, 1.0f);
  vUV = iUV;
}
