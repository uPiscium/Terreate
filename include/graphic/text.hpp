#ifndef __TERREATE_GRAPHICS_TEXT_HPP__
#define __TERREATE_GRAPHICS_TEXT_HPP__

#include <core/type.hpp>
#include <graphic/buffer.hpp>
#include <graphic/font.hpp>
#include <graphic/shader.hpp>

namespace Terreate::Graphic {
using namespace Terreate::Types;

class Text {
private:
  bool mShaderLoaded = false;
  WStr mLastText = L"";
  WStr mText = L"";
  Vec<float> mVertices;
  Vec<u32> mIndices;
  Buffer mBuffer;
  Map<Str, AttributeData> mAttributes = {
      {"iPosition", {0, 0, 3, 6 * sizeof(float), 0}},
      {"iUV", {0, 1, 3, 6 * sizeof(float), 3 * sizeof(float)}}};
  Map<Str, u32> mLocations = {{"iPosition", 0}, {"iUV", 1}};
  Font *mFont = nullptr;
  Shader mShader;
  vec3 mColor = vec3(1.0f, 1.0f, 1.0f);

private:
  void loadText();

public:
  Text() {}
  /*
   * @brief: Construct text object
   * @param: text: Text to set
   * @param: font: Font to use
   */
  Text(Str const &text, Font *font)
      : mText(WStr(text.begin(), text.end())), mFont(font) {
    this->loadText();
  }
  /*
   * @brief: Construct text object
   * @param: text: Text to set
   * @param: font: Font to use
   */
  Text(WStr const &text, Font *font) : mText(text), mFont(font) {
    this->loadText();
  }
  ~Text() {}

  /*
   * @brief: Set color of the text
   * @param: color: Color to set
   */
  void setColor(vec3 const &color) { mColor = color; }
  /*
   * @brief: Set text
   * @param: text: Text to set
   */
  void setText(WStr const &text) { this->loadText(text); }
  /*
   * @brief: Set text
   * @param: text: Text to set
   */
  void setText(Str const &text) {
    this->loadText(WStr(text.begin(), text.end()));
  }

  void loadFont(Font *font) { mFont = font; }
  /*
   * @brief: Load shader from object
   * @param: shader: Shader object
   */
  void loadShader(Shader const &shader) { mShader = shader; }
  /*
   * @brief: Load shader from file
   * @param: vertexPath: Path to the vertex shader
   * @param: fragmentPath: Path to the fragment shader
   */
  void loadShader(Str const &vertexPath, Str const &fragmentPath);
  /*
   * @brief: Load text to the buffer
   * @param: text: Text to load
   * @note: This function needs to be called after LoadFont and LoadShader
   */
  void loadText(WStr const &text);
  /*
   * @brief: Load text to the buffer
   * @param: text: Text to load
   * @note: This function needs to be called after LoadFont and LoadShader
   */
  void loadText(Str const &text) {
    this->loadText(WStr(text.begin(), text.end()));
  }

  /*
   * @brief: Render the text
   * @param: x: X position of the text
   * @param: y: Y position of the text
   * @param: windowWidth: Width of the window
   * @param: windowHeight: Height of the window
   */
  void render(float const &x, float const &y, float const &windowWidth,
              float const &windowHeight);

  Text &operator=(Str const &text);
  Text &operator=(WStr const &text);
};
} // namespace Terreate::Graphic

#endif // __TERREATE_GRAPHICS_TEXT_HPP__
