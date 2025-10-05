#version 450 core

layout(binding = 0) uniform UniformBufferObject {
    mat4 model;
    mat4 view;
    mat4 proj;
} ubo;

layout(location = 0) in vec2 inPosition;
layout(location = 1) in vec3 inColor;
layout(location = 0) out vec3 fragColor;

void main() {
  mat4 mvp = ubo.proj * ubo.view * ubo.model;
  gl_Position = mvp * vec4(inPosition, 0.0, 1.0);
  fragColor = inColor;
}
