#ifndef __TERREATE_GRAPHICS_FONT_HPP__
#define __TERREATE_GRAPHICS_FONT_HPP__

#include <ft2build.h>
#include FT_FREETYPE_H

#include <bindable.hpp>
#include <graphic/texture.hpp>
#include <types.hpp>

namespace Terreate::Graphic {
using namespace Terreate::Types;

struct CharacterData {
  Uint codepoint;
  Pair<Uint> size;
  Pair<Uint> bearing;
  Long advance;
  Vec<Float> uv; // {x0, y0, x1, y1, z}
};

class Font : public Interface::IBindable {
private:
  Shared<FT_Library> mLibrary = nullptr;
  Shared<FT_Face> mFace = nullptr;
  Uint mSize;
  Texture mTexture;
  Uint mXOffset = 0u;
  Uint mYOffset = 0u;
  Uint mZOffset = 0u;
  Map<wchar_t, CharacterData> mCharacters;

private:
  void InitializeTexture();
  void LoadDummyCharacter();

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
  Font(Str const &path, Uint const &size);
  ~Font();

  /*
   * @brief: Getter for font size.
   * @return: font size
   */
  Uint GetFontSize() const { return mSize; }
  /*
   * @brief: Getter for character.
   * @param: character: character to get
   * @return: character
   */
  CharacterData const &GetCharacter(wchar_t const &character);

  /*
   * @brief: Getter for character.
   * @param: character: character to get
   * @return: character
   */
  CharacterData const &GetCharacter(wchar_t const &character) const;
  /*
   * @brief: Getter for text size.
   * @param: text: text to get size of
   * @return: text size in pixels {width, height}
   */
  Pair<Uint> GetTextSize(WStr const &text) const;
  /*
   * @brief: Getter for characters of text.
   * @param: text: text to get characters of
   * @return: characters of text
   */
  Vec<CharacterData> GetCharacters(WStr const &text) const;

  /*
   * @brief: Loads font data.
   * @param: path: path to font file
   * @param: size: size of font
   */
  void LoadFont(Str const &path, Uint const &size);
  /*
   * @brief: Loads character data.
   * @param: character: character to load
   */
  void LoadCharacter(wchar_t const &character);
  /*
   * @brief: Loads text into OpenGL texture.
   * @param: text: text to load
   */
  void LoadText(WStr const &text);

  /*
   * @brief: Uses font texture.
   */
  void Bind() const override { mTexture.Bind(); }
  /*
   * @brief: Unuses font texture.
   */
  void Unbind() const override { mTexture.Unbind(); }

  operator Bool() const { return mFace != nullptr; }
};
} // namespace Terreate::Graphic

#endif // __TERREATE_GRAPHICS_FONT_HPP__
