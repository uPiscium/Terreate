#ifndef __TERREATE_FONT_HPP__
#define __TERREATE_FONT_HPP__

#include <bindable.hpp>
#include <graphic.hpp>
#include <mutex>
#include <types.hpp>

namespace Terreate {
using namespace Terreate::Types;

typedef Graphic::CharacterData Char;

class Font : public Interface::IBindable {
private:
  typedef std::recursive_mutex Mutex;

private:
  Mutex mCharMutex;
  Graphic::Font mFont;

public:
  /*
   * @brief: Graphic font wrapper class.
   * @param: path: path to font file
   * @param: size: size of font
   */
  Font(Str const &path, Uint const &size) : mFont(path, size) {}
  ~Font() {}

  /*
   * @brief: Getter for font size.
   * @return: font size
   */
  Uint GetSize() const { return mFont.GetFontSize(); }
  /*
   * @brief: Getter for character.
   * @param: cp: codepoint of character
   * @return: character
   */
  Char const &GetChar(Uint const &cp) const { return mFont.GetCharacter(cp); }
  /*
   * @brief: Getter for character.
   * @param: chr: character
   * @return: character
   */
  Char const &GetChar(wchar_t const &chr) const {
    return mFont.GetCharacter(chr);
  }
  /*
   * @brief: Getter for character.
   * @param: text: text
   * @return: characters
   */
  Vec<Char> GetText(WStr const &text) const {
    return mFont.GetCharacters(text);
  }
  /*
   * @brief: Getter for text size.
   * @param: text: text
   * @return: text size
   */
  Pair<Uint> GetTextSize(WStr const &text) const {
    return mFont.GetTextSize(text);
  }
  /*
   * @brief: Loads font data.
   * @param: path: path to font file
   * @param: size: size of font
   */
  void LoadFont(Str const &path, Uint const &size);
  /*
   * @brief: Loads character data.
   * @param: cp: codepoint of character
   */
  void LoadChar(Uint const &cp);
  /*
   * @brief: Loads character data.
   * @param: chr: character
   */
  void LoadChar(wchar_t const &chr);
  /*
   * @brief: Loads characters data of text.
   * @param: text: text
   */
  void LoadText(WStr const &text);
  /*
   * @brief: Bind font.
   */
  void Bind() const override { mFont.Bind(); }
  /*
   * @brief: Unbind font.
   */
  void Unbind() const override { mFont.Unbind(); }
};
} // namespace Terreate

#endif // __TERREATE_FONT_HPP__
