#include <TerreateTest.hpp>

#include <iostream>

namespace Terreate::Test {

void RunThread(std::atomic<Bool> *running, std::atomic<Ubyte> *numWindows,
               Graphic::Window *window) {
  auto thread = std::thread([window, running, numWindows]() {
    numWindows->fetch_add(1);
    while (!window->IsClosed()) {
      window->SetCurrentContext();
      window->Fill(1.0f, 1.0f, 0.0f);
      window->Clear();

      if (window->IsPressing(Graphic::Keyboard::K_ESCAPE)) {
        break;
      }

      window->Swap();
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    window->Destroy();

    if (numWindows->fetch_sub(1) == 1) {
      running->store(false);
    }
  });

  thread.detach();
}

void Run() {
  Graphic::InitializeGLFW();
  Graphic::Window window1(800, 600, "Window 1", Graphic::WindowSettings());
  Graphic::Window window2(800, 600, "Window 2", Graphic::WindowSettings());
  Graphic::Window window3(800, 600, "Window 3", Graphic::WindowSettings());
  Graphic::Window window4(800, 600, "Window 4", Graphic::WindowSettings());
  Graphic::Window window5(800, 600, "Window 4", Graphic::WindowSettings());
  Graphic::InitializeGLAD();
  glfwMakeContextCurrent(nullptr);

  std::atomic<Bool> running = true;
  std::atomic<Ubyte> numWindows = 0;
  RunThread(&running, &numWindows, &window1);
  RunThread(&running, &numWindows, &window2);
  RunThread(&running, &numWindows, &window3);
  RunThread(&running, &numWindows, &window4);

  while (running) {
    glfwPollEvents();
  }
  Graphic::Terminate();
}

} // namespace Terreate::Test
