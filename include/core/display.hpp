#pragma once
#include "../vulkan/device.hpp"
#include "../vulkan/surface.hpp"
#include "../vulkan/vkobj.hpp"
#include "../vulkan/window.hpp"

namespace Terreate::Core {
class IDisplay {
public:
  virtual ~IDisplay() = default;

  virtual Vulkan::VkObjectRef<Vulkan::Window> getWindow() const = 0;
  virtual Vulkan::VkObjectRef<Vulkan::Surface> getSurface() const = 0;
  virtual Vulkan::WindowProperties &getProperties() = 0;
  virtual Vulkan::WindowEvent &getEvent() = 0;
  virtual Vulkan::WindowProperties const &getProperties() const = 0;
  virtual Vulkan::WindowEvent const &getEvent() const = 0;
  virtual bool
  getMousebuttonState(Type::MousebuttonInput const &button) const = 0;
  virtual bool getInputState(Type::InputType const &type) const = 0;
  virtual void setIcon(Vulkan::Icon const &icon) = 0;
  virtual void setCursor(Vulkan::Cursor const &cursor) = 0;
  virtual void setStandardCursor(Vulkan::StandardCursor const &cursor) = 0;
  virtual bool isClosed() const = 0;
  virtual bool isFullScreen() const = 0;
  virtual bool isWindowed() const = 0;
  virtual bool isIconified() const = 0;
  virtual bool isMaximized() const = 0;
  virtual bool isVisible() const = 0;
  virtual bool isFocused() const = 0;
  virtual bool isTransparent() const = 0;
  virtual bool isDecorated() const = 0;
  virtual bool isResizable() const = 0;
  virtual bool isFloating() const = 0;
  virtual bool isAutoIconified() const = 0;
  virtual bool isFocusOnShown() const = 0;
  virtual bool isPressing(Type::Keyboard const &key) const = 0;
  virtual bool isHovering() const = 0;
  virtual void enableVsync(bool enable) = 0;
  virtual void iconify() = 0;
  virtual void maximize() = 0;
  virtual void show() = 0;
  virtual void hide() = 0;
  virtual void focus() = 0;
  virtual void restore() = 0;
  virtual void requestAttention() = 0;
  virtual void close() = 0;
};

typedef Vulkan::Icon Icon;
typedef Vulkan::Cursor Cursor;
typedef Vulkan::StandardCursor StandardCursor;

class Display : public IDisplay {
private:
  Vulkan::VkObject<Vulkan::Window> mWindow;
  Vulkan::VkObject<Vulkan::Surface> mSurface;

public:
  Display(VkInstance instance, Vulkan::VkObject<Vulkan::Device> *device,
          Type::str const &title, Type::pair<Type::i32> const &size,
          Vulkan::WindowSettings const &settings = Vulkan::WindowSettings());
  ~Display() override;

  Vulkan::VkObjectRef<Vulkan::Window> getWindow() const override {
    return mWindow.ref();
  }
  Vulkan::VkObjectRef<Vulkan::Surface> getSurface() const override {
    return mSurface.ref();
  }
  Vulkan::WindowProperties &getProperties() override {
    return mWindow->properties;
  }
  Vulkan::WindowEvent &getEvent() override { return mWindow->events; }
  Vulkan::WindowProperties const &getProperties() const override {
    return mWindow->properties;
  }
  Vulkan::WindowEvent const &getEvent() const override {
    return mWindow->events;
  }
  bool
  getMousebuttonState(Type::MousebuttonInput const &button) const override {
    return mWindow->getMousebutton(button);
  }
  bool getInputState(Type::InputType const &type) const override {
    return mWindow->getInputState(type);
  }

  void setIcon(Icon const &icon) override { mWindow->setIcon(icon); }
  void setCursor(Cursor const &cursor) override { mWindow->setCursor(cursor); }
  void setStandardCursor(StandardCursor const &cursor) override {
    mWindow->setCursor(cursor);
  }

  bool isClosed() const override { return !mWindow || mWindow->isClosed(); }
  bool isFullScreen() const override { return mWindow->isFullScreen(); }
  bool isWindowed() const override { return mWindow->isWindowed(); }
  bool isIconified() const override { return mWindow->isIconified(); }
  bool isMaximized() const override { return mWindow->isMaximized(); }
  bool isVisible() const override { return mWindow->isVisible(); }
  bool isFocused() const override { return mWindow->isFocused(); }
  bool isTransparent() const override { return mWindow->isTransparent(); }
  bool isDecorated() const override { return mWindow->isDecorated(); }
  bool isResizable() const override { return mWindow->isResizable(); }
  bool isFloating() const override { return mWindow->isFloating(); }
  bool isAutoIconified() const override { return mWindow->isAutoIconified(); }
  bool isFocusOnShown() const override { return mWindow->isFocusOnShown(); }
  bool isPressing(Type::Keyboard const &key) const override {
    return mWindow->isPressing(key);
  }
  bool isHovering() const override { return mWindow->isEntering(); }

  void enableVsync(bool enable) override {
    enable ? mWindow->enableVsync() : mWindow->disableVsync();
  }
  void iconify() override { mWindow->iconify(); }
  void maximize() override { mWindow->maximize(); }
  void show() override { mWindow->show(); }
  void hide() override { mWindow->hide(); }
  void focus() override { mWindow->focus(); }
  void restore() override { mWindow->restore(); }
  void requestAttention() override { mWindow->requestAttention(); }
  void close() override { mWindow->close(); }
};
} // namespace Terreate::Core
