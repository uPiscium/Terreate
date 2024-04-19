#include "../includes/font.hpp"

namespace TerreateCore {
namespace Core {
using namespace TerreateCore::Defines;

Font::Font(Str const &path, Uint const &size) : mSize(size) {
  if (FT_Init_FreeType(&mLibrary)) {
    TC_THROW("Failed to initialize FreeType library.");
  }

  if (FT_New_Face(mLibrary, path.c_str(), 0, &mFace)) {
    TC_THROW("Failed to load font.");
  }

  FT_Set_Pixel_Sizes(mFace, 0, size);
}

Font::~Font() {
  FT_Done_Face(mFace);
  FT_Done_FreeType(mLibrary);
  mCharacters.clear();
}

Shared<Character> const &
Font::AcquireCharacter(wchar_t const &character) const {
  auto it = mCharacters.find(character);
  if (it == mCharacters.end()) {
    TC_THROW("Character not found.");
  }
  return it->second;
}

Pair<Uint> Font::AcquireTextSize(WStr const &text) const {
  Uint width = 0;
  Uint height = 0;
  for (wchar_t const &character : text) {
    Shared<Character> const &c = AcquireCharacter(character);
    width += c->advance >> 6;
    if (c->size.second > height) {
      height = c->size.second;
    }
  }
  return {width, height};
}

Vec<Shared<Character>> Font::AcquireCharacters(WStr const &text) const {
  Vec<Shared<Character>> characters;
  for (wchar_t const &character : text) {
    Shared<Character> const &c = AcquireCharacter(character);
    characters.push_back(c);
  }
  return characters;
}

void Font::LoadCharacter(wchar_t const &character) {
  if (mCharacters.find(character) != mCharacters.end()) {
    return;
  }

  if ((Uint)character == TC_UNICODE_HALF_SPACE ||
      (Uint)character == TC_UNICODE_FULL_SPACE) {
    Shared<Character> c = std::make_shared<Character>();
    Uint width = ((Uint)character == TC_UNICODE_HALF_SPACE) ? mSize / 2 : mSize;
    c->codepoint = (Uint)character;
    c->texture = nullptr;
    c->size = {width, mSize};
    c->bearing = {0, 0};
    c->advance = width;

    mCharacters.insert({character, c});
    return;
  }

  if (FT_Load_Char(mFace, character, FT_LOAD_RENDER)) {
    TC_THROW("Failed to load glyph.");
    return;
  }

  Shared<Character> c = std::make_shared<Character>();
  c->codepoint = (Uint)character;
  c->texture = std::make_shared<Texture>();
  c->texture->LoadData(mFace->glyph->bitmap.width, mFace->glyph->bitmap.rows, 1,
                       mFace->glyph->bitmap.buffer);

  c->size = {mFace->glyph->bitmap.width, mFace->glyph->bitmap.rows};
  c->bearing = {mFace->glyph->bitmap_left, mFace->glyph->bitmap_top};
  c->advance = mFace->glyph->advance.x;

  mCharacters.insert({character, c});
}
} // namespace Core
} // namespace TerreateCore
