#include <core/math.hpp>
#include <graphic/text.hpp>

namespace Terreate::Graphic {
using namespace Terreate::Math;

void Text::LoadText() {
  if (mText == mLastText) {
    return;
  }

  mFont->LoadText(mText);
  auto chars = mFont->GetCharacters(mText);

  Vec<Vec<Float>> vertices;
  Vec<Uint> indices;
  Float px = 0.0f;
  Uint count = 0;
  for (auto &chr : chars) {
    if (chr.codepoint == 0) { // Skip dummy characters
      px += chr.advance >> 6;
      continue;
    }

    Uint c = chr.codepoint;
    if (c == TC_UNICODE_HALF_SPACE ||
        c == TC_UNICODE_FULL_SPACE) { // Skip spaces
      px += chr.advance >> 6;
      continue;
    }

    Float w = chr.size.first;
    Float h = chr.size.second;
    Float x = px + chr.bearing.first;
    Float y = chr.bearing.second - h;
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

  mBuffer.LoadData(mBuffer.Flatten(vertices), mAttributes, mLocations);
  mBuffer.LoadIndices(indices);
  mLastText = mText;
}

void Text::LoadShader(Str const &vertexPath, Str const &fragmentPath) {
  mShader.AddVertexShaderSource(Shader::LoadShaderSource(vertexPath));
  mShader.AddFragmentShaderSource(Shader::LoadShaderSource(fragmentPath));
  mShader.Compile();
  mShader.Link();
  mShaderLoaded = true;
}

void Text::LoadText(WStr const &text) {
  mText = text;
  this->LoadText();
}

void Text::Render(Float const &x, Float const &y, Float const &windowWidth,
                  Float const &windowHeight) {
  if (!mShaderLoaded) {
    throw Exceptions::TextError("Shader not loaded");
  }

  mShader.Use();
  Shader::ActivateTexture(TextureTargets::TEX_0);
  mShader.SetInt("uTexture", 0);

  mShader.SetMat4("uModel", translate(identity<mat4>(), vec3(x, y, 0.0f)));
  mShader.SetMat4("uTransform", ortho(0.0f, windowWidth, 0.0f, windowHeight));
  mShader.SetVec3("uColor", mColor);

  mFont->Use();
  mBuffer.Draw(DrawMode::TRIANGLES);
  mFont->Unuse();

  mShader.Unuse();
}

Text &Text::operator=(Str const &text) {
  this->LoadText(WStr(text.begin(), text.end()));
  return *this;
}

Text &Text::operator=(WStr const &text) {
  this->LoadText(text);
  return *this;
}
} // namespace Terreate::Graphic
