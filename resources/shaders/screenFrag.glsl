#version 430 core
in vec2 vUV;

out vec4 fragColor[4];

uniform sampler2DArray tex;

void main() {
  // fragColor[0] = vec4(1.0, 0.0, 0.0, 1.0);
  fragColor[0] = texture(tex, vec3(vUV, 1));
  fragColor[1] = vec4(0.0, 1.0, 0.0, 1.0);
  fragColor[2] = vec4(0.0, 0.0, 1.0, 1.0);
  fragColor[3] = vec4(1.0, 1.0, 1.0, 1.0);
}
