#version 460
layout(local_size_x = 16, local_size_y = 16, local_size_z = 16) in;

struct CurveSegment {
  vec2 p0;
  vec2 p1;
  vec2 p2;
  vec2 p3;
  int type; // 0: linear, 1: quadratic, 2: cubic
};

layout(std430) buffer CurveSegments {
  CurveSegment segments[];
};
(layout rgba32f) writeonly uniform sampler2D SDFout;
uniform int layer;

bool isCrossing(vec2 p, vec2 p0, vec2 p1) {
  return ((p0.y <= p.y && p1.y > p.y) || (p0.y > p.y && p1.y <= p.y));
}

float isLeft(vec2 p0, vec2 p1, vec2 p) {
  return (p1.x - p0.x) * (p.y - p0.y) - (p1.y - p0.y) * (p.x - p0.x);
}

vec2 bezier2(vec2 p0, vec2 p1, vec2 p2, float t) {
  float u = 1.0 - t;
  return u * u * p0 + 2.0 * u * t * p1 + t * t * p2;
}

vec2 bezier3(vec2 p0, vec2 p1, vec2 p2, vec2 p3, float t) {
  float u = 1.0 - t;
  return u * u * u * p0 + 3.0 * u * u * t * p1 + 3.0 * u * t * t * p2 + t * t * t * p3;
}

float pointLineDist(vec2 p, vec2 p0, vec2 p1) {
  vec2 pa = p - p0;
  vec2 ba = p1 - p0;
  float h = clamp(dot(pa, ba) / dot(ba, ba), 0.0, 1.0);
  return length(pa - ba * h);
}

float pointConicDist(vec2 p, vec2 p0, vec2 p1, vec2 p2) {
  float minDist = 1e9;
  for (int i = 0; i <= 20; ++i) {
    float t = (float)i / 20.0;
    vec2 pt = bezier2(p0, p1, p2, t);
    minDist = min(minDist, length(p - pt));
  }
  return minDist;
}

float pointCubicDist(vec2 p, vec2 p0, vec2 p1, vec2 p2, vec2 p3) {
  float minDist = 1e9;
  for (int i = 0; i <= 20; ++i) {
    float t = (float)i / 20.0;
    vec2 pt = bezier3(p0, p1, p2, p3, t);
    minDist = min(minDist, length(p - pt));
  }
  return minDist;
}

int winding(vec2 p) {
  int w = 0;

  for (uint i = 0; i < segments.length(); ++i) {
    CurveSegment segment = segments[i];
    uint const n = 16; // Number of segments for the curve

    vec2 prev = segment.p0;
    for (uint j = 1; j <= n; ++j) {
      float t = float(j) / float(n);
      vec2 curr;

      if (segment.type == 0) { // Linear
        curr = segment.p1;
        j = n; // Skip further processing for linear segments
      } else if (segment.type == 1) { // Quadratic
        curr = bezier2(prev, segment.p1, segment.p2, t);
      } else if (segment.type == 2) { // Cubic
        curr = bezier3(prev, segment.p1, segment.p2, segment.p3, t);
      }

      if (isCrossing(p, prev, curr)) {
        float side = isLeft(prev, curr, p);
        if (curr.y > prev.y && side > 0.0) {
          w++;
        } else if (curr.y <= prev.y && side < 0.0) {
          w--;
        }
      }
      prev = curr;
    }
  }

  return w;
}

void main() {
  ivec2 coord = ivec2(gl_GlobalInvocationID.xy);
  ivec2 size = imageSize(SDLout);
  vec2 uv = vec2(coord) / vec2(size) * 1000.0; // Scale to 1000 units

  float minDist = 1e9;
  for (int i = 0; i < segments.length(); ++i) {
    CurveSegment segment = segments[i];
    float dist = 0.0;

    if (segment.type == 0) { // Linear
      dist = pointLineDist(uv, segment.p0, segment.p1);
    } else if (segment.type == 1) { // Quadratic
      dist = pointConicDist(uv, segment.p0, segment.p1, segment.p2);
    } else if (segment.type == 2) { // Cubic
      dist = pointCubicDist(uv, segment.p0, segment.p1, segment.p2, segment.p3);
    }

    minDist = min(minDist, dist);
  }

  int w = winding(uv);
  float s = w == 0 ? 1.0 : -1.0;
  float dist = clamp(s * minDist / 20.0 + 0.5, 0.0, 1.0);
  imageStore(SDFout, coord, vec4(dist, dist, dist, 1.0));
}
