#version 430
layout(local_size_x = 1) in;

layout(std430) buffer InputBuffer {
    float inputData[];
};

layout(std430) buffer OutputBuffer {
    float outputData[];
};

uniform float scaleFactor;

void main() {
    uint index = gl_GlobalInvocationID.x;
    outputData[index] = int(float(inputData[index]) * scaleFactor);
}
