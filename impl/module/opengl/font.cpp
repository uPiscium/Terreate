#include "../../../include/module/opengl/exception.hpp"
#include "../../../include/module/opengl/font.hpp"

namespace Terreate::OpenGL {
void Font::initializeTexture() {
  u32 size = Texture::getMaxTextureSize() / 2;
  Texture *texture = new Texture(size / 4, size / 4, 16);
  mTexture = unique<Texture>(texture);
}

void Font::loadDummyCharacter() {
  CharacterData chr = CharacterData();
  chr.codepoint = 0;
  chr.size = {0, 0};
  chr.bearing = {0, 0};
  chr.advance = 0;
  chr.uv = {0, 0, 0, 0, 0};

  mCharacters.insert({0, chr});
}

Font::Font() {
  mLibrary = FT_Library();
  if (FT_Init_FreeType(&mLibrary)) {
    throw Exception::FontError("Failed to initialize FreeType.");
    return;
  }
}

Font::Font(str const &path, u32 const &size) : mSize(size) {
  mLibrary = FT_Library();
  if (FT_Init_FreeType(&mLibrary)) {
    throw Exception::FontError("Failed to initialize FreeType.");
    return;
  }
  this->loadFont(path, size);
}

Font::~Font() {
  if (mFace != nullptr) {
    FT_Done_Face(mFace);
  }
  if (mLibrary != nullptr) {
    FT_Done_FreeType(mLibrary);
  }
  mCharacters.clear();
}

CharacterData const &Font::getCharacter(wchar_t const &character) {
  auto it = mCharacters.find(character);
  if (it == mCharacters.end()) {
    this->loadCharacter(character);
    it = mCharacters.find(character);
  }
  return it->second;
}

CharacterData const &Font::getCharacter(wchar_t const &character) const {
  auto it = mCharacters.find(character);
  if (it == mCharacters.end()) {
    return mCharacters.at(0);
  }
  return it->second;
}

pair<u32> Font::getTextSize(wstr const &text) const {
  u32 width = 0;
  u32 height = 0;
  for (wchar_t const &character : text) {
    CharacterData const &c = this->getCharacter(character);
    width += c.advance >> 6;
    if (c.size.second > height) {
      height = c.size.second;
    }
  }
  return {width, height};
}

vec<CharacterData> Font::getCharacters(wstr const &text) const {
  vec<CharacterData> characters;
  for (wchar_t const &character : text) {
    CharacterData const &c = this->getCharacter(character);
    characters.push_back(c);
  }
  return characters;
}

void Font::loadFont(str const &path, u32 const &size) {
  mFace = FT_Face();
  if (FT_New_Face(mLibrary, path.c_str(), 0, &mFace)) {
    throw Exception::FontError("Failed to load font.");
    return;
  }

  mSize = size;
  FT_Set_Pixel_Sizes(mFace, 0, size);
  this->initializeTexture();
  this->loadDummyCharacter();
}

void Font::loadCharacter(wchar_t const &character) {
  if (mCharacters.find(character) != mCharacters.end()) {
    return;
  }

  if ((u32)character == UNICODE_HALF_SPACE ||
      (u32)character == UNICODE_FULL_SPACE) {
    CharacterData c = CharacterData();
    u32 width = ((u32)character == UNICODE_HALF_SPACE) ? mSize / 2 : mSize;
    c.codepoint = (u32)character;
    c.size = {width, mSize};
    c.bearing = {0, 0};
    c.advance = width << 6;
    c.uv = {0, 0, 0, 0, 0};

    mCharacters.insert({character, c});
    return;
  }

  if (FT_Load_Char(mFace, character, FT_LOAD_RENDER)) {
    throw Exception::FontError("Failed to load character.");
    return;
  }

  u32 width = mFace->glyph->bitmap.width;
  u32 height = mFace->glyph->bitmap.rows;
  unsigned char *buffer = mFace->glyph->bitmap.buffer;

  u32 tw = mTexture->getWidth();
  u32 th = mTexture->getHeight();
  if (mXOffset + width >= tw) {
    mXOffset = 0;
    mYOffset += mSize;
  }
  if (mYOffset >= th) {
    mYOffset = 0;
    ++mZOffset;
  }

  mTexture->loadDataAt(std::to_string((u32)character), mXOffset, mYOffset,
                       mZOffset, width, height, 1, buffer);
  vec<float> uv = {(float)mXOffset / tw, (float)mYOffset / th,
                   (float)(mXOffset + width) / tw,
                   (float)(mYOffset + height) / th, (float)mZOffset};
  mXOffset += width;

  CharacterData c = CharacterData();
  c.codepoint = (u32)character;
  c.size = {width, height};
  c.bearing = {mFace->glyph->bitmap_left, mFace->glyph->bitmap_top};
  c.advance = mFace->glyph->advance.x;
  c.uv = uv;

  mCharacters.insert({character, c});
}

void Font::loadText(wstr const &text) {
  for (wchar_t const &character : text) {
    this->loadCharacter(character);
  }
}
} // namespace Terreate::OpenGL
