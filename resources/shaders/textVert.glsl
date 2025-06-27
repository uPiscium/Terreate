#version 430 core
layout(location=0) in vec3 iPosition;
layout(location=1) in vec3 iUV;

out vec3 vUV;

uniform mat4 uTransform;
uniform mat4 uModel;

void main() {
  gl_Position = uTransform * uModel * vec4(iPosition, 1.0f);
  vUV = iUV;
}
