#version 430 core
in vec3 vNormal;
in vec2 vUV;
in vec3 vColor;

out vec4 fragColor;

uniform sampler2DArray uTexture;

layout(std430) buffer Settings {
  vec4 info;
};

vec4 sampleTexture(sampler2DArray target, vec2 uv) {
  if (uv.x < 0.5f) {
    if (uv.y < 0.5f) {
      return texture(target, vec3(uv, 0));
    } else {
      return texture(target, vec3(uv, 1));
    }
  } else {
    if (uv.y < 0.5f) {
      return texture(target, vec3(uv, 2));
    } else {
      return texture(target, vec3(uv, 3));
    }
  }
}

vec4 sampleFrom0(sampler2DArray target, vec2 uv) {
  return texture(target, vec3(uv, 0));
}

vec4 sampleFrom(sampler2DArray target, vec2 uv, int layer) {
  return texture(target, vec3(uv, layer));
}

void main() {
  // vec4 color = sampleTexture(uTexture, vUV);
  vec4 color = sampleFrom(uTexture, vUV, 0);
  if (color.a < info.a) {
    discard;
  }
  fragColor = color * vec4(info.rgb, 1.0);
  // fragColor = color * factor.z;
}
