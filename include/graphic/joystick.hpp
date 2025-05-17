#ifndef __TERREATE_GRAPHICS_JOYSTICK_HPP__
#define __TERREATE_GRAPHICS_JOYSTICK_HPP__

#include <core/math.hpp>
#include <core/type.hpp>
#include <graphic/GLdefs.hpp>

namespace Terreate::Graphic {
using namespace Terreate::Types;
using namespace Terreate::Math;

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
  friend void JoystickCallback(int id, int event);

private:
  JoystickID mJoystickId = JoystickID::NONE;
  void *mUserPointer = nullptr;
  JoystickEventHandler mEvent;

public:
  /*
   * @brief: Joystick is a class that represents a joystick.
   */
  Joystick() = default;
  /*
   * @brief: Joystick is a class that represents a joystick.
   * @param: id: the id of the joystick
   */
  Joystick(JoystickID const &id) : mJoystickId(id) {
    glfwSetJoystickUserPointer((u32)mJoystickId, (void *)this);
  }
  /*
   * @brief: Joystick is a class that represents a joystick.
   * @param: id: the id of the joystick
   */
  Joystick(Joystick const &joystick) : mJoystickId(joystick.mJoystickId) {}
  ~Joystick() = default;

  /*
   * @brief: Get the id of the joystick
   * @return: the id of the joystick
   */
  JoystickID const &getJoystickId() const { return mJoystickId; }
  /*
   * @brief: Get the user pointer of the joystick
   * @return: the user pointer of the joystick
   */
  template <typename T> T *getUserPointer() const { return (T *)mUserPointer; }
  /*
   * @brief: Get the event of the joystick
   * @return: the event of the joystick
   */
  JoystickEventHandler &getJoystickEvent() { return mEvent; }
  /*
   * @brief: Get the axis state of the joystick
   * @return: the state of the joystick axis
   */
  JoystickAxisState getAxisState() const;
  /*
   * @brief: Get the botton state of the joystick
   * @return: the state of the joystick buttons
   */
  JoystickButtonState getButtonState() const;
  /*
   * @brief: Get the hat state of the joystick
   * @return: the state of the joystick hat
   */
  JoystickHatState getHatState() const;

  /*
   * @brief: Set the user pointer of the joystick
   * @param: ptr: the pointer to set
   */
  template <typename T> void setUserPointer(T *ptr) {
    mUserPointer = (void *)ptr;
  }

  /*
   * @brief: Check if the joystick is a gamepad
   * @return: true if the joystick is a gamepad, false otherwise
   */
  bool isGamepad() const { return glfwJoystickIsGamepad((int)mJoystickId); }
  /*
   * @brief: Check if the joystick is connected
   * @return: true if the joystick is connected, false otherwise
   */
  bool isConnected() const { return glfwJoystickPresent((int)mJoystickId); }

  Joystick &operator=(Joystick const &joystick);

public:
  /*
   * @brief: Get the joystick with the given id
   * @param: id: the id of the joystick
   * @return: the joystick with the given id
   */
  static Joystick const &getJoystick(JoystickID const &id) {
    return sJoysticks[(u32 const)id];
  }
  /*
   * @brief: Check if the joystick with the given id is connected
   * @param: id: the id of the joystick
   * @return: true if the joystick with the given id is connected, false
   * otherwise
   */
  static bool isConnected(JoystickID const &id) {
    return sJoysticks[(u32 const)id].isConnected();
  }
};
} // namespace Terreate::Graphic

#endif // __TERREATE_GRAPHICS_JOYSTICK_HPP__
