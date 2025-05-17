#include <core/window.hpp>
#include <core/winevent.hpp>

namespace Terreate::Core {
namespace Wrappers {
void windowPositionCallbackWrapper(GLFWwindow *window, int xpos, int ypos) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->properties.position = pair<i32>(xpos, ypos);
  ptr->events.onPositionChange.publish(ptr, xpos, ypos);
}

void windowSizeCallbackWrapper(GLFWwindow *window, int width, int height) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->properties.size = pair<u32>(width, height);
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
  ptr->properties.cursorPosition = pair<double>(xpos, ypos);
  ptr->events.onCursorPositionChange.publish(ptr, xpos, ypos);
}

void cursorEnterCallbackWrapper(GLFWwindow *window, int entered) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->events.onCursorEnter.publish(ptr, (bool)entered);
}

void scrollCallbackWrapper(GLFWwindow *window, double xoffset, double yoffset) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->properties.scrollOffset = pair<double>(xoffset, yoffset);
  ptr->events.onScroll.publish(ptr, xoffset, yoffset);
}

void keyCallbackWrapper(GLFWwindow *window, int key, int scancode, int action,
                        int mods) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  Key wrappedKey = Key(key, scancode, action, mods);
  ptr->properties.keys.get().push_back(wrappedKey);
  ptr->events.onKeyInput.publish(ptr, wrappedKey);
}

void charCallbackWrapper(GLFWwindow *window, u32 codepoint) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  ptr->properties.codePoints.get().push_back(codepoint);
  ptr->events.onCharInput.publish(ptr, codepoint);
}

void dropCallbackWrapper(GLFWwindow *window, int count, const char **paths) {
  Window *ptr = (Window *)glfwGetWindowUserPointer(window);
  vec<str> droppedFiles(paths, paths + count);
  ptr->properties.droppedFiles = droppedFiles;
  ptr->events.onFileDrop.publish(ptr, droppedFiles);
}
} // namespace Wrappers

WindowProperties::WindowProperties() {}

void WindowProperties::setup(GLFWwindow *window) {
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

  this->size.getter = [window](pair<i32> *size) -> pair<i32> & {
    glfwGetWindowSize(window, &(*size).first, &(*size).second);
    return *size;
  };
  this->size.setter = [window](pair<i32> *value, pair<i32> const &size) {
    glfwSetWindowSize(window, size.first, size.second);
    *value = size;
  };
  this->position.getter = [window](pair<i32> *position) -> pair<i32> & {
    glfwGetWindowPos(window, &(*position).first, &(*position).second);
    return *position;
  };
  this->position.setter = [window](pair<i32> *value,
                                   pair<i32> const &position) {
    glfwSetWindowPos(window, position.first, position.second);
    *value = position;
  };
  this->cursorPosition.getter =
      [window](pair<double> *position) -> pair<double> & {
    glfwGetCursorPos(window, &(*position).first, &(*position).second);
    return *position;
  };
  this->cursorPosition.setter = [window](pair<double> *value,
                                         pair<double> const &position) {
    glfwSetCursorPos(window, position.first, position.second);
    *value = position;
  };
  this->clipboard.getter = [window](str *clipboard) -> str & {
    char const *clipboardString = glfwGetClipboardString(window);
    if (clipboardString) {
      *clipboard = clipboardString;
    } else {
      *clipboard = "";
    }
    return *clipboard;
  };
  this->clipboard.setter = [window](str *value, str const &clipboard) {
    glfwSetClipboardString(window, clipboard.c_str());
    *value = clipboard;
  };
  this->title.getter = [window](str *title) -> str & {
    char const *titleString = glfwGetWindowTitle(window);
    if (titleString) {
      *title = titleString;
    } else {
      *title = "";
    }
    return *title;
  };
  this->title.setter = [window](str *value, str const &title) {
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

WindowEvent::WindowEvent() {}
} // namespace Terreate::Core
