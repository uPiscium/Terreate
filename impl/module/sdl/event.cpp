#include "../../../include/module/sdl/event.hpp"
#include "../../../include/module/sdl/exception.hpp"

namespace Terreate::SDL {
void EventHandler::handle(SDL_CommonEvent const &event) {
  // Common events are handled here, but they are not used in this module.
  // This is a placeholder for future common event handling.
}

void EventHandler::handle(SDL_DisplayEvent const &event) {
  switch (event.type) {
  case SDL_EVENT_DISPLAY_ORIENTATION:
    this->onOrientation.publish(event.timestamp, event.displayID, event.data1);
    break;
  case SDL_EVENT_DISPLAY_ADDED:
    this->onDisplayAdd.publish(event.timestamp, event.displayID);
    break;
  case SDL_EVENT_DISPLAY_REMOVED:
    this->onDisplayRemove.publish(event.timestamp, event.displayID);
    break;
  case SDL_EVENT_DISPLAY_DESKTOP_MODE_CHANGED:
    this->onDisplayModeChange.publish(
        event.timestamp, event.displayID,
        SDL_GetDesktopDisplayMode(event.displayID));
    break;
  case SDL_EVENT_DISPLAY_CURRENT_MODE_CHANGED:
    this->onDisplayCurrentModeChange.publish(
        event.timestamp, event.displayID,
        SDL_GetDesktopDisplayMode(event.displayID));
    break;
  case SDL_EVENT_DISPLAY_CONTENT_SCALE_CHANGED:
    this->onDisplayContentScaleChange.publish(
        event.timestamp, event.displayID,
        SDL_GetDisplayContentScale(event.displayID));
    break;
  default:
    throw Exception::SDLModuleError(
        "Unhandled SDL_DisplayEvent type: " +
        std::to_string(static_cast<int>(event.type)));
    break;
  }
}

void EventHandler::handle(SDL_WindowEvent const &event) {
  shared<Window> window = mRegistry->getWindow(event.windowID);
  switch (event.type) {
  case SDL_EVENT_WINDOW_SHOWN:
    this->onWindowVisibilityChange.publish(event.timestamp, window, true);
    break;
  case SDL_EVENT_WINDOW_HIDDEN:
    this->onWindowVisibilityChange.publish(event.timestamp, window, false);
    break;
  case SDL_EVENT_WINDOW_EXPOSED:
    this->onWindowExpose.publish(event.timestamp, window, event.data1 != 0);
    break;
  case SDL_EVENT_WINDOW_MOVED:
    this->onWindowMove.publish(event.timestamp, window, event.data1,
                               event.data2);
    break;
  case SDL_EVENT_WINDOW_RESIZED:
    this->onWindowResize.publish(event.timestamp, window, event.data1,
                                 event.data2);
    break;
  case SDL_EVENT_WINDOW_PIXEL_SIZE_CHANGED:
    this->onWindowPixelSizeChange.publish(event.timestamp, window, event.data1,
                                          event.data2);
    break;
  case SDL_EVENT_WINDOW_METAL_VIEW_RESIZED:
    // This event is not supported yet.
    break;
  case SDL_EVENT_WINDOW_MINIMIZED:
    this->onWindowMinimize.publish(event.timestamp, window);
    break;
  case SDL_EVENT_WINDOW_MAXIMIZED:
    this->onWindowMaximize.publish(event.timestamp, window);
    break;
  case SDL_EVENT_WINDOW_RESTORED:
    this->onWindowRestore.publish(event.timestamp, window);
    break;
  case SDL_EVENT_WINDOW_MOUSE_ENTER:
    this->onWindowMouseEnter.publish(event.timestamp, window);
    break;
  case SDL_EVENT_WINDOW_MOUSE_LEAVE:
    this->onWindowMouseLeave.publish(event.timestamp, window);
    break;
  case SDL_EVENT_WINDOW_FOCUS_GAINED:
    this->onWindowKeyboardFocusChange.publish(event.timestamp, window, true);
    break;
  case SDL_EVENT_WINDOW_FOCUS_LOST:
    this->onWindowKeyboardFocusChange.publish(event.timestamp, window, false);
    break;
  case SDL_EVENT_WINDOW_CLOSE_REQUESTED:
    this->onWindowCloseRequested.publish(event.timestamp, window);
    break;
  case SDL_EVENT_WINDOW_HIT_TEST:
    this->onWindowHitTest.publish(event.timestamp, window,
                                  static_cast<HitTestResult>(event.data1));
    break;
  case SDL_EVENT_WINDOW_ICCPROF_CHANGED:
    // This event is not supported yet.
    break;
  case SDL_EVENT_WINDOW_DISPLAY_CHANGED:
    this->onWindowDisplayChange.publish(event.timestamp, window, event.data1);
    break;
  case SDL_EVENT_WINDOW_DISPLAY_SCALE_CHANGED:
    this->onWindowDisplayScaleChange.publish(event.timestamp, window,
                                             static_cast<float>(event.data1));
    break;
  case SDL_EVENT_WINDOW_SAFE_AREA_CHANGED:
    this->onWindowSafeAreaChange.publish(event.timestamp, window, event.data1,
                                         event.data2);
    break;
  case SDL_EVENT_WINDOW_OCCLUDED:
    this->onWindowOcclude.publish(event.timestamp, window);
    break;
  case SDL_EVENT_WINDOW_ENTER_FULLSCREEN:
    this->onWindowFullscreenChange.publish(event.timestamp, window, true);
    break;
  case SDL_EVENT_WINDOW_LEAVE_FULLSCREEN:
    this->onWindowFullscreenChange.publish(event.timestamp, window, false);
    break;
  case SDL_EVENT_WINDOW_DESTROYED:
    this->onWindowDestroy.publish(event.timestamp, window);
    break;
  case SDL_EVENT_WINDOW_HDR_STATE_CHANGED:
    this->onWindowHDRStateChange.publish(event.timestamp, window);
    break;
  default:
    throw Exception::SDLModuleError(
        "Unhandled SDL_WindowEvent type: " +
        std::to_string(static_cast<int>(event.type)));
    break;
  }
}

void EventHandler::handle(SDL_KeyboardEvent const &event) {
  Key key = Key(event);
  this->onKey.publish(event.timestamp, key);
}

void EventHandler::handle(SDL_TextEditingEvent const &event) {
  str text(event.text, event.length);
  shared<Window> window = mRegistry->getWindow(event.windowID);
  this->onTextEditing.publish(event.timestamp, window, text, event.start);
}

void EventHandler::handle(SDL_TextInputEvent const &event) {
  str text(event.text);
  shared<Window> window = mRegistry->getWindow(event.windowID);
  this->onTextInput.publish(event.timestamp, window, text);
}

void EventHandler::handle(SDL_KeyboardDeviceEvent const &event) {
  switch (event.type) {
  case SDL_EVENT_KEYBOARD_ADDED:
    this->onKeyboardAdd.publish(event.timestamp, event.which);
    break;
  case SDL_EVENT_KEYBOARD_REMOVED:
    this->onKeyboardRemove.publish(event.timestamp, event.which);
    break;
  default:
    throw Exception::SDLModuleError(
        "Unhandled SDL_KeyboardDeviceEvent type: " +
        std::to_string(static_cast<int>(event.type)));
    break;
  }
}

void EventHandler::handle(SDL_TextEditingCandidatesEvent const &event) {
  shared<Window> window = mRegistry->getWindow(event.windowID);
  vec<str> candidates(event.candidates,
                      event.candidates + event.num_candidates);
  TextCandidateOrientation orientation =
      event.horizontal ? TextCandidateOrientation::HORIZONTAL
                       : TextCandidateOrientation::VERTICAL;
  this->onTextEditingCandidates.publish(event.timestamp, window, candidates,
                                        event.selected_candidate, orientation);
}

void EventHandler::handle(SDL_MouseMotionEvent const &event) {
  shared<Window> window = mRegistry->getWindow(event.windowID);
  shared<Mouse> mouse;
  if (mRegistry->hasMouse(event.which)) {
    mouse = mRegistry->getMouse(event.which);
  } else {
    mouse = std::make_shared<Mouse>(event.which);
    mRegistry->registerMouse(event.which, mouse);
  }
  this->onMouseMotion.publish(event.timestamp, window, mouse,
                              {event.x, event.y}, {event.xrel, event.yrel});
}

void EventHandler::handle(SDL_MouseButtonEvent const &event) {
  shared<Window> window = mRegistry->getWindow(event.windowID);
  shared<Mouse> mouse;
  if (mRegistry->hasMouse(event.which)) {
    mouse = mRegistry->getMouse(event.which);
  } else {
    mouse = std::make_shared<Mouse>(event.which);
    mRegistry->registerMouse(event.which, mouse);
  }
  this->onMouseButton.publish(event.timestamp, window, mouse,
                              (MouseButton)event.button, event.down,
                              event.clicks, {event.x, event.y});
}

void EventHandler::handle(SDL_MouseWheelEvent const &event) {
  shared<Window> window = mRegistry->getWindow(event.windowID);
  shared<Mouse> mouse;
  if (mRegistry->hasMouse(event.which)) {
    mouse = mRegistry->getMouse(event.which);
  } else {
    mouse = std::make_shared<Mouse>(event.which);
    mRegistry->registerMouse(event.which, mouse);
  }
  this->onMouseWheel.publish(event.timestamp, window, mouse,
                             (MouseWheelDirection)event.direction,
                             {event.x, event.y}, {event.mouse_x, event.mouse_y},
                             {event.integer_x, event.integer_y});
}

void EventHandler::handle(SDL_MouseDeviceEvent const &event) {
  switch (event.type) {
  case SDL_EVENT_MOUSE_ADDED:
    if (!mRegistry->hasMouse(event.which)) {
      mRegistry->registerMouse(event.which,
                               std::make_shared<Mouse>(event.which));
    }
    this->onMouseAdd.publish(event.timestamp, mRegistry->getMouse(event.which));
    break;
  case SDL_EVENT_MOUSE_REMOVED:
    this->onMouseRemove.publish(event.timestamp,
                                mRegistry->getMouse(event.which));
    if (mRegistry->hasMouse(event.which)) {
      mRegistry->unregisterMouse(event.which);
    }
    break;
  default:
    throw Exception::SDLModuleError(
        "Unhandled SDL_MouseDeviceEvent type: " +
        std::to_string(static_cast<int>(event.type)));
    break;
  }
}

void EventHandler::handle(SDL_JoyAxisEvent const &event) {
  shared<Joystick> joystick = mRegistry->getJoystick(event.which);
  this->onJoystickAxisMotion.publish(event.timestamp, joystick, event.axis,
                                     event.value);
}

void EventHandler::handle(SDL_JoyBallEvent const &event) {
  shared<Joystick> joystick = mRegistry->getJoystick(event.which);
  this->onJoystickBallMotion.publish(event.timestamp, joystick, event.ball,
                                     {event.xrel, event.yrel});
}

void EventHandler::handle(SDL_JoyHatEvent const &event) {
  shared<Joystick> joystick = mRegistry->getJoystick(event.which);
  this->onJoystickHatMotion.publish(event.timestamp, joystick, event.hat,
                                    (JoystickHatState)event.value);
}

void EventHandler::handle(SDL_JoyButtonEvent const &event) {
  shared<Joystick> joystick = mRegistry->getJoystick(event.which);
  this->onJoystickButton.publish(event.timestamp, joystick, event.button,
                                 event.down);
}

void EventHandler::handle(SDL_JoyDeviceEvent const &event) {
  switch (event.type) {
  case SDL_EVENT_JOYSTICK_ADDED:
    if (!mRegistry->hasJoystick(event.which)) {
      mRegistry->registerJoystick(event.which,
                                  std::make_shared<Joystick>(event.which));
    }
    this->onJoystickAdd.publish(event.timestamp,
                                mRegistry->getJoystick(event.which));
    break;
  case SDL_EVENT_JOYSTICK_REMOVED:
    this->onJoystickRemove.publish(event.timestamp,
                                   mRegistry->getJoystick(event.which));
    if (mRegistry->hasJoystick(event.which)) {
      mRegistry->unregisterJoystick(event.which);
    }
    break;
  case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE:
    this->onJoystickUpdateComplete.publish(event.timestamp,
                                           mRegistry->getJoystick(event.which));
    break;
  default:
    throw Exception::SDLModuleError(
        "Unhandled SDL_JoyDeviceEvent type: " +
        std::to_string(static_cast<int>(event.type)));
    break;
  }
}

void EventHandler::handle(SDL_JoyBatteryEvent const &event) {
  shared<Joystick> joystick = mRegistry->getJoystick(event.which);
  if (joystick) {
    this->onJoystickBatteryUpdate.publish(
        event.timestamp, joystick, (PowerState)event.state, event.percent);
  } else {
    throw Exception::SDLModuleError(
        "Joystick not found for SDL_JoyBatteryEvent with which: " +
        std::to_string(event.which));
  }
}

void EventHandler::handle(SDL_GamepadAxisEvent const &event) {
  shared<Gamepad> gamepad = mRegistry->getGamepad(event.which);
  this->onGamepadAxisMotion.publish(event.timestamp, gamepad,
                                    (GamepadAxis)event.axis, event.value);
}

void EventHandler::handle(SDL_GamepadButtonEvent const &event) {
  shared<Gamepad> gamepad = mRegistry->getGamepad(event.which);
  this->onGamepadButton.publish(event.timestamp, gamepad,
                                (GamepadButton)event.button, event.down);
}

void EventHandler::handle(SDL_GamepadDeviceEvent const &event) {
  switch (event.type) {
  case SDL_EVENT_GAMEPAD_ADDED:
    if (!mRegistry->hasGamepad(event.which)) {
      mRegistry->registerGamepad(event.which,
                                 std::make_shared<Gamepad>(event.which));
    }
    this->onGamepadAdd.publish(event.timestamp,
                               mRegistry->getGamepad(event.which));
    break;
  case SDL_EVENT_GAMEPAD_REMOVED:
    this->onGamepadRemove.publish(event.timestamp,
                                  mRegistry->getGamepad(event.which));
    if (mRegistry->hasGamepad(event.which)) {
      mRegistry->unregisterGamepad(event.which);
    }
    break;
  case SDL_EVENT_GAMEPAD_REMAPPED:
    this->onGamepadRemapped.publish(event.timestamp,
                                    mRegistry->getGamepad(event.which));
    break;
  case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
    this->onGamepadStreamHandleUpdate.publish(
        event.timestamp, mRegistry->getGamepad(event.which));
    break;
  case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
    this->onGamepadUpdateComplete.publish(event.timestamp,
                                          mRegistry->getGamepad(event.which));
    break;
  default:
    throw Exception::SDLModuleError(
        "Unhandled SDL_JoyDeviceEvent type: " +
        std::to_string(static_cast<int>(event.type)));
    break;
  }
}

void EventHandler::handle(SDL_GamepadTouchpadEvent const &event) {
  shared<Gamepad> gamepad = mRegistry->getGamepad(event.which);
  this->onGamepadTouchpadEvent.publish(event.timestamp, gamepad, event.touchpad,
                                       event.finger, {event.x, event.y},
                                       event.pressure);
}

void EventHandler::handle(SDL_GamepadSensorEvent const &event) {
  shared<Gamepad> gamepad = mRegistry->getGamepad(event.which);
  this->onGamepadSensorEvent.publish(
      event.timestamp, gamepad, (SDLSensor)event.sensor,
      {event.data[0], event.data[1], event.data[2]}, event.timestamp);
}

void EventHandler::handle(SDL_TouchFingerEvent const &event) {
  shared<Window> window = mRegistry->getWindow(event.windowID);
  this->onTouchFinger.publish(event.timestamp, window, event.touchID,
                              event.fingerID, {event.x, event.y},
                              {event.dx, event.dy}, event.pressure);
}

void EventHandler::handle(SDL_ClipboardEvent const &event) {
  vec<str> mimeTypes(event.mime_types, event.mime_types + event.num_mime_types);
  this->onClipboard.publish(event.timestamp, event.owner, mimeTypes);
}

void EventHandler::handle(SDL_DropEvent const &event) {
  shared<Window> window = mRegistry->getWindow(event.windowID);
  str filePath(event.source);
  str text(event.data);
  this->onDrop.publish(event.timestamp, window, {event.x, event.y}, filePath,
                       text);
}

void EventHandler::handle(SDL_AudioDeviceEvent const &event) {
  SDL_AudioSpec spec;
  i32 sampleRate = 0;
  SDL_GetAudioDeviceFormat(event.which, &spec, &sampleRate);
  switch (event.type) {
  case SDL_EVENT_AUDIO_DEVICE_ADDED:
    if (!mRegistry->hasAudioDevice(event.which)) {
      mRegistry->registerAudioDevice(
          event.which, std::make_shared<AudioDevice>(event.which));
    }
    this->onAudioDeviceAdd.publish(event.timestamp,
                                   mRegistry->getAudioDevice(event.which),
                                   event.recording);
    break;
  case SDL_EVENT_AUDIO_DEVICE_REMOVED:
    this->onAudioDeviceRemove.publish(event.timestamp,
                                      mRegistry->getAudioDevice(event.which),
                                      event.recording);
    if (mRegistry->hasAudioDevice(event.which)) {
      mRegistry->unregisterAudioDevice(event.which);
    }
    break;
  case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED:
    this->onAudioDeviceFormatChange.publish(
        event.timestamp, mRegistry->getAudioDevice(event.which),
        event.recording, (AudioFormat)spec.format, spec.channels, spec.freq,
        sampleRate);
    break;
  default:
    throw Exception::SDLModuleError(
        "Unhandled SDL_AudioDeviceEvent type: " +
        std::to_string(static_cast<int>(event.type)));
    break;
  }
}

void EventHandler::handle(SDL_PenProximityEvent const &event) {
  shared<Window> window = mRegistry->getWindow(event.windowID);
  this->onPenProximity.publish(event.timestamp, window, event.which);
}

void EventHandler::handle(SDL_PenTouchEvent const &event) {
  shared<Window> window = mRegistry->getWindow(event.windowID);
  this->onPenTouch.publish(event.timestamp, window, event.which,
                           (Bitflag<PenState>)event.pen_state,
                           {event.x, event.y}, event.eraser, event.down);
}

void EventHandler::handle(SDL_PenButtonEvent const &event) {
  shared<Window> window = mRegistry->getWindow(event.windowID);
  this->onPenButton.publish(event.timestamp, window, event.which,
                            (Bitflag<PenState>)event.pen_state,
                            {event.x, event.y}, event.button, event.down);
}

void EventHandler::handle(SDL_PenMotionEvent const &event) {
  shared<Window> window = mRegistry->getWindow(event.windowID);
  this->onPenMotion.publish(event.timestamp, window, event.which,
                            (Bitflag<PenState>)event.pen_state,
                            {event.x, event.y});
}

void EventHandler::handle(SDL_PenAxisEvent const &event) {
  shared<Window> window = mRegistry->getWindow(event.windowID);
  this->onPenAxis.publish(event.timestamp, window, event.which,
                          (PenAxis)event.axis, {event.x, event.y}, event.value);
}

void EventHandler::handle(SDL_CameraDeviceEvent const &event) {
  switch (event.type) {
  case SDL_EVENT_CAMERA_DEVICE_ADDED:
    if (!mRegistry->hasCamera(event.which)) {
      mRegistry->registerCamera(event.which,
                                std::make_shared<Camera>(event.which));
    }
    this->onCameraAdd.publish(event.timestamp,
                              mRegistry->getCamera(event.which));
    break;
  case SDL_EVENT_CAMERA_DEVICE_REMOVED:
    this->onCameraRemove.publish(event.timestamp,
                                 mRegistry->getCamera(event.which));
    if (mRegistry->hasCamera(event.which)) {
      mRegistry->unregisterCamera(event.which);
    }
    break;
  case SDL_EVENT_CAMERA_DEVICE_APPROVED:
    this->onCameraApproved.publish(event.timestamp,
                                   mRegistry->getCamera(event.which));
    break;
  case SDL_EVENT_CAMERA_DEVICE_DENIED:
    this->onCameraDenied.publish(event.timestamp,
                                 mRegistry->getCamera(event.which));
    break;
  default:
    throw Exception::SDLModuleError(
        "Unhandled SDL_CameraDeviceEvent type: " +
        std::to_string(static_cast<int>(event.type)));
    break;
  }
}

void EventHandler::handle(SDL_UserEvent const &event) {
  shared<Window> window = mRegistry->getWindow(event.windowID);
  this->onUser.publish(event.timestamp, window, event.type,
                       {event.data1, event.data2});
}

void EventHandler::poll() {
  SDL_Event handle;
  while (SDL_PollEvent(&handle)) {
    switch (handle.type) {
    case SDL_EVENT_QUIT:
      this->handle(handle.quit);
      break;

    case SDL_EVENT_TERMINATING:
    case SDL_EVENT_LOW_MEMORY:
    case SDL_EVENT_WILL_ENTER_BACKGROUND:
    case SDL_EVENT_DID_ENTER_BACKGROUND:
    case SDL_EVENT_LOCALE_CHANGED:
    case SDL_EVENT_SYSTEM_THEME_CHANGED:
    case SDL_EVENT_KEYMAP_CHANGED:
    case SDL_EVENT_PRIVATE0:
    case SDL_EVENT_PRIVATE1:
    case SDL_EVENT_PRIVATE2:
    case SDL_EVENT_PRIVATE3:
    case SDL_EVENT_POLL_SENTINEL:
    case SDL_EVENT_RENDER_TARGETS_RESET:
    case SDL_EVENT_RENDER_DEVICE_RESET:
    case SDL_EVENT_RENDER_DEVICE_LOST:
      this->handle(handle.common);
      break;

    case SDL_EVENT_DISPLAY_FIRST ... SDL_EVENT_DISPLAY_LAST:
      this->handle(handle.display);
      break;

    case SDL_EVENT_WINDOW_FIRST ... SDL_EVENT_WINDOW_LAST:
      this->handle(handle.window);
      break;

    case SDL_EVENT_KEY_DOWN:
    case SDL_EVENT_KEY_UP:
      this->handle(handle.key);
      break;

    case SDL_EVENT_TEXT_EDITING:
      this->handle(handle.edit);
      break;
    case SDL_EVENT_TEXT_INPUT:
      this->handle(handle.text);
      break;
    case SDL_EVENT_KEYBOARD_ADDED:
    case SDL_EVENT_KEYBOARD_REMOVED:
      this->handle(handle.kdevice);
      break;
    case SDL_EVENT_TEXT_EDITING_CANDIDATES:
      this->handle(handle.edit_candidates);
      break;

    case SDL_EVENT_MOUSE_MOTION:
      this->handle(handle.motion);
      break;
    case SDL_EVENT_MOUSE_BUTTON_DOWN:
    case SDL_EVENT_MOUSE_BUTTON_UP:
      this->handle(handle.button);
      break;
    case SDL_EVENT_MOUSE_WHEEL:
      this->handle(handle.wheel);
      break;
    case SDL_EVENT_MOUSE_ADDED:
    case SDL_EVENT_MOUSE_REMOVED:
      this->handle(handle.mdevice);
      break;

    case SDL_EVENT_JOYSTICK_AXIS_MOTION:
      this->handle(handle.jaxis);
      break;
    case SDL_EVENT_JOYSTICK_BALL_MOTION:
      this->handle(handle.jball);
      break;
    case SDL_EVENT_JOYSTICK_HAT_MOTION:
      this->handle(handle.jhat);
      break;
    case SDL_EVENT_JOYSTICK_BUTTON_DOWN:
    case SDL_EVENT_JOYSTICK_BUTTON_UP:
      this->handle(handle.jbutton);
      break;
    case SDL_EVENT_JOYSTICK_ADDED:
    case SDL_EVENT_JOYSTICK_REMOVED:
    case SDL_EVENT_JOYSTICK_UPDATE_COMPLETE:
      this->handle(handle.jdevice);
      break;
    case SDL_EVENT_JOYSTICK_BATTERY_UPDATED:
      this->handle(handle.jbattery);
      break;

    case SDL_EVENT_GAMEPAD_AXIS_MOTION:
      this->handle(handle.gaxis);
      break;
    case SDL_EVENT_GAMEPAD_BUTTON_DOWN:
    case SDL_EVENT_GAMEPAD_BUTTON_UP:
      this->handle(handle.gbutton);
      break;
    case SDL_EVENT_GAMEPAD_ADDED:
    case SDL_EVENT_GAMEPAD_REMOVED:
    case SDL_EVENT_GAMEPAD_REMAPPED:
    case SDL_EVENT_GAMEPAD_UPDATE_COMPLETE:
    case SDL_EVENT_GAMEPAD_STEAM_HANDLE_UPDATED:
      this->handle(handle.gdevice);
      break;
    case SDL_EVENT_GAMEPAD_TOUCHPAD_DOWN:
    case SDL_EVENT_GAMEPAD_TOUCHPAD_MOTION:
    case SDL_EVENT_GAMEPAD_TOUCHPAD_UP:
      this->handle(handle.gtouchpad);
      break;
    case SDL_EVENT_GAMEPAD_SENSOR_UPDATE:
      this->handle(handle.gsensor);
      break;

    case SDL_EVENT_FINGER_DOWN:
    case SDL_EVENT_FINGER_UP:
    case SDL_EVENT_FINGER_MOTION:
    case SDL_EVENT_FINGER_CANCELED:
      this->handle(handle.tfinger);
      break;

    case SDL_EVENT_CLIPBOARD_UPDATE:
      this->handle(handle.clipboard);
      break;

    case SDL_EVENT_DROP_FILE:
    case SDL_EVENT_DROP_TEXT:
    case SDL_EVENT_DROP_BEGIN:
    case SDL_EVENT_DROP_COMPLETE:
    case SDL_EVENT_DROP_POSITION:
      this->handle(handle.drop);
      break;

    case SDL_EVENT_AUDIO_DEVICE_ADDED:
    case SDL_EVENT_AUDIO_DEVICE_REMOVED:
    case SDL_EVENT_AUDIO_DEVICE_FORMAT_CHANGED:
      this->handle(handle.adevice);
      break;

    case SDL_EVENT_SENSOR_UPDATE:
      this->handle(handle.sensor);
      break;

    case SDL_EVENT_PEN_PROXIMITY_IN:
    case SDL_EVENT_PEN_PROXIMITY_OUT:
      this->handle(handle.pproximity);
      break;
    case SDL_EVENT_PEN_DOWN:
    case SDL_EVENT_PEN_UP:
      this->handle(handle.ptouch);
      break;
    case SDL_EVENT_PEN_BUTTON_DOWN:
    case SDL_EVENT_PEN_BUTTON_UP:
      this->handle(handle.pbutton);
      break;
    case SDL_EVENT_PEN_MOTION:
      this->handle(handle.pmotion);
      break;
    case SDL_EVENT_PEN_AXIS:
      this->handle(handle.paxis);
      break;

    case SDL_EVENT_CAMERA_DEVICE_ADDED:
    case SDL_EVENT_CAMERA_DEVICE_REMOVED:
    case SDL_EVENT_CAMERA_DEVICE_APPROVED:
    case SDL_EVENT_CAMERA_DEVICE_DENIED:
      this->handle(handle.cdevice);
      break;

    case SDL_EVENT_USER:
      this->handle(handle.user);
      break;

      // case SDL_EVENT_LAST:
      //   break;

    default:
      break;
    }
  }
}

} // namespace Terreate::SDL
