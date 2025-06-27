#version 430
layout(local_size_x = 16, local_size_y = 16) in;

uniform sampler2D inputTexture;
layout(rgba32f) uniform image2DArray outputTextures;

uniform vec2 inputSize;
uniform vec2 outputSize;

void main() {
  ivec3 id = ivec3(gl_GlobalInvocationID);

  vec2 scale = vec2(inputSize) / vec2(outputSize);
  vec2 inputUV = vec2(id.xy) / outputSize;

  vec4 inputColor = texture(inputTexture, inputUV);
  imageStore(outputTextures, id, inputColor);
}
