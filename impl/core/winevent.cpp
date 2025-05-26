#include "../../include/core/window.hpp"
#include "../../include/core/winevent.hpp"

namespace Terreate::Core {
namespace Wrapper {
void windowPositionCallbackWrapper(GLFWwindow *window, int xpos, int ypos) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->properties.position = Type::pair<Type::i32>(xpos, ypos);
  ptr->events.onPositionChange.publish(ptr, xpos, ypos);
}

void windowSizeCallbackWrapper(GLFWwindow *window, int width, int height) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->properties.size = Type::pair<Type::u32>(width, height);
  ptr->events.onSizeChange.publish(ptr, width, height);
}

void windowCloseCallbackWrapper(GLFWwindow *window) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->events.onClose.publish(ptr);
}

void windowRefreshCallbackWrapper(GLFWwindow *window) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->events.onRefresh.publish(ptr);
}

void windowFocusCallbackWrapper(GLFWwindow *window, int focused) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->events.onFocus.publish(ptr, (bool)focused);
}

void windowIconifyCallbackWrapper(GLFWwindow *window, int iconified) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->events.onIconify.publish(ptr, (bool)iconified);
}

void windowMaximizeCallbackWrapper(GLFWwindow *window, int maximized) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->events.onMaximize.publish(ptr, (bool)maximized);
}

void windowFramebufferSizeCallbackWrapper(GLFWwindow *window, int width,
                                          int height) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->properties.framebufferSize = Type::pair<Type::i32>(width, height);
  ptr->events.onFramebufferSizeChange.publish(ptr, width, height);
}

void windowContentScaleCallbackWrapper(GLFWwindow *window, float xscale,
                                       float yscale) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->events.onContentScaleChange.publish(ptr, xscale, yscale);
}

void mousebuttonCallbackWrapper(GLFWwindow *window, int button, int action,
                                int mods) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->events.onMousebuttonInput.publish(ptr, button, action, Modifier(mods));
}

void cursorPositionCallbackWrapper(GLFWwindow *window, double xpos,
                                   double ypos) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->properties.cursorPosition = Type::pair<double>(xpos, ypos);
  ptr->events.onCursorPositionChange.publish(ptr, xpos, ypos);
}

void cursorEnterCallbackWrapper(GLFWwindow *window, int entered) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->events.onCursorEnter.publish(ptr, (bool)entered);
}

void scrollCallbackWrapper(GLFWwindow *window, double xoffset, double yoffset) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->properties.scrollOffset = Type::pair<double>(xoffset, yoffset);
  ptr->events.onScroll.publish(ptr, xoffset, yoffset);
}

void keyCallbackWrapper(GLFWwindow *window, int key, int scancode, int action,
                        int mods) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  Key wrappedKey = Key(key, scancode, action, mods);
  ptr->properties.keys.get().push_back(wrappedKey);
  ptr->events.onKeyInput.publish(ptr, wrappedKey);
}

void charCallbackWrapper(GLFWwindow *window, Type::u32 codepoint) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->properties.codePoints.get().push_back(codepoint);
  ptr->events.onCharInput.publish(ptr, codepoint);
}

void dropCallbackWrapper(GLFWwindow *window, int count, const char **paths) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  Type::vec<Type::str> droppedFiles(paths, paths + count);
  ptr->properties.droppedFiles = droppedFiles;
  ptr->events.onFileDrop.publish(ptr, droppedFiles);
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
