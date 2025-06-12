#include "../../include/core/window.hpp"
#include "../../include/core/winevent.hpp"

namespace Terreate::Core {
namespace Wrapper {
void windowPositionCallbackWrapper(GLFWwindow *window, int xpos, int ypos) {
  IWindow *ptr = (IWindow *)glfwGetWindowUserPointer(window);
  ptr->getProperties().position = Type::pair<Type::i32>(xpos, ypos);
  ptr->getEvents().onPositionChange.publish(ptr, xpos, ypos);
}

void windowSizeCallbackWrapper(GLFWwindow *window, int width, int height) {
  IWindow *ptr = (IWindow *)glfwGetWindowUserPointer(window);
  ptr->getProperties().size = Type::pair<Type::u32>(width, height);
  ptr->getEvents().onSizeChange.publish(ptr, width, height);
}

void windowCloseCallbackWrapper(GLFWwindow *window) {
  IWindow *ptr = (IWindow *)glfwGetWindowUserPointer(window);
  ptr->getEvents().onClose.publish(ptr);
}

void windowRefreshCallbackWrapper(GLFWwindow *window) {
  IWindow *ptr = (IWindow *)glfwGetWindowUserPointer(window);
  ptr->getEvents().onRefresh.publish(ptr);
}

void windowFocusCallbackWrapper(GLFWwindow *window, int focused) {
  IWindow *ptr = (IWindow *)glfwGetWindowUserPointer(window);
  ptr->getEvents().onFocus.publish(ptr, (bool)focused);
}

void windowIconifyCallbackWrapper(GLFWwindow *window, int iconified) {
  IWindow *ptr = (IWindow *)glfwGetWindowUserPointer(window);
  ptr->getEvents().onIconify.publish(ptr, (bool)iconified);
}

void windowMaximizeCallbackWrapper(GLFWwindow *window, int maximized) {
  IWindow *ptr = (IWindow *)glfwGetWindowUserPointer(window);
  ptr->getEvents().onMaximize.publish(ptr, (bool)maximized);
}

void windowFramebufferSizeCallbackWrapper(GLFWwindow *window, int width,
                                          int height) {
  IWindow *ptr = (IWindow *)glfwGetWindowUserPointer(window);
  ptr->getProperties().framebufferSize = Type::pair<Type::i32>(width, height);
  ptr->getEvents().onFramebufferSizeChange.publish(ptr, width, height);
}

void windowContentScaleCallbackWrapper(GLFWwindow *window, float xscale,
                                       float yscale) {
  IWindow *ptr = (IWindow *)glfwGetWindowUserPointer(window);
  ptr->getEvents().onContentScaleChange.publish(ptr, xscale, yscale);
}

void mousebuttonCallbackWrapper(GLFWwindow *window, int button, int action,
                                int mods) {
  IWindow *ptr = (IWindow *)glfwGetWindowUserPointer(window);
  ptr->getEvents().onMousebuttonInput.publish(ptr, button, action,
                                              Modifier(mods));
}

void cursorPositionCallbackWrapper(GLFWwindow *window, double xpos,
                                   double ypos) {
  IWindow *ptr = (IWindow *)glfwGetWindowUserPointer(window);
  ptr->getProperties().cursorPosition = Type::pair<double>(xpos, ypos);
  ptr->getEvents().onCursorPositionChange.publish(ptr, xpos, ypos);
}

void cursorEnterCallbackWrapper(GLFWwindow *window, int entered) {
  IWindow *ptr = (IWindow *)glfwGetWindowUserPointer(window);
  ptr->getEvents().onCursorEnter.publish(ptr, (bool)entered);
}

void scrollCallbackWrapper(GLFWwindow *window, double xoffset, double yoffset) {
  IWindow *ptr = (IWindow *)glfwGetWindowUserPointer(window);
  ptr->getProperties().scrollOffset = Type::pair<double>(xoffset, yoffset);
  ptr->getEvents().onScroll.publish(ptr, xoffset, yoffset);
}

void keyCallbackWrapper(GLFWwindow *window, int key, int scancode, int action,
                        int mods) {
  IWindow *ptr = (IWindow *)glfwGetWindowUserPointer(window);
  Key wrappedKey = Key(key, scancode, action, mods);
  ptr->getProperties().keys.get().push_back(wrappedKey);
  ptr->getEvents().onKeyInput.publish(ptr, wrappedKey);
}

void charCallbackWrapper(GLFWwindow *window, Type::u32 codepoint) {
  IWindow *ptr = (IWindow *)glfwGetWindowUserPointer(window);
  ptr->getProperties().codePoints.get().push_back(codepoint);
  ptr->getEvents().onCharInput.publish(ptr, codepoint);
}

void dropCallbackWrapper(GLFWwindow *window, int count, const char **paths) {
  IWindow *ptr = (IWindow *)glfwGetWindowUserPointer(window);
  Type::vec<Type::str> droppedFiles(paths, paths + count);
  ptr->getProperties().droppedFiles = droppedFiles;
  ptr->getEvents().onFileDrop.publish(ptr, droppedFiles);
}
} // namespace Wrapper

void WindowProperties::setup(GLFWwindow *window) {
  this->framebufferSize.set(&mFramebufferSize);
  this->scrollOffset.set(&mScrollOffset);
  this->codePoints.set(&mCodePoints);
  this->keys.set(&mKeys);
  this->droppedFiles.set(&mDroppedFiles);

  this->size.set(&mSize);
  this->position.set(&mPosition);
  this->cursorPosition.set(&mCursorPosition);
  this->clipboard.set(&mClipboard);
  this->title.set(&mTitle);
  this->opacity.set(&mOpacity);

  this->framebufferSize.getter =
      [window](
          Type::pair<Type::i32> *framebufferSize) -> Type::pair<Type::i32> & {
    glfwGetFramebufferSize(window, &(*framebufferSize).first,
                           &(*framebufferSize).second);
    return *framebufferSize;
  };

  this->size.getter =
      [window](Type::pair<Type::i32> *size) -> Type::pair<Type::i32> & {
    glfwGetWindowSize(window, &(*size).first, &(*size).second);
    return *size;
  };
  this->size.setter = [window](Type::pair<Type::i32> *value,
                               Type::pair<Type::i32> const &size) {
    glfwSetWindowSize(window, size.first, size.second);
    *value = size;
  };
  this->position.getter =
      [window](Type::pair<Type::i32> *position) -> Type::pair<Type::i32> & {
    glfwGetWindowPos(window, &(*position).first, &(*position).second);
    return *position;
  };
  this->position.setter = [window](Type::pair<Type::i32> *value,
                                   Type::pair<Type::i32> const &position) {
    glfwSetWindowPos(window, position.first, position.second);
    *value = position;
  };
  this->cursorPosition.getter =
      [window](Type::pair<double> *position) -> Type::pair<double> & {
    glfwGetCursorPos(window, &(*position).first, &(*position).second);
    return *position;
  };
  this->cursorPosition.setter = [window](Type::pair<double> *value,
                                         Type::pair<double> const &position) {
    glfwSetCursorPos(window, position.first, position.second);
    *value = position;
  };
  this->clipboard.getter = [window](Type::str *clipboard) -> Type::str & {
    char const *clipboardString = glfwGetClipboardString(window);
    if (clipboardString) {
      *clipboard = clipboardString;
    } else {
      *clipboard = "";
    }
    return *clipboard;
  };
  this->clipboard.setter = [window](Type::str *value,
                                    Type::str const &clipboard) {
    glfwSetClipboardString(window, clipboard.c_str());
    *value = clipboard;
  };
  this->title.getter = [window](Type::str *title) -> Type::str & {
    char const *titleString = glfwGetWindowTitle(window);
    if (titleString) {
      *title = titleString;
    } else {
      *title = "";
    }
    return *title;
  };
  this->title.setter = [window](Type::str *value, Type::str const &title) {
    glfwSetWindowTitle(window, title.c_str());
    *value = title;
  };
  this->opacity.getter = [window](float *opacity) -> float & {
    *opacity = glfwGetWindowOpacity(window);
    return *opacity;
  };
  this->opacity.setter = [window](float *value, float const &opacity) {
    glfwSetWindowOpacity(window, opacity);
    *value = opacity;
  };
}
} // namespace Terreate::Core
