#version 450 core

layout(binding = 1) uniform sampler2D texSampler;

layout(location = 0) in vec3 fragColor;
layout(location = 1) in vec2 fragTexCoord;

layout(location = 0) out vec4 outColor;

void main() {
  // outColor = vec4(fragColor, 1.0);
  // outColor = vec4(fragTexCoord, 0.0, 1.0);
  vec3 color = texture(texSampler, fragTexCoord).rgb;
  // outColor = vec4(color * fragColor, 1.0);
  outColor = vec4(color, 1.0);
}
