#include <TerreateTest.hpp>

#include <iostream>

namespace Terreate::Test {

class TestInterface {
public:
  ~TestInterface() {
    std::cout << "TestInterface::~TestInterface" << std::endl;
  }
  virtual void Test() = 0;
};

class TestImplementation : public TestInterface {
public:
  TestImplementation() {}
  ~TestImplementation() {
    std::cout << "TestImplementation::~TestImplementation" << std::endl;
  }
  void Test() override { std::cout << "TestImplementation::Test" << std::endl; }
};

class App {
private:
  Str vsrc;
  Str fsrc;
  Resource<Renderer> renderer;
  Point point;

public:
  App() {
    vsrc = Graphic::Shader::LoadShaderSource(
        "tests/resources/shaders/pointVert.glsl");
    fsrc = Graphic::Shader::LoadShaderSource(
        "tests/resources/shaders/pointFrag.glsl");
    renderer = Resource<Renderer>::Create(vsrc, fsrc);
  }

  Bool FrameFunction(Context *context) {
    point = Point({0, 0, 0}, 10);
    context->window->Fill(1.0f, 1.0f, 0.0f);
    context->window->Clear();
    point.Draw(renderer);
    context->window->Swap();
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    return !context->window->IsPressing(Graphic::Keyboard::K_ESCAPE);
  }

  Bool FrameFunction2(Context *context) {
    context->window->Fill(0.0f, 1.0f, 1.0f);
    context->window->Clear();
    context->window->Swap();
    return !context->window->IsPressing(Graphic::Keyboard::K_ESCAPE);
  }

  Bool FrameFunction3(Context *context) {
    context->window->Fill(1.0f, 0.0f, 1.0f);
    context->window->Clear();
    context->window->Swap();
    return !context->window->IsPressing(Graphic::Keyboard::K_ESCAPE);
  }
};

void Run() {
  ContextHandler handler;
  App app;
  auto context1 = handler.CreateContext(800, 600, "Window 1");
  auto context2 = handler.CreateContext(800, 600, "Window 2");
  auto context3 = handler.CreateContext(800, 600, "Window 3");
  auto context4 = handler.CreateContext(800, 600, "Window 4");

  auto key =
      KeySubscriber([&](Graphic::Window *window, Graphic::Key const &key) {
        if (key.key == Graphic::Keyboard::K_Q && key.pressed)
          handler.Quit();
      });

  /* context1->window->onKeyInput += */
  /*     KeySubscriber([&](Graphic::Window *window, Graphic::Key const &key) {
   */
  /*       if (key.key == Graphic::Keyboard::K_ENTER && key.pressed) { */
  /*         auto context = handler.CreateContext(400, 300, "Sub window"); */
  /*         context->onEnd += ContextSubscriber([&](Context *context) { */
  /*           std::cout << "Sub window has ended" << std::endl; */
  /*         }); */
  /*         context->Run( */
  /*             [&app](Context *context) { return app.FrameFunction2(context);
   * }); */
  /*       } */
  /*     }); */

  /* context1->window->onKeyInput += key; */
  /* context2->window->onKeyInput += key; */
  /* context3->window->onKeyInput += key; */
  context4->window->onKeyInput += key;

  auto fb = WindowFramebufferSizeSubscriber(
      [&](Graphic::Window *window, Uint const &width, Uint const &height) {
        handler.SetViewport(width, height);
      });

  context4->window->onFramebufferSizeChange += fb;

  context1->onEnd += ContextSubscriber([&](Context *context) {
    std::cout << "Window 1 has ended" << std::endl;
  });
  context2->onEnd += ContextSubscriber([&](Context *context) {
    std::cout << "Window 2 has ended" << std::endl;
  });
  context3->onEnd += ContextSubscriber([&](Context *context) {
    std::cout << "Window 3 has ended" << std::endl;
  });
  context4->onEnd += ContextSubscriber([&](Context *context) {
    std::cout << "Window 4 has ended" << std::endl;
  });

  /* Resource<TestInterface> t = Resource<TestImplementation>::Create(); */
  /* t->Test(); */
  /* t.Delete(); */

  context1->Run(
      [&app](Context *context) { return app.FrameFunction2(context); });
  context2->Run(
      [&app](Context *context) { return app.FrameFunction2(context); });
  context3->Run(
      [&app](Context *context) { return app.FrameFunction2(context); });
  context4->Run(
      [&app](Context *context) { return app.FrameFunction(context); });

  while (handler.IsRunning()) {
    glfwPollEvents();
    std::this_thread::sleep_for(std::chrono::milliseconds(50));
  }
}

} // namespace Terreate::Test
