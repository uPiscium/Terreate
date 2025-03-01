#ifndef __TERREATE_GRAPHICS_JOYSTICK_HPP__
#define __TERREATE_GRAPHICS_JOYSTICK_HPP__

#include <core/math.hpp>
#include <graphic/GLdefs.hpp>
#include <types.hpp>

namespace Terreate::Graphic {
using namespace Terreate::Types;
using namespace Terreate::Math;

void JoystickCallback(int id, int event);

typedef Event<JoystickID, JoystickEvent> JoystickEventHandler;

struct JoystickAxisState {
  vec2 leftStick = vec2(0.0f);
  vec2 rightStick = vec2(0.0f);
  Float leftTrigger = 0.0f;
  Float rightTrigger = 0.0f;
};

struct JoystickButtonState {
  Bool a = false;
  Bool b = false;
  Bool x = false;
  Bool y = false;
  Bool cross = false;
  Bool circle = false;
  Bool square = false;
  Bool triangle = false;
  Bool leftBumper = false;
  Bool rightBumper = false;
  Bool back = false;
  Bool start = false;
  Bool guide = false;
  Bool leftThumb = false;
  Bool rightThumb = false;
  Bool dpadUp = false;
  Bool dpadRight = false;
  Bool dpadDown = false;
  Bool dpadLeft = false;
};

struct JoystickHatState {
  Bool up = false;
  Bool right = false;
  Bool down = false;
  Bool left = false;
};

class Joystick {
private:
  static Joystick sJoysticks[(Uint const)JoystickID::LAST + 1];

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
    glfwSetJoystickUserPointer((Uint)mJoystickId, (void *)this);
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
  JoystickID const &GetJoystickId() const { return mJoystickId; }
  /*
   * @brief: Get the user pointer of the joystick
   * @return: the user pointer of the joystick
   */
  template <typename T> T *GetUserPointer() const { return (T *)mUserPointer; }
  /*
   * @brief: Get the event of the joystick
   * @return: the event of the joystick
   */
  JoystickEventHandler &GetJoystickEvent() { return mEvent; }
  /*
   * @brief: Get the axis state of the joystick
   * @return: the state of the joystick axis
   */
  JoystickAxisState GetAxisState() const;
  /*
   * @brief: Get the botton state of the joystick
   * @return: the state of the joystick buttons
   */
  JoystickButtonState GetButtonState() const;
  /*
   * @brief: Get the hat state of the joystick
   * @return: the state of the joystick hat
   */
  JoystickHatState GetHatState() const;

  /*
   * @brief: Set the user pointer of the joystick
   * @param: ptr: the pointer to set
   */
  template <typename T> void SetUserPointer(T *ptr) {
    mUserPointer = (void *)ptr;
  }

  /*
   * @brief: Check if the joystick is a gamepad
   * @return: true if the joystick is a gamepad, false otherwise
   */
  Bool IsGamepad() const { return glfwJoystickIsGamepad((int)mJoystickId); }
  /*
   * @brief: Check if the joystick is connected
   * @return: true if the joystick is connected, false otherwise
   */
  Bool IsConnected() const { return glfwJoystickPresent((int)mJoystickId); }

  Joystick &operator=(Joystick const &joystick);

public:
  /*
   * @brief: Get the joystick with the given id
   * @param: id: the id of the joystick
   * @return: the joystick with the given id
   */
  static Joystick const &GetJoystick(JoystickID const &id) {
    return sJoysticks[(Uint const)id];
  }
  /*
   * @brief: Check if the joystick with the given id is connected
   * @param: id: the id of the joystick
   * @return: true if the joystick with the given id is connected, false
   * otherwise
   */
  static Bool IsConnected(JoystickID const &id) {
    return sJoysticks[(Uint const)id].IsConnected();
  }
};
} // namespace Terreate::Graphic

#endif // __TERREATE_GRAPHICS_JOYSTICK_HPP__
