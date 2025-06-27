#include "../../../include/module/opengl/joystick.hpp"

namespace Terreate::OpenGL {
Joystick Joystick::sJoysticks[(u32 const)JoystickID::LAST + 1] = {
    Joystick(JoystickID::JOYSTICK1),  Joystick(JoystickID::JOYSTICK2),
    Joystick(JoystickID::JOYSTICK3),  Joystick(JoystickID::JOYSTICK4),
    Joystick(JoystickID::JOYSTICK5),  Joystick(JoystickID::JOYSTICK6),
    Joystick(JoystickID::JOYSTICK7),  Joystick(JoystickID::JOYSTICK8),
    Joystick(JoystickID::JOYSTICK9),  Joystick(JoystickID::JOYSTICK10),
    Joystick(JoystickID::JOYSTICK11), Joystick(JoystickID::JOYSTICK12),
    Joystick(JoystickID::JOYSTICK13), Joystick(JoystickID::JOYSTICK14),
    Joystick(JoystickID::JOYSTICK15), Joystick(JoystickID::JOYSTICK16),
};

void joystickCallback(int id, int event) {
  JoystickID JID = (JoystickID)id;
  Joystick *joystick = (Joystick *)glfwGetJoystickUserPointer(id);
  joystick->getJoystickEvent().publish(JID, (JoystickEvent)event);
}

JoystickAxisState Joystick::getAxisState() const {
  JoystickAxisState state;

  if (!this->isConnected()) {
    return state;
  }

  if (!this->isGamepad()) {
    int count = 0;
    float const *axes = glfwGetJoystickAxes((int)mJoystickId, &count);

    if (count > (int)JoystickAxis::LEFT_X) {
      state.leftStick[0] = axes[(int)JoystickAxis::LEFT_X];
    }
    if (count > (int)JoystickAxis::LEFT_Y) {
      state.leftStick[0] = axes[(int)JoystickAxis::LEFT_Y];
    }
    if (count > (int)JoystickAxis::RIGHT_X) {
      state.leftStick[0] = axes[(int)JoystickAxis::RIGHT_X];
    }
    if (count > (int)JoystickAxis::RIGHT_Y) {
      state.rightStick[0] = axes[(int)JoystickAxis::RIGHT_Y];
    }
    if (count > (int)JoystickAxis::RIGHT_TRIGGER) {
      state.rightTrigger = axes[(int)JoystickAxis::RIGHT_TRIGGER];
    }
    if (count > (int)JoystickAxis::LEFT_TRIGGER) {
      state.leftTrigger = axes[(int)JoystickAxis::LEFT_TRIGGER];
    }
  } else {
    GLFWgamepadstate gpState;
    if (glfwGetGamepadState((int)mJoystickId, &gpState)) {
      state.leftStick[0] = gpState.axes[(int)JoystickAxis::LEFT_X];
      state.leftStick[1] = gpState.axes[(int)JoystickAxis::LEFT_Y];
      state.rightStick[0] = gpState.axes[(int)JoystickAxis::RIGHT_X];
      state.rightStick[1] = gpState.axes[(int)JoystickAxis::RIGHT_Y];
      state.leftTrigger = gpState.axes[(int)JoystickAxis::LEFT_TRIGGER];
      state.rightTrigger = gpState.axes[(int)JoystickAxis::RIGHT_TRIGGER];
    }
  }

  return state;
}

JoystickButtonState Joystick::getButtonState() const {
  JoystickButtonState state;

  if (!this->isConnected()) {
    return state;
  }

  if (!this->isGamepad()) {
    int count = 0;
    unsigned char const *buttons =
        glfwGetJoystickButtons((int)mJoystickId, &count);

    if (count > (int)JoystickButton::A) {
      state.a = buttons[(int)JoystickButton::A];
    }
    if (count > (int)JoystickButton::B) {
      state.b = buttons[(int)JoystickButton::B];
    }
    if (count > (int)JoystickButton::X) {
      state.x = buttons[(int)JoystickButton::X];
    }
    if (count > (int)JoystickButton::Y) {
      state.y = buttons[(int)JoystickButton::Y];
    }
    if (count > (int)JoystickButton::CROSS) {
      state.cross = buttons[(int)JoystickButton::CROSS];
    }
    if (count > (int)JoystickButton::CIRCLE) {
      state.circle = buttons[(int)JoystickButton::CIRCLE];
    }
    if (count > (int)JoystickButton::SQUARE) {
      state.square = buttons[(int)JoystickButton::SQUARE];
    }
    if (count > (int)JoystickButton::TRIANGLE) {
      state.triangle = buttons[(int)JoystickButton::TRIANGLE];
    }
    if (count > (int)JoystickButton::LEFT_BUMPER) {
      state.leftBumper = buttons[(int)JoystickButton::LEFT_BUMPER];
    }
    if (count > (int)JoystickButton::RIGHT_BUMPER) {
      state.rightBumper = buttons[(int)JoystickButton::RIGHT_BUMPER];
    }
    if (count > (int)JoystickButton::BACK) {
      state.back = buttons[(int)JoystickButton::BACK];
    }
    if (count > (int)JoystickButton::START) {
      state.start = buttons[(int)JoystickButton::START];
    }
    if (count > (int)JoystickButton::GUIDE) {
      state.guide = buttons[(int)JoystickButton::GUIDE];
    }
    if (count > (int)JoystickButton::LEFT_THUMB) {
      state.leftThumb = buttons[(int)JoystickButton::LEFT_THUMB];
    }
    if (count > (int)JoystickButton::RIGHT_THUMB) {
      state.rightThumb = buttons[(int)JoystickButton::RIGHT_THUMB];
    }
    if (count > (int)JoystickButton::DPAD_UP) {
      state.dpadUp = buttons[(int)JoystickButton::DPAD_UP];
    }
    if (count > (int)JoystickButton::DPAD_RIGHT) {
      state.dpadRight = buttons[(int)JoystickButton::DPAD_RIGHT];
    }
    if (count > (int)JoystickButton::DPAD_DOWN) {
      state.dpadDown = buttons[(int)JoystickButton::DPAD_DOWN];
    }
    if (count > (int)JoystickButton::DPAD_LEFT) {
      state.dpadLeft = buttons[(int)JoystickButton::DPAD_LEFT];
    }
  } else {
    GLFWgamepadstate gpState;
    if (glfwGetGamepadState((int)mJoystickId, &gpState)) {
      state.a = gpState.buttons[(int)JoystickButton::A];
      state.b = gpState.buttons[(int)JoystickButton::B];
      state.x = gpState.buttons[(int)JoystickButton::X];
      state.y = gpState.buttons[(int)JoystickButton::Y];
      state.cross = gpState.buttons[(int)JoystickButton::CROSS];
      state.circle = gpState.buttons[(int)JoystickButton::CIRCLE];
      state.square = gpState.buttons[(int)JoystickButton::SQUARE];
      state.triangle = gpState.buttons[(int)JoystickButton::TRIANGLE];
      state.leftBumper = gpState.buttons[(int)JoystickButton::LEFT_BUMPER];
      state.rightBumper = gpState.buttons[(int)JoystickButton::RIGHT_BUMPER];
      state.back = gpState.buttons[(int)JoystickButton::BACK];
      state.start = gpState.buttons[(int)JoystickButton::START];
      state.guide = gpState.buttons[(int)JoystickButton::GUIDE];
      state.leftThumb = gpState.buttons[(int)JoystickButton::LEFT_THUMB];
      state.rightThumb = gpState.buttons[(int)JoystickButton::RIGHT_THUMB];
      state.dpadUp = gpState.buttons[(int)JoystickButton::DPAD_UP];
      state.dpadRight = gpState.buttons[(int)JoystickButton::DPAD_RIGHT];
      state.dpadDown = gpState.buttons[(int)JoystickButton::DPAD_DOWN];
      state.dpadLeft = gpState.buttons[(int)JoystickButton::DPAD_LEFT];
    }
  }
  return state;
}

JoystickHatState Joystick::getHatState() const {
  JoystickHatState state;

  if (!this->isConnected()) {
    return state;
  }

  int count = 0;
  ubyte const *hats = glfwGetJoystickHats((int)mJoystickId, &count);

  if (count > (int)JoystickHat::UP) {
    state.up = hats[(int)JoystickHat::UP];
  }
  if (count > (int)JoystickHat::RIGHT) {
    state.right = hats[(int)JoystickHat::RIGHT];
  }
  if (count > (int)JoystickHat::DOWN) {
    state.down = hats[(int)JoystickHat::DOWN];
  }
  if (count > (int)JoystickHat::LEFT) {
    state.left = hats[(int)JoystickHat::LEFT];
  }
  return state;
}

Joystick &Joystick::operator=(Joystick const &joystick) {
  mJoystickId = joystick.mJoystickId;
  return *this;
}
} // namespace Terreate::OpenGL
