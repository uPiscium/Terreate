#ifndef __TERREATE_GRAPHICS_FONT_HPP__
#define __TERREATE_GRAPHICS_FONT_HPP__

#include <ft2build.h>
#include FT_FREETYPE_H

#include <core/type.hpp>
#include <graphic/texture.hpp>

namespace Terreate::Graphic {
using namespace Terreate::Types;

struct CharacterData {
  u32 codepoint;
  Pair<u32> size;
  Pair<u32> bearing;
  i64 advance;
  Vec<float> uv; // {x0, y0, x1, y1, z}
};

class Font {
private:
  Shared<FT_Library> mLibrary = nullptr;
  Shared<FT_Face> mFace = nullptr;
  u32 mSize;
  Texture mTexture;
  u32 mXOffset = 0u;
  u32 mYOffset = 0u;
  u32 mZOffset = 0u;
  Map<wchar_t, CharacterData> mCharacters;

private:
  void initializeTexture();
  void loadDummyCharacter();

public:
  /*
   * @brief: Default constructor for RawFont.
   */
  Font();
  /*
   * @brief: Constructor for RawFont.
   * @param: path: path to font file
   * @param: size: size of font
   */
  Font(Str const &path, u32 const &size);
  ~Font();

  /*
   * @brief: Getter for font size.
   * @return: font size
   */
  u32 getFontSize() const { return mSize; }
  /*
   * @brief: Getter for character.
   * @param: character: character to get
   * @return: character
   */
  CharacterData const &getCharacter(wchar_t const &character);

  /*
   * @brief: Getter for character.
   * @param: character: character to get
   * @return: character
   */
  CharacterData const &getCharacter(wchar_t const &character) const;
  /*
   * @brief: Getter for text size.
   * @param: text: text to get size of
   * @return: text size in pixels {width, height}
   */
  Pair<u32> getTextSize(WStr const &text) const;
  /*
   * @brief: Getter for characters of text.
   * @param: text: text to get characters of
   * @return: characters of text
   */
  Vec<CharacterData> getCharacters(WStr const &text) const;

  /*
   * @brief: Loads font data.
   * @param: path: path to font file
   * @param: size: size of font
   */
  void loadFont(Str const &path, u32 const &size);
  /*
   * @brief: Loads character data.
   * @param: character: character to load
   */
  void loadCharacter(wchar_t const &character);
  /*
   * @brief: Loads text into OpenGL texture.
   * @param: text: text to load
   */
  void loadText(WStr const &text);

  /*
   * @brief: Uses font texture.
   */
  void bind() const { mTexture.bind(); }
  /*
   * @brief: Unuses font texture.
   */
  void unbind() const { mTexture.unbind(); }

  operator bool() const { return mFace != nullptr; }
};
} // namespace Terreate::Graphic

#endif // __TERREATE_GRAPHICS_FONT_HPP__
