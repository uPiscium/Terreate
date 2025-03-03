#include <graphictest.hpp>
#include <iostream>

namespace Terreate::Test::Graphic {

void OutputJoystickData(Joystick const &joystick, Text &text, Uint const &width,
                        Uint const &height) {
  JoystickAxisState axisState = joystick.GetAxisState();
  JoystickButtonState buttonState = joystick.GetButtonState();
  JoystickHatState hatState = joystick.GetHatState();

  Stream ss;
  if (joystick.IsGamepad()) {
    ss << "Gamepad: " << (Uint)joystick.GetJoystickId();
  } else {
    ss << "Joystick: " << (Uint)joystick.GetJoystickId();
  }

  if (!joystick.IsConnected()) {
    ss << " Disconnected";
    text.LoadText(ss.str());
    text.Render(0, 1500, width, height);
    return;
  } else {
    ss << " Connected";
    text.LoadText(ss.str());
    text.Render(0, 1500, width, height);
  }

  ss.str("");
  ss << "Left Stick: (" << std::fixed << std::setprecision(3)
     << axisState.leftStick[0] << ", " << axisState.leftStick[1] << ")";
  ss << " / Right Stick: (" << std::fixed << std::setprecision(3)
     << axisState.rightStick[0] << ", " << axisState.rightStick[1] << ")";
  ss << std::fixed << std::setprecision(3)
     << " / Left Trigger: " << axisState.leftTrigger
     << " / Right Trigger: " << axisState.rightTrigger;
  text.LoadText(ss.str());
  text.Render(0, 1450, width, height);

  ss.str("");
  ss << "A: " << buttonState.a << " / B: " << buttonState.b
     << " / X: " << buttonState.x << " / Y: " << buttonState.y;
  ss << " / Cross: " << buttonState.cross << " / Circle: " << buttonState.circle
     << " / Square: " << buttonState.square
     << " / Triangle: " << buttonState.triangle;
  text.LoadText(ss.str());
  text.Render(0, 1400, width, height);

  ss.str("");
  ss << "Left Bumper: " << buttonState.leftBumper
     << " / Right Bumper: " << buttonState.rightBumper
     << " / Back: " << buttonState.back << " / Start: " << buttonState.start
     << " / Guide: " << buttonState.guide
     << " / Left Thumb: " << buttonState.leftThumb
     << " / Right Thumb: " << buttonState.rightThumb;
  text.LoadText(ss.str());
  text.Render(0, 1350, width, height);

  ss.str("");
  ss << "D-Pad: Up: " << buttonState.dpadUp
     << " / Right: " << buttonState.dpadRight
     << " / Down: " << buttonState.dpadDown
     << " / Left: " << buttonState.dpadLeft;
  text.LoadText(ss.str());
  text.Render(0, 1300, width, height);

  ss.str("");
  ss << "Hat: Up: " << hatState.up << " / Right: " << hatState.right
     << " / Down: " << hatState.down << " / Left: " << hatState.left;
  text.LoadText(ss.str());
  text.Render(0, 1250, width, height);
}

void TestCompute() {
  Vec<Float> inputData = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

  ShaderStorageBuffer input, input2, output, output2;
  input.LoadData(inputData);
  output.Allocate(input.GetSize());
  input2.LoadData(inputData);
  output2.Allocate(input2.GetSize());
  std::cout << "input size: " << input.GetSize() << std::endl;
  std::cout << "output size: " << output.GetSize() << std::endl;

  Compute::ComputeKernel kernel;
  kernel.AddKernelSource(
      Shader::LoadShaderSource("tests/resources/shaders/compute.glsl"));
  kernel.Compile();
  kernel.Link();
  std::cout << "kernel compiled" << std::endl;

  input.Bind(kernel, "InputBuffer");
  output.Bind(kernel, "OutputBuffer");
  std::cout << "buffers bound" << std::endl;

  kernel.SetFloat("scaleFactor", 2.0f);
  kernel.Dispatch(10, 1, 1);
  std::cout << "dispatching kernel" << std::endl;

  Compute::ComputeKernel kernel2;
  kernel2.AddKernelSource(
      Shader::LoadShaderSource("tests/resources/shaders/compute.glsl"));
  kernel2.Compile();
  kernel2.Link();
  std::cout << "kernel2 compiled" << std::endl;

  input2.Bind(kernel2, "InputBuffer");
  output2.Bind(kernel2, "OutputBuffer");
  std::cout << "buffers bound" << std::endl;

  kernel2.SetFloat("scaleFactor", 3.0f);
  kernel2.Dispatch(10, 1, 1);
  std::cout << "dispatching kernel2" << std::endl;

  Vec<float> outputData, outputData2;
  output.GetData(outputData);
  output2.GetData(outputData2);
  for (int i = 0; i < inputData.size(); ++i) {
    std::cout << "output[" << i << "] = " << outputData[i] << std::endl;
  }
  for (int i = 0; i < inputData.size(); ++i) {
    std::cout << "output2[" << i << "] = " << outputData2[i] << std::endl;
  }
}

void TestApp::SizeCallback(Window *window, int const &width,
                           int const &height) {
  glViewport(0, 0, width, height);
  mWidth = (Float)width;
  mHeight = (Float)height;
  mShader.Use();
  mUniform.transform = scale(
      identity<mat4>(), vec3(1.0f / mWidth, 1.0f / mHeight, 1.0f / mDepth));
  mUBO.ReloadData(mUniform);
}

void TestApp::KeyCallback(Window *window, Key const &key) {
  if (key.key == Keyboard::K_ESCAPE) {
    window->Close();
  }

  if (key.key == Keyboard::K_BACKSPACE && key.pressed) {
    if (!mTextString.empty() &&
        (mDelflag > mDelInterval || mPressingFlag > 1)) {
      mDelflag = 0u;
      ++mPressingFlag;
      mTextString.pop_back();
      mText.LoadText(mTextString);
    }
  } else {
    mPressingFlag = 0u;
  }
}

void TestApp::CharCallback(Window *window, Uint const &codepoint) {
  mTextString.push_back(codepoint);
  mText.LoadText(mTextString);
}

TestApp::TestApp() : mScreen(1000, 1000, 4) {
  mFont = Font("tests/resources/AsebiMin-Light.otf", 128);
  mInfoFont = Font("tests/resources/AsebiMin-Light.otf", 32);

  mText.LoadFont(&mFont);
  mText.LoadShader("tests/resources/shaders/textVert.glsl",
                   "tests/resources/shaders/textFrag.glsl");

  mInfoText.LoadFont(&mInfoFont);
  mInfoText.LoadShader("tests/resources/shaders/textVert.glsl",
                       "tests/resources/shaders/textFrag.glsl");

  mShader.AddVertexShaderSource(
      Shader::LoadShaderSource("tests/resources/shaders/mainVert.glsl"));
  mShader.AddFragmentShaderSource(
      Shader::LoadShaderSource("tests/resources/shaders/mainFrag.glsl"));
  mShader.Compile();
  mShader.Link();

  mScreenShader.AddVertexShaderSource(
      Shader::LoadShaderSource("tests/resources/shaders/screenVert.glsl"));
  mScreenShader.AddFragmentShaderSource(
      Shader::LoadShaderSource("tests/resources/shaders/screenFrag.glsl"));
  mScreenShader.Compile();
  mScreenShader.Link();

  Map<Str, Uint> attrs = {{"iPosition", 0}, {"iUV", 1}, {"iColor", 2}};
  BufferDataConstructor bdc;

  bdc.AddVertexComponent("iPosition", {{-600.0f, -600.0f, 600.0f},
                                       {600.0f, -600.0f, 600.0f},
                                       {600.0f, -600.0f, -600.0f},
                                       {-600.0f, -600.0f, -600.0f},
                                       {-600.0f, 600.0f, 600.0f},
                                       {600.0f, 600.0f, 600.0f},
                                       {600.0f, 600.0f, -600.0f},
                                       {-600.0f, 600.0f, -600.0f}});
  bdc.AddVertexComponent(
      "iUV", {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}});
  bdc.SetVertexIndices({
      {0, 0}, {1, 1}, {5, 2}, {4, 3}, {1, 0}, {2, 1}, {6, 2}, {5, 3},
      {2, 0}, {3, 1}, {7, 2}, {6, 3}, {3, 0}, {0, 1}, {4, 2}, {7, 3},
      {3, 0}, {2, 1}, {1, 2}, {0, 3}, {4, 0}, {5, 1}, {6, 2}, {7, 3},
  });
  bdc.Construct();
  mBuffer.LoadData(bdc, attrs);

  BufferDataConstructor screenBDC;
  screenBDC.AddVertexComponent("iPosition", {{-0.8f, -0.8f, 0.2f},
                                             {0.8f, -0.8f, 0.2f},
                                             {0.8f, 0.8f, 0.2f},
                                             {-0.8f, 0.8f, 0.2f}});
  screenBDC.AddVertexComponent(
      "iUV", {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}});
  screenBDC.SetVertexIndices({{0, 0}, {1, 1}, {2, 2}, {3, 3}});
  screenBDC.Construct();
  mScreenBuffer.LoadData(screenBDC, attrs);

  mColorDataConstructor.AddVertexComponent("iColor", {{1, 0, 0}});
  mColorDataConstructor.SetVertexIndices(
      {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
       {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}});
  mColorDataConstructor.Construct();
  mBuffer.LoadData(mColorDataConstructor, attrs);

  mBuffer.LoadIndices({{0, 1, 2, 2, 3, 0},
                       {4, 5, 6, 6, 7, 4},
                       {8, 9, 10, 10, 11, 8},
                       {12, 13, 14, 14, 15, 12},
                       {16, 17, 18, 18, 19, 16},
                       {20, 21, 22, 22, 23, 20}});
  mScreenBuffer.LoadIndices({0, 1, 2, 2, 3, 0});

  mat4 view = lookAt(vec3(0, 0, 2), vec3(0, 0, 0), vec3(0, 1, 0));
  mat4 proj = perspective(45.0f, 1.0f, mNear, mFar);
  mUniform.view = view;
  mScreenUniform.view = identity<mat4>();
  mUniform.proj = proj;
  mScreenUniform.proj = identity<mat4>();
  mUniform.model = identity<mat4>();
  mScreenUniform.model = identity<mat4>();
  mUBO.LoadData(mUniform);
  mScreenUBO.LoadData(mScreenUniform);
  mUBO.Bind(mShader, "Matrices");
  mScreenUBO.Bind(mScreenShader, "Matrices");

  mSettings = {{1.0f, 1.0f, 1.0f, 0.5f}};
  mSSBO.LoadData(mSettings);
  mSSBO.Bind(mShader, "Settings");

  // Uncomment if you want to break your brain...
  /* mShader.UseDepth(false); */

  mShader.Use();
  mShader.SetInt("uTexture", 0);
  Shader::ActivateTexture(TextureTargets::TEX_0);

  mScreenShader.Use();
  mScreenShader.SetInt("uTexture", 0);
  Shader::ActivateTexture(TextureTargets::TEX_0);

  mTexture2 = Texture(800, 800, 2);
  Compute::ImageConverter converter;
  Int width = 0, height = 0, channels = 0;
  stbi_set_flip_vertically_on_load(true);
  Ubyte *pixels = stbi_load("tests/resources/testImage2.png", &width, &height,
                            &channels, 4);
  converter.Convert("testImage2", 1, width, height, channels, pixels,
                    mTexture2);
  stbi_image_free(pixels);
}

void TestApp::Frame(Window *window) {
  glfwPollEvents();
  window->Fill(0.2, 0.2, 0.2);
  window->Clear();

  /* auto state = */
  /*     Joystick::GetJoystick(JoystickID::JOYSTICK1).GetAxisState(); */
  /* Float angleX = state.leftStick[0]; */
  /* Float angleY = state.leftStick[1]; */
  /* mat4 model = rotate(identity<mat4>(), angleX, vec3(0, 1, 0)); */
  /* model = rotate(model, angleY, vec3(1, 0, 0)); */
  Float angle = mClock.GetCurrentRuntime() * 0.1;
  mat4 model = rotate(identity<mat4>(), angle, vec3(1, 1, 1));
  mUniform.model = model;
  mScreenUniform.model = model;
  mUBO.ReloadData(mUniform);
  mScreenUBO.ReloadData(mScreenUniform);

  Texture const &texture = mScreen.GetTexture();

  mScreen.Fill({0, 0, 0});
  mScreen.Clear();
  mScreen.Bind();
  mScreenShader.Use();
  /* mInfoFont.Use(); */
  mTexture2.Bind();
  mScreenBuffer.Draw(DrawMode::TRIANGLES);
  mTexture2.Unbind();
  /* mInfoFont.Unuse(); */
  mScreenShader.Unuse();
  mText.LoadText(WStr(L"Cube"));
  auto size = mFont.GetTextSize(WStr(L"Cube"));
  mText.Render(500 - size.first / 2.0, 500 - size.second / 2.0,
               mScreen.GetWidth(), mScreen.GetHeight());
  mScreen.Unbind();

  mSettings = {
      {(std::sin(angle) + 1) / 2.0, (std::cos(angle) + 1) / 2.0, 1.0f, 0.5f}};
  mSSBO.ReloadData(mSettings);

  window->Bind();
  mShader.Use();
  texture.Bind();
  mBuffer.Draw(DrawMode::TRIANGLES);
  texture.Unbind();
  mShader.Unuse();

  mText = mTextString;
  mText.Render(0, 0, mWidth, mHeight);

  /* AttributeData color = mBuffer["iColor"]; */
  /* Float r = (state.leftTrigger + 1) / 2; */
  /* Float g = (state.rightTrigger + 1) / 2; */
  /* mColorDataConstructor.ReloadVertexComponent("iColor", {{r, g, 1}}); */
  /* mColorDataConstructor.Construct(); */
  /* mBuffer.ReloadData(color, mColorDataConstructor); */
  mText.SetColor({1, 0, 0});

  mInfoText = L"FPS: " + std::to_wstring(mClock.GetFPS());
  mInfoText.Render(0, 180, mWidth, mHeight);

  Joystick const &joystick = Joystick::GetJoystick(JoystickID::JOYSTICK1);
  OutputJoystickData(joystick, mInfoText, mWidth, mHeight);

  window->Swap();
  ++mDelflag;
  mClock.Tick();
}

void LaunchApp() {
  Window window(2500, 1600, "Test Window", WindowSettings());
  InitializeGLAD();
  glViewport(0, 0, 2500, 1600);
  window.DisableVsync();

  // TestCompute();

  TestApp app;
  window.onFramebufferSizeChange +=
      Subscriber<Window *, Uint const &, Uint const &>(
          [&app](Window *window, Uint const &width, Uint const &height) {
            app.SizeCallback(window, width, height);
          });
  window.onKeyInput += Subscriber<Window *, Key const &>(
      [&app](Window *window, Key const &key) { app.KeyCallback(window, key); });
  window.onCharInput += Subscriber<Window *, Uint const &>(
      [&app](Window *window, Uint const &codepoint) {
        app.CharCallback(window, codepoint);
      });

  glViewport(0, 0, 2500, 1600);
  while (window) {
    app.Frame(&window);
  }
}

void Run() {
  InitializeGLFW();
  LaunchApp();
  Terminate();
}
} // namespace Terreate::Test::Graphic
