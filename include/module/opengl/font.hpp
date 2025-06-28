#pragma once

#include <ft2build.h>
#include FT_FREETYPE_H

#include "../common/type.hpp"

#include "texture.hpp"

namespace Terreate::OpenGL {
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
