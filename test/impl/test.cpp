#include "../include/test.hpp"

#include <iostream>

using namespace Terreate;
using namespace Terreate::SDL;
using namespace Terreate::OpenGL;

int main() {
  Core::Context ctx;

  auto window = ctx.createWindow(1200, 1200, "Test Window");
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
      });
  // event->onMouseMotion.subscribe([](u64 timestamp, shared<Window> window,
  //                                   shared<Mouse> mouse, vec2 const &pos,
  //                                   vec2 const &rel) {
  //   std::cout << "Mouse motion: " << mouse->getName() << " at (" << pos.x
  //             << ", " << pos.y << ") with relative movement (" << rel.x << ",
  //             "
  //             << rel.y << ")" << std::endl;
  // });

  event->onKey.subscribe([&window](u64 timestamp, Key const &key) {
    if (key.pressed && key.key == Keyboard::K_ESCAPE) {
      std::cout << "Escape key pressed, closing window." << std::endl;
      window->close();
    }
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

  auto resourceController =
      ctx.createController<Resource::ResourceController>();
  auto componentController =
      ctx.createController<Component::ComponentController>();

  shared<Resource::MeshManager> meshManager =
      resourceController->createManager<Resource::MeshManager>();
  shared<Resource::Mesh> mesh =
      meshManager->createPrimitive<Resource::Circle>(0.5f);
  // mesh->setDrawMode(DrawMode::LINE_LOOP);

  shared<Core::Entity> entity = ctx.createEntity();

  shared<Component::MeshSystem> meshSystem =
      componentController->createSystem<Component::MeshSystem>();
  shared<Component::Mesh> meshComponent = meshSystem->create(mesh);

  entity->addComponent(meshComponent);

  OpenGL::Shader shader;
  str vert = shader.loadShaderSource("resources/shaders/rect.vert.glsl");
  str frag = shader.loadShaderSource("resources/shaders/rect.frag.glsl");
  shader.addVertexShaderSource(vert);
  shader.addFragmentShaderSource(frag);
  shader.compile();
  shader.link();

  while (ctx.valid()) {
    window->fill(0, 0, 0);
    window->clear();

    shader.bind();
    meshComponent->draw();
    shader.unbind();

    window->update();
    ctx.tick(120);
  }

  return 0;
}
