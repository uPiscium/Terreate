#include <core/math.hpp>
#include <graphic/text.hpp>

namespace Terreate::Graphic {
using namespace Terreate::Math;

void Text::loadText() {
  if (mText == mLastText) {
    return;
  }

  mFont->loadText(mText);
  auto chars = mFont->getCharacters(mText);

  Vec<Vec<float>> vertices;
  Vec<u32> indices;
  float px = 0.0f;
  u32 count = 0;
  for (auto &chr : chars) {
    if (chr.codepoint == 0) { // Skip dummy characters
      px += chr.advance >> 6;
      continue;
    }

    u32 c = chr.codepoint;
    if (c == TC_UNICODE_HALF_SPACE ||
        c == TC_UNICODE_FULL_SPACE) { // Skip spaces
      px += chr.advance >> 6;
      continue;
    }

    float w = chr.size.first;
    float h = chr.size.second;
    float x = px + chr.bearing.first;
    float y = chr.bearing.second - h;
    vertices.push_back({x, y + h, 0.0f});
    vertices.push_back({chr.uv[0], chr.uv[1], chr.uv[4]});
    vertices.push_back({x, y, 0.0f});
    vertices.push_back({chr.uv[0], chr.uv[3], chr.uv[4]});
    vertices.push_back({x + w, y, 0.0f});
    vertices.push_back({chr.uv[2], chr.uv[3], chr.uv[4]});
    vertices.push_back({x + w, y + h, 0.0f});
    vertices.push_back({chr.uv[2], chr.uv[1], chr.uv[4]});
    indices.push_back(count);
    indices.push_back(count + 1);
    indices.push_back(count + 2);
    indices.push_back(count + 2);
    indices.push_back(count + 3);
    indices.push_back(count);
    count += 4;
    px += chr.advance >> 6;
  }

  mBuffer.loadData(mBuffer.flatten(vertices), mAttributes, mLocations);
  mBuffer.loadIndices(indices);
  mLastText = mText;
}

void Text::loadShader(Str const &vertexPath, Str const &fragmentPath) {
  mShader.compile(Shader::loadShaderSource(vertexPath),
                  Shader::loadShaderSource(fragmentPath));
  mShader.link();
  mShaderLoaded = true;
}

void Text::loadText(WStr const &text) {
  mText = text;
  this->loadText();
}

void Text::render(float const &x, float const &y, float const &windowWidth,
                  float const &windowHeight) {
  if (!mShaderLoaded) {
    throw Exceptions::TextError("Shader not loaded");
  }

  mShader.bind();
  Shader::activateTexture(TextureTargets::TEX_0);
  mShader.setUniform("uTexture", 0);

  mShader.setUniform("uModel", translate(identity<mat4>(), vec3(x, y, 0.0f)));
  mShader.setUniform("uTransform",
                     ortho(0.0f, windowWidth, 0.0f, windowHeight));
  mShader.setUniform("uColor", mColor);

  mFont->bind();
  mBuffer.draw(DrawMode::TRIANGLES);
  mFont->unbind();

  mShader.unbind();
}

Text &Text::operator=(Str const &text) {
  this->loadText(WStr(text.begin(), text.end()));
  return *this;
}

Text &Text::operator=(WStr const &text) {
  this->loadText(text);
  return *this;
}
} // namespace Terreate::Graphic
