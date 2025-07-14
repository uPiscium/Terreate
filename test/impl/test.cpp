#include "../include/test.hpp"

#include <iostream>

using namespace Terreate;
using namespace Terreate::SDL;
using namespace Terreate::OpenGL;

int main() {
  Core::Context ctx;

  auto window = ctx.createWindow(1500, 750, "Test Window");
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
  event->onMouseMotion.subscribe([](u64 timestamp, shared<Window> window,
                                    shared<Mouse> mouse, vec2 const &pos,
                                    vec2 const &rel) {
    std::cout << "Mouse motion: " << mouse->getName() << " at (" << pos.x
              << ", " << pos.y << ") with relative movement (" << rel.x << ", "
              << rel.y << ")" << std::endl;
  });

  event->onKey.subscribe([&window](u64 timestamp, Key const &key) {
    std::cout << "Key event: " << (u32)key.key << " pressed: " << key.pressed
              << std::endl;
    if (key.key == Keyboard::K_P && key.pressed) {
      std::cout << window->getMouse()->getCursorPosition().x << " "
                << window->getMouse()->getCursorPosition().y << std::endl;
    }

    if (key.key == Keyboard::K_ESCAPE && key.pressed) {
      std::cout << "Escape key pressed, closing window." << std::endl;
      window->close();
    }
  });

  event->onTextInput.subscribe(
      [](u64 timestamp, shared<Window> window, str const &text) {
        std::cout << "Text input: " << text << std::endl;
      });

  // event->onCameraAdd.subscribe([](u64 timestamp, shared<Camera> camera) {
  //   camera->open();
  //   std::cout << "Camera added: " << camera->getID() << std::endl;
  //   std::cout << "Camera spec: " << camera->getSpec().width << "x"
  //             << camera->getSpec().height << std::endl;
  // });
  // event->onCameraRemove.subscribe([](u64 timestamp, shared<Camera> camera) {
  //   std::cout << "Camera removed: " << camera->getID() << std::endl;
  //   camera->close();
  // });

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

  // for (auto &id : Mouse::getMise()) {
  //   std::cout << "Mouse ID: " << id << " / Name: " <<
  //   SDL_GetMouseNameForID(id)
  //             << std::endl;
  // }

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

  return 0;
}
