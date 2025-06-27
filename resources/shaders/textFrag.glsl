#version 430 core
in vec3 vUV;

out vec4 fragColor;

uniform sampler2DArray uTexture;
uniform vec3 uColor;

void main() {
  float alpha = texture(uTexture, vUV).r;
  if (alpha < 0.1) {
    discard;
  }
  fragColor = vec4(uColor, alpha);
}
