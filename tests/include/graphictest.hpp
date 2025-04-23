// #ifndef __TERREATE_TEST_GRAPHICTEST_HPP__
// #define __TERREATE_TEST_GRAPHICTEST_HPP__

// #include <graphic.hpp>

// #ifndef STB_IMAGE_IMPLEMENTATION
// #define STB_IMAGE_IMPLEMENTATION
// #else
// #undef STB_IMAGE_IMPLEMENTATION
// #endif
// #define STB_IMAGE_STATIC
// #include <stb/stb_image.h>

// namespace Terreate::Test::Graphic {
// using namespace Terreate::Types;
// using namespace Terreate::Graphic;

// struct Uniform {
//   mat4 transform;
//   mat4 model;
//   mat4 view;
//   mat4 proj;
// };

// struct Settings {
//   vec4 setting;
// };

// void OutputJoystickData(Joystick const &joystick, Text &text, Uint const
// &width,
//                         Uint const &height);
// void TestCompute();

// class TestApp {
// private:
//   Clock mClock = Clock(120, 100);
//   Shader mShader;
//   Shader mScreenShader;
//   mat4 mTransform;
//   Float mWidth = 1500.0f;
//   Float mHeight = 750.0f;
//   Float mNear = 0.01f;
//   Float mFar = 10000.0f;
//   Float mDepth = mFar - mNear;

//   Font mFont;
//   Text mText;

//   Font mInfoFont;
//   Text mInfoText;

//   Texture mTexture;
//   Texture mTexture2;

//   /* WStr mTextString = WStr(L"日本語テスト"); */
//   WStr mTextString = L"ABC";
//   Uint mDelflag = 0u;
//   Uint mPressingFlag = 0u;
//   Uint mDelInterval = 10u;

//   Buffer mBuffer;
//   Buffer mScreenBuffer;
//   BufferDataConstructor mColorDataConstructor;

//   Screen mScreen;
//   Uniform mUniform;
//   Uniform mScreenUniform;
//   UniformBuffer mUBO;
//   UniformBuffer mScreenUBO;

//   Settings mSettings;
//   ShaderStorageBuffer mSSBO;

// public:
//   void SizeCallback(Window *window, int const &width, int const &height);
//   void KeyCallback(Window *window, Key const &key);
//   void CharCallback(Window *window, Uint const &codepoint);

// public:
//   TestApp();

//   void Frame(Window *window);
// };

// void Run();

// } // namespace Terreate::Test::Graphic

// #endif // __TERREATE_TEST_GRAPHICTEST_HPP__
