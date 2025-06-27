#pragma once

#include "../common/enum.hpp"
#include "../common/event.hpp"
#include "../common/math.hpp"
#include "../common/type.hpp"

namespace Terreate::OpenGL {
void joystickCallback(int id, int event);

typedef Event<JoystickID, JoystickEvent> JoystickEventHandler;

struct JoystickAxisState {
  vec2 leftStick = vec2(0.0f);
  vec2 rightStick = vec2(0.0f);
  float leftTrigger = 0.0f;
  float rightTrigger = 0.0f;
};

struct JoystickButtonState {
  bool a = false;
  bool b = false;
  bool x = false;
  bool y = false;
  bool cross = false;
  bool circle = false;
  bool square = false;
  bool triangle = false;
  bool leftBumper = false;
  bool rightBumper = false;
  bool back = false;
  bool start = false;
  bool guide = false;
  bool leftThumb = false;
  bool rightThumb = false;
  bool dpadUp = false;
  bool dpadRight = false;
  bool dpadDown = false;
  bool dpadLeft = false;
};

struct JoystickHatState {
  bool up = false;
  bool right = false;
  bool down = false;
  bool left = false;
};

class Joystick {
private:
  static Joystick sJoysticks[(u32 const)JoystickID::LAST + 1];

private:
  friend void joystickCallback(int id, int event);

private:
  JoystickID mJoystickId = JoystickID::NONE;
  void *mUserPointer = nullptr;
  JoystickEventHandler mEvent;

public:
  Joystick() = default;
  Joystick(JoystickID const &id) : mJoystickId(id) {
    glfwSetJoystickUserPointer((u32)mJoystickId, (void *)this);
  }
  Joystick(Joystick const &joystick) : mJoystickId(joystick.mJoystickId) {}
  ~Joystick() = default;

  JoystickID const &getJoystickId() const { return mJoystickId; }
  template <typename T> T *getUserPointer() const { return (T *)mUserPointer; }
  JoystickEventHandler &getJoystickEvent() { return mEvent; }
  JoystickAxisState getAxisState() const;
  JoystickButtonState getButtonState() const;
  JoystickHatState getHatState() const;

  template <typename T> void setUserPointer(T *ptr) {
    mUserPointer = (void *)ptr;
  }

  bool isGamepad() const { return glfwJoystickIsGamepad((int)mJoystickId); }
  bool isConnected() const { return glfwJoystickPresent((int)mJoystickId); }

  Joystick &operator=(Joystick const &joystick);

public:
  static Joystick const &getJoystick(JoystickID const &id) {
    return sJoysticks[(u32 const)id];
  }
  static bool isConnected(JoystickID const &id) {
    return sJoysticks[(u32 const)id].isConnected();
  }
};
} // namespace Terreate::OpenGL
