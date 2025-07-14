#pragma once

#include <freetype/freetype.h>
#include <freetype/ftoutln.h>
#include <ft2build.h>

#include "../common/math.hpp"
#include "../common/type.hpp"

#include "texture.hpp"

namespace Terreate::OpenGL {
struct CurveSegment {
  vec2 p0;
  vec2 p1;
  vec2 p2;
  vec2 p3;
  u32 type; // 0: line, 1: quadratic, 2: cubic
};

struct Outline {
  FT_Vector current;
  vec<CurveSegment> *segments;
};

namespace Decomposer {
int moveTo(FT_Vector const *to, void *outline);
int lineTo(FT_Vector const *to, void *outline);
int quadraticTo(FT_Vector const *control, FT_Vector const *to, void *outline);
int cubicTo(FT_Vector const *control1, FT_Vector const *control2,
            FT_Vector const *to, void *outline);
} // namespace Decomposer

struct CharacterData {
  u32 codepoint;
  pair<u32> size;
  pair<u32> bearing;
  i64 advance;
  vec<float> uv; // {x0, y0, x1, y1, z}
};

class Font {
private:
  PROHIBIT_COPY_AND_ASSIGN(Font);

private:
  static constexpr u32 UNICODE_HALF_SPACE = 0x0020;
  static constexpr u32 UNICODE_FULL_SPACE = 0x7e32;

private:
  FT_Library mLibrary = nullptr;
  FT_Face mFace = nullptr;
  u32 mSize;
  unique<Texture> mTexture;
  u32 mXOffset = 0u;
  u32 mYOffset = 0u;
  u32 mZOffset = 0u;
  umap<wchar_t, CharacterData> mCharacters;

private:
  void initializeTexture();
  void loadDummyCharacter();
  vec<CurveSegment> decomposeOutline(wchar_t const &chr) const;

public:
  Font();
  Font(str const &path, u32 const &size);
  ~Font();

  u32 getFontSize() const { return mSize; }
  CharacterData const &getCharacter(wchar_t const &character);
  CharacterData const &getCharacter(wchar_t const &character) const;
  pair<u32> getTextSize(wstr const &text) const;
  vec<CharacterData> getCharacters(wstr const &text) const;

  void loadFont(str const &path, u32 const &size);
  void loadCharacter(wchar_t const &character);
  void loadText(wstr const &text);

  void bind() const { mTexture->bind(); }
  void unbind() const { mTexture->unbind(); }

  operator bool() const { return mFace != nullptr; }
};
} // namespace Terreate::OpenGL
