#pragma once
#include <module/common.hpp>
#include <module/core.hpp>
#include <module/opengl.hpp>
#include <module/sdl.hpp>

#ifndef STB_IMAGE_IMPLEMENTATION
#define STB_IMAGE_IMPLEMENTATION
#else
#undef STB_IMAGE_IMPLEMENTATION
#endif
#define STB_IMAGE_STATIC
#include <stb/stb_image.h>

namespace Terreate::Test {
using namespace Terreate::OpenGL;
using namespace Terreate::SDL;

struct Uniform {
  mat4 transform;
  mat4 model;
  mat4 view;
  mat4 proj;
};

struct Settings {
  vec4 setting;
};

// void outputJoystickData(Joystick const &joystick, Text &text, u32 const
// &width,
//                         u32 const &height);
void testCompute();

class TestApp {
private:
  Clock mClock = Clock(100);
  Shader mShader;
  Shader mScreenShader;
  mat4 mTransform;
  float mWidth = 1500.0f;
  float mHeight = 750.0f;
  float mNear = 0.01f;
  float mFar = 10000.0f;
  float mDepth = mFar - mNear;

  unique<Font> mFont;
  Text mText;

  unique<Font> mInfoFont;
  Text mInfoText;

  unique<Texture> mTexture;
  unique<Texture> mTexture2;

  wstr mTextString = L"日本語テスト";
  // wstr mTextString = L"ABC";
  u32 mDelflag = 0u;
  u32 mPressingFlag = 0u;
  u32 mDelInterval = 10u;

  Buffer mBuffer;
  Buffer mScreenBuffer;
  BufferDataConstructor mColorDataConstructor;

  Screen mScreen;
  Uniform mUniform;
  Uniform mScreenUniform;
  UniformBuffer mUBO;
  UniformBuffer mScreenUBO;

  Settings mSettings;
  ShaderStorageBuffer mSSBO;

public:
  // void sizeCallback(Window *window, int const &width, int const &height);
  // void keyCallback(Window *window, Key const &key);
  // void charCallback(Window *window, u32 const &codepoint);

public:
  TestApp();

  void frame(Window *window);
};

} // namespace Terreate::Test

int main();
