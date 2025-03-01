#include <exceptions.hpp>
#include <graphic/font.hpp>

namespace Terreate::Graphic {
void Font::InitializeTexture() {
  Uint size = Texture::GetMaxTextureSize() / 2;
  mTexture = Texture(size / 4, size / 4, 16);
}

void Font::LoadDummyCharacter() {
  CharacterData chr = CharacterData();
  chr.codepoint = 0;
  chr.size = {0, 0};
  chr.bearing = {0, 0};
  chr.advance = 0;
  chr.uv = {0, 0, 0, 0, 0};

  mCharacters.insert({0, chr});
}

Font::Font() {
  mLibrary = Shared<FT_Library>(new FT_Library());
  if (FT_Init_FreeType(mLibrary.get())) {
    throw Exceptions::FontError("Failed to initialize FreeType.");
    return;
  }
}

Font::Font(Str const &path, Uint const &size) : mSize(size) {
  mLibrary = Shared<FT_Library>(new FT_Library());
  if (FT_Init_FreeType(mLibrary.get())) {
    throw Exceptions::FontError("Failed to initialize FreeType.");
    return;
  }
  this->LoadFont(path, size);
}

Font::~Font() {
  if (mFace.use_count() <= 1 && mLibrary.use_count() <= 1) {
    if (mFace != nullptr) {
      FT_Done_Face(*mFace);
    }
    if (mLibrary != nullptr) {
      FT_Done_FreeType(*mLibrary);
    }
    mCharacters.clear();
  }
}

CharacterData const &Font::GetCharacter(wchar_t const &character) {
  auto it = mCharacters.find(character);
  if (it == mCharacters.end()) {
    this->LoadCharacter(character);
    it = mCharacters.find(character);
  }
  return it->second;
}

CharacterData const &Font::GetCharacter(wchar_t const &character) const {
  auto it = mCharacters.find(character);
  if (it == mCharacters.end()) {
    return mCharacters.at(0);
  }
  return it->second;
}

Pair<Uint> Font::GetTextSize(WStr const &text) const {
  Uint width = 0;
  Uint height = 0;
  for (wchar_t const &character : text) {
    CharacterData const &c = GetCharacter(character);
    width += c.advance >> 6;
    if (c.size.second > height) {
      height = c.size.second;
    }
  }
  return {width, height};
}

Vec<CharacterData> Font::GetCharacters(WStr const &text) const {
  Vec<CharacterData> characters;
  for (wchar_t const &character : text) {
    CharacterData const &c = GetCharacter(character);
    characters.push_back(c);
  }
  return characters;
}

void Font::LoadFont(Str const &path, Uint const &size) {
  mFace = Shared<FT_Face>(new FT_Face());
  if (FT_New_Face(*mLibrary, path.c_str(), 0, mFace.get())) {
    throw Exceptions::FontError("Failed to load font.");
    return;
  }

  mSize = size;
  FT_Set_Pixel_Sizes(*mFace, 0, size);
  this->InitializeTexture();
  this->LoadDummyCharacter();
}

void Font::LoadCharacter(wchar_t const &character) {
  if (mCharacters.find(character) != mCharacters.end()) {
    return;
  }

  if ((Uint)character == TC_UNICODE_HALF_SPACE ||
      (Uint)character == TC_UNICODE_FULL_SPACE) {
    CharacterData c = CharacterData();
    Uint width = ((Uint)character == TC_UNICODE_HALF_SPACE) ? mSize / 2 : mSize;
    c.codepoint = (Uint)character;
    c.size = {width, mSize};
    c.bearing = {0, 0};
    c.advance = width << 6;
    c.uv = {0, 0, 0, 0, 0};

    mCharacters.insert({character, c});
    return;
  }

  if (FT_Load_Char(*mFace, character, FT_LOAD_RENDER)) {
    throw Exceptions::FontError("Failed to load character.");
    return;
  }

  Uint width = (*mFace)->glyph->bitmap.width;
  Uint height = (*mFace)->glyph->bitmap.rows;
  unsigned char *buffer = (*mFace)->glyph->bitmap.buffer;

  Uint tw = mTexture.GetWidth();
  Uint th = mTexture.GetHeight();
  if (mXOffset + width >= tw) {
    mXOffset = 0;
    mYOffset += mSize;
  }
  if (mYOffset >= th) {
    mYOffset = 0;
    ++mZOffset;
  }

  mTexture.LoadDataAt(std::to_string((Uint)character), mXOffset, mYOffset,
                      mZOffset, width, height, 1, buffer);
  Vec<Float> uv = {(Float)mXOffset / tw, (Float)mYOffset / th,
                   (Float)(mXOffset + width) / tw,
                   (Float)(mYOffset + height) / th, (Float)mZOffset};
  mXOffset += width;

  CharacterData c = CharacterData();
  c.codepoint = (Uint)character;
  c.size = {width, height};
  c.bearing = {(*mFace)->glyph->bitmap_left, (*mFace)->glyph->bitmap_top};
  c.advance = (*mFace)->glyph->advance.x;
  c.uv = uv;

  mCharacters.insert({character, c});
}

void Font::LoadText(WStr const &text) {
  for (wchar_t const &character : text) {
    this->LoadCharacter(character);
  }
}
} // namespace Terreate::Graphic
