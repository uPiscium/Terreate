#pragma once

#include "../common/type.hpp"

#include "buffer.hpp"
#include "font.hpp"
#include "shader.hpp"

namespace Terreate::OpenGL {
class Text {
private:
  bool mShaderLoaded = false;
  wstr mLastText = L"";
  wstr mText = L"";
  vec<float> mVertices;
  vec<u32> mIndices;
  Buffer mBuffer;
  umap<str, AttributeData> mAttributes = {
      {"iPosition", {0, 0, 3, 6 * sizeof(float), 0}},
      {"iUV", {0, 1, 3, 6 * sizeof(float), 3 * sizeof(float)}}};
  umap<str, u32> mLocations = {{"iPosition", 0}, {"iUV", 1}};
  Font *mFont = nullptr;
  Shader mShader;
  vec3 mColor = vec3(1.0f, 1.0f, 1.0f);

private:
  void loadText();

public:
  Text() {}
  Text(str const &text, Font *font)
      : mText(wstr(text.begin(), text.end())), mFont(font) {
    this->loadText();
  }
  Text(wstr const &text, Font *font) : mText(text), mFont(font) {
    this->loadText();
  }
  ~Text() {}

  void setColor(vec3 const &color) { mColor = color; }
  void setText(wstr const &text) { this->loadText(text); }
  void setText(str const &text) {
    this->loadText(wstr(text.begin(), text.end()));
  }

  void loadFont(Font *font) { mFont = font; }
  void loadShader(Shader const &shader) { mShader = shader; }
  void loadShader(str const &vertexPath, str const &fragmentPath);
  void loadText(wstr const &text);
  void loadText(str const &text) {
    this->loadText(wstr(text.begin(), text.end()));
  }

  void render(float const &x, float const &y, float const &windowWidth,
              float const &windowHeight);

  Text &operator=(str const &text);
  Text &operator=(wstr const &text);
};
} // namespace Terreate::OpenGL
