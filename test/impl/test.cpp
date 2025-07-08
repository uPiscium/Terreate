#include "../include/test.hpp"

#include <iomanip>
#include <iostream>

namespace Terreate::Test {
// void outputJoystickData(Joystick const &joystick, Text &text, u32 const
// &width,
//                         u32 const &height) {
//   JoystickAxisState axisState = joystick.getAxisState();
//   JoystickButtonState buttonState = joystick.getButtonState();
//   JoystickHatState hatState = joystick.getHatState();

//   stream ss;
//   if (joystick.isGamepad()) {
//     ss << "Gamepad: " << (u32)joystick.getJoystickId();
//   } else {
//     ss << "Joystick: " << (u32)joystick.getJoystickId();
//   }

//   if (!joystick.isConnected()) {
//     ss << " Disconnected";
//     text.loadText(ss.str());
//     text.render(0, 1500, width, height);
//     return;
//   } else {
//     ss << " Connected";
//     text.loadText(ss.str());
//     text.render(0, 1500, width, height);
//   }

//   ss.str("");
//   ss << "Left Stick: (" << std::fixed << std::setprecision(3)
//      << axisState.leftStick[0] << ", " << axisState.leftStick[1] << ")";
//   ss << " / Right Stick: (" << std::fixed << std::setprecision(3)
//      << axisState.rightStick[0] << ", " << axisState.rightStick[1] << ")";
//   ss << std::fixed << std::setprecision(3)
//      << " / Left Trigger: " << axisState.leftTrigger
//      << " / Right Trigger: " << axisState.rightTrigger;
//   text.loadText(ss.str());
//   text.render(0, 1450, width, height);

//   ss.str("");
//   ss << "A: " << buttonState.a << " / B: " << buttonState.b
//      << " / X: " << buttonState.x << " / Y: " << buttonState.y;
//   ss << " / Cross: " << buttonState.cross << " / Circle: " <<
//   buttonState.circle
//      << " / Square: " << buttonState.square
//      << " / Triangle: " << buttonState.triangle;
//   text.loadText(ss.str());
//   text.render(0, 1400, width, height);

//   ss.str("");
//   ss << "Left Bumper: " << buttonState.leftBumper
//      << " / Right Bumper: " << buttonState.rightBumper
//      << " / Back: " << buttonState.back << " / Start: " << buttonState.start
//      << " / Guide: " << buttonState.guide
//      << " / Left Thumb: " << buttonState.leftThumb
//      << " / Right Thumb: " << buttonState.rightThumb;
//   text.loadText(ss.str());
//   text.render(0, 1350, width, height);

//   ss.str("");
//   ss << "D-Pad: Up: " << buttonState.dpadUp
//      << " / Right: " << buttonState.dpadRight
//      << " / Down: " << buttonState.dpadDown
//      << " / Left: " << buttonState.dpadLeft;
//   text.loadText(ss.str());
//   text.render(0, 1300, width, height);

//   ss.str("");
//   ss << "Hat: Up: " << hatState.up << " / Right: " << hatState.right
//      << " / Down: " << hatState.down << " / Left: " << hatState.left;
//   text.loadText(ss.str());
//   text.render(0, 1250, width, height);
// }

// void testCompute() {
//   vec<float> inputData = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};

//   ShaderStorageBuffer input, input2, output, output2;
//   input.loadData(inputData);
//   output.allocate(input.getSize());
//   input2.loadData(inputData);
//   output2.allocate(input2.getSize());
//   std::cout << "input size: " << input.getSize() << std::endl;
//   std::cout << "output size: " << output.getSize() << std::endl;

//   ComputeKernel kernel;
//   kernel.addKernelSource(
//       Shader::loadShaderSource("resources/shaders/compute.glsl"));
//   kernel.compile();
//   kernel.link();
//   std::cout << "kernel compiled" << std::endl;

//   input.bind(kernel, "InputBuffer");
//   output.bind(kernel, "OutputBuffer");
//   std::cout << "buffers bound" << std::endl;

//   kernel.setUniform("scaleFactor", 2.0f);
//   kernel.dispatch(10, 1, 1);
//   std::cout << "dispatching kernel" << std::endl;

//   ComputeKernel kernel2;
//   kernel2.addKernelSource(
//       Shader::loadShaderSource("resources/shaders/compute.glsl"));
//   kernel2.compile();
//   kernel2.link();
//   std::cout << "kernel2 compiled" << std::endl;

//   input2.bind(kernel2, "InputBuffer");
//   output2.bind(kernel2, "OutputBuffer");
//   std::cout << "buffers bound" << std::endl;

//   kernel2.setUniform("scaleFactor", 3.0f);
//   kernel2.dispatch(10, 1, 1);
//   std::cout << "dispatching kernel2" << std::endl;

//   vec<float> outputData, outputData2;
//   output.getData(outputData);
//   output2.getData(outputData2);
//   for (int i = 0; i < inputData.size(); ++i) {
//     std::cout << "output[" << i << "] = " << outputData[i] << std::endl;
//   }
//   for (int i = 0; i < inputData.size(); ++i) {
//     std::cout << "output2[" << i << "] = " << outputData2[i] << std::endl;
//   }
// }

// void TestApp::sizeCallback(Window *window, int const &width,
//                            int const &height) {
//   glViewport(0, 0, width, height);
//   mWidth = (float)width;
//   mHeight = (float)height;
//   mShader.bind();
//   mUniform.transform = scale(
//       identity<mat4>(), vec3(1.0f / mWidth, 1.0f / mHeight, 1.0f / mDepth));
//   mUBO.reloadData(mUniform);
// }

// void TestApp::keyCallback(Window *window, Key const &key) {
//   if (key.key == Keyboard::K_ESCAPE) {
//     window->close();
//   }

//   if (key.key == Keyboard::K_BACKSPACE && key.pressed) {
//     if (!mTextString.empty() &&
//         (mDelflag > mDelInterval || mPressingFlag > 1)) {
//       mDelflag = 0u;
//       ++mPressingFlag;
//       mTextString.pop_back();
//       mText.loadText(mTextString);
//     }
//   } else {
//     mPressingFlag = 0u;
//   }
// }

// void TestApp::charCallback(Window *window, u32 const &codepoint) {
//   mTextString.push_back(codepoint);
//   mText.loadText(mTextString);
// }

TestApp::TestApp() : mScreen(1000, 1000, 4) {
  mFont = std::make_unique<Font>("resources/AsebiMin-Light.otf", 128);
  mInfoFont = std::make_unique<Font>("resources/AsebiMin-Light.otf", 32);

  mText.loadFont(mFont.get());
  mText.loadShader("resources/shaders/textVert.glsl",
                   "resources/shaders/textFrag.glsl");

  mInfoText.loadFont(mInfoFont.get());
  mInfoText.loadShader("resources/shaders/textVert.glsl",
                       "resources/shaders/textFrag.glsl");

  mShader.addVertexShaderSource(
      Shader::loadShaderSource("resources/shaders/mainVert.glsl"));
  mShader.addFragmentShaderSource(
      Shader::loadShaderSource("resources/shaders/mainFrag.glsl"));
  mShader.compile();
  mShader.link();

  mScreenShader.addVertexShaderSource(
      Shader::loadShaderSource("resources/shaders/screenVert.glsl"));
  mScreenShader.addFragmentShaderSource(
      Shader::loadShaderSource("resources/shaders/screenFrag.glsl"));
  mScreenShader.compile();
  mScreenShader.link();

  umap<str, u32> attrs = {{"iPosition", 0}, {"iUV", 1}, {"iColor", 2}};
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
  mBuffer.loadData(bdc, attrs);

  BufferDataConstructor screenBDC;
  screenBDC.AddVertexComponent("iPosition", {{-0.8f, -0.8f, 0.2f},
                                             {0.8f, -0.8f, 0.2f},
                                             {0.8f, 0.8f, 0.2f},
                                             {-0.8f, 0.8f, 0.2f}});
  screenBDC.AddVertexComponent(
      "iUV", {{0.0f, 0.0f}, {1.0f, 0.0f}, {1.0f, 1.0f}, {0.0f, 1.0f}});
  screenBDC.SetVertexIndices({{0, 0}, {1, 1}, {2, 2}, {3, 3}});
  screenBDC.Construct();
  mScreenBuffer.loadData(screenBDC, attrs);

  mColorDataConstructor.AddVertexComponent("iColor", {{1, 0, 0}});
  mColorDataConstructor.SetVertexIndices(
      {{0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0},
       {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}, {0}});
  mColorDataConstructor.Construct();
  mBuffer.loadData(mColorDataConstructor, attrs);

  mBuffer.loadIndices({{0, 1, 2, 2, 3, 0},
                       {4, 5, 6, 6, 7, 4},
                       {8, 9, 10, 10, 11, 8},
                       {12, 13, 14, 14, 15, 12},
                       {16, 17, 18, 18, 19, 16},
                       {20, 21, 22, 22, 23, 20}});
  mScreenBuffer.loadIndices({0, 1, 2, 2, 3, 0});

  mat4 view = lookAt(vec3(0, 0, 2), vec3(0, 0, 0), vec3(0, 1, 0));
  mat4 proj = perspective(45.0f, 1.0f, mNear, mFar);
  mUniform.view = view;
  mScreenUniform.view = identity<mat4>();
  mUniform.proj = proj;
  mScreenUniform.proj = identity<mat4>();
  mUniform.model = identity<mat4>();
  mScreenUniform.model = identity<mat4>();
  mUBO.loadData(mUniform);
  mScreenUBO.loadData(mScreenUniform);
  mUBO.bind(mShader, "Matrices");
  mScreenUBO.bind(mScreenShader, "Matrices");

  mSettings = {{1.0f, 1.0f, 1.0f, 0.5f}};
  mSSBO.loadData(mSettings);
  mSSBO.bind(mShader, "Settings");

  // Uncomment if you want to break your brain...
  /* mShader.UseDepth(false); */

  mShader.bind();
  mShader.setUniform("uTexture", 0);
  Shader::activateTexture(TextureTargets::TEX_0);

  mScreenShader.bind();
  mScreenShader.setUniform("uTexture", 0);
  Shader::activateTexture(TextureTargets::TEX_0);

  mTexture2 = std::make_unique<Texture>(800, 800, 2);
  ImageConverter converter;
  i32 width = 0, height = 0, channels = 0;
  stbi_set_flip_vertically_on_load(true);
  ubyte *pixels =
      stbi_load("resources/testImage2.png", &width, &height, &channels, 4);
  converter.convert("testImage2", 1, width, height, channels, pixels,
                    *mTexture2);
  stbi_image_free(pixels);
}

void TestApp::frame(Window *window) {
  // window->pollEvents();
  window->fill(0.2, 0.2, 0.2);
  window->clear();

  /* auto state = */
  /*     Joystick::GetJoystick(JoystickID::JOYSTICK1).GetAxisState(); */
  /* float angleX = state.leftStick[0]; */
  /* float angleY = state.leftStick[1]; */
  /* mat4 model = rotate(identity<mat4>(), angleX, vec3(0, 1, 0)); */
  /* model = rotate(model, angleY, vec3(1, 0, 0)); */
  float angle = mClock.getCurrentRuntime();
  mat4 model = rotate(identity<mat4>(), angle, vec3(1, 1, 1));
  mUniform.model = model;
  mScreenUniform.model = model;
  mUBO.reloadData(mUniform);
  mScreenUBO.reloadData(mScreenUniform);

  Texture *texture = mScreen.getTexture();

  mScreen.fill({0, 0, 0});
  mScreen.clear();
  mScreen.bind();
  mScreenShader.bind();
  /* mInfoFont.Use(); */
  mTexture2->bind();
  mScreenBuffer.draw(DrawMode::TRIANGLES);
  mTexture2->unbind();
  /* mInfoFont.Unuse(); */
  mScreenShader.unbind();
  mText.loadText(wstr(L"Cube"));
  auto size = mFont->getTextSize(wstr(L"Cube"));
  mText.render(500 - size.first / 2.0, 500 - size.second / 2.0,
               mScreen.getWidth(), mScreen.getHeight());
  mScreen.unbind();

  mSettings = {
      {(std::sin(angle) + 1) / 2.0, (std::cos(angle) + 1) / 2.0, 1.0f, 0.5f}};
  mSSBO.reloadData(mSettings);

  window->bind();
  mShader.bind();
  texture->bind();
  mBuffer.draw(DrawMode::TRIANGLES);
  texture->unbind();
  mShader.unbind();

  mText = mTextString;
  mText.render(0, 0, mWidth, mHeight);

  /* AttributeData color = mBuffer["iColor"]; */
  /* float r = (state.leftTrigger + 1) / 2; */
  /* float g = (state.rightTrigger + 1) / 2; */
  /* mColorDataConstructor.ReloadVertexComponent("iColor", {{r, g, 1}}); */
  /* mColorDataConstructor.Construct(); */
  /* mBuffer.ReloadData(color, mColorDataConstructor); */
  mText.setColor({1, 0, 0});

  mInfoText = L"FPS: " + std::to_wstring(mClock.getFPS());
  mInfoText.render(0, 180, mWidth, mHeight);

  // Joystick const &joystick = Joystick::getJoystick(JoystickID::JOYSTICK1);
  // outputJoystickData(joystick, mInfoText, mWidth, mHeight);

  window->swap();
  ++mDelflag;
  mClock.tick(120);
}

void launchApp() {
  Core::Context ctx;

  auto window = ctx.createWindow(1500, 750, "Test Window");
  window->setBordered(true);
  window->setResizable(true);
  window->setFloating(false);
  window->bind();

  auto event = ctx.getEventHandler();
  auto &property = window->getProperty();

  event->onWindowPixelSizeChange.subscribe(
      [](u64 timestamp, shared<Window> window, i32 const &width,
         i32 const &height) {
        if (!window) {
          std::cerr << "Window is null in pixel size change event."
                    << std::endl;
          return;
        }
        window->setViewPort(0, 0, width, height);
        std::cout << "Window pixel size changed: " << width << "x" << height
                  << std::endl;
      });
  // event->onWindowCloseRequested.subscribe(
  //     [](u64 timestamp, shared<Window> window) {
  //       if (!window) {
  //         std::cerr << "Window is null in close event." << std::endl;
  //         return;
  //       }
  //       std::cout << "Window closed." << std::endl;
  //       window->close();
  //     });
  event->onMouseMotion.subscribe([](u64 timestamp, shared<Window> window,
                                    SDL_MouseID which, vec2 const &pos,
                                    vec2 const &rel) {
    std::cout << "Mouse motion: " << (u32)which << " at (" << pos.x << ", "
              << pos.y << ") with relative movement (" << rel.x << ", " << rel.y
              << ")" << std::endl;
  });

  event->onKey.subscribe([&window](u64 timestamp, Key const &key) {
    std::cout << "Key event: " << (u32)key.key << " pressed: " << key.pressed
              << std::endl;
    if (key.key == Keyboard::K_P && key.pressed) {
      std::cout << window->getMouse()->getCursorPosition().x << " "
                << window->getMouse()->getCursorPosition().y << std::endl;
    }
  });

  // event->onJoystickAdd.subscribe([](u64 timestamp, shared<Joystick> joystick)
  // {
  //   std::cout << "Joystick added:" << joystick->getID() << std::endl;
  //   std::cout << "Is gamepad: " << joystick->isGamepad() << std::endl;
  // });
  // event->onJoystickRemove.subscribe(
  //     [](u64 timestamp, shared<Joystick> joystick) {
  //       std::cout << "Joystick removed:" << joystick->getID() << std::endl;
  //     });
  // event->onMouseAdd.subscribe([](u64 timestamp, SDL_MouseID which) {
  //   std::cout << "Mouse added: " << which << std::endl;
  // });

  for (auto &id : Mouse::getMise()) {
    std::cout << "Mouse ID: " << id << " / Name: " << SDL_GetMouseNameForID(id)
              << std::endl;
  }

  // event.onKeyInput.subscribe([&ctx](Window *window, Key const &key) {
  //   std::cout << "Key pressed: " << (u32)key.key << std::endl;
  // });
  // event.onCharInput.subscribe([&ctx](Window *window, u32 const &codepoint) {
  //   if (codepoint == (u32)-1) {
  //     std::cout << "Invalid character input." << std::endl;
  //     return;
  //   }
  //   std::cout << "Character input: " << (char)codepoint << std::endl;
  // });

  while (ctx.valid()) {
    window->fill(0.2, 0.2, 0.2);
    window->clear();

    window->update();
    ctx.tick(120);
  }

  // Window window(2500, 1600, "Test Window", WindowSettings());
  // initializeGLAD();
  // glViewport(0, 0, 2500, 1600);
  // // window.disableVsync();

  // testCompute();

  // TestApp app;
  // window.getEvent().onFramebufferSizeChange.subscribe(
  //     [&app](Window *window, u32 const &width, u32 const &height) {
  //       app.sizeCallback(window, width, height);
  //     });
  // window.getEvent().onKeyInput.subscribe(
  //     [&app](Window *window, Key const &key) { app.keyCallback(window, key);
  //     });
  // window.getEvent().onCharInput.subscribe(
  //     [&app](Window *window, u32 const &codepoint) {
  //       app.charCallback(window, codepoint);
  //     });

  // glViewport(0, 0, 2500, 1600);
  // while (window) {
  //   app.frame(&window);
  // }
}
} // namespace Terreate::Test

int main() {
  Terreate::Test::launchApp();
  return 0;
}
