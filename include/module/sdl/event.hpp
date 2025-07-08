#pragma once

#include "../common/bitflag.hpp"
#include "../common/event.hpp"
#include "../common/type.hpp"

#include "gamepad.hpp"
#include "joystick.hpp"
#include "registry.hpp"
#include "window.hpp"

namespace Terreate::SDL {
struct Modifier {
public:
  bool lshift = false;
  bool rshift = false;
  bool level5 = false; // ?
  bool lctrl = false;
  bool rctrl = false;
  bool lalt = false;
  bool ralt = false;
  bool lgui = false;
  bool rgui = false;
  bool num = false;
  bool caps = false;
  bool mode = false;
  bool scroll = false;
  bool ctrl = false;
  bool shift = false;
  bool alt = false;
  bool gui = false;

public:
  Modifier(SDL_Keymod mods)
      : lshift(mods & SDL_KMOD_LSHIFT), rshift(mods & SDL_KMOD_RSHIFT),
        level5(mods & SDL_KMOD_LEVEL5), // ?
        lctrl(mods & SDL_KMOD_LCTRL), rctrl(mods & SDL_KMOD_RCTRL),
        lalt(mods & SDL_KMOD_LALT), ralt(mods & SDL_KMOD_RALT),
        lgui(mods & SDL_KMOD_LGUI), rgui(mods & SDL_KMOD_RGUI),
        num(mods & SDL_KMOD_NUM), caps(mods & SDL_KMOD_CAPS),
        mode(mods & SDL_KMOD_MODE), scroll(mods & SDL_KMOD_SCROLL),
        ctrl(mods & SDL_KMOD_CTRL), shift(mods & SDL_KMOD_SHIFT),
        alt(mods & SDL_KMOD_ALT), gui(mods & SDL_KMOD_GUI) {}
};

struct Key {
public:
  Keyboard key = Keyboard::K_UNKNOWN;
  i32 scancode = 0;
  bool pressed = 0;
  bool repeat = false;
  Modifier mods = 0;

public:
  Key(SDL_KeyboardEvent const &event)
      : key(static_cast<Keyboard>(event.scancode)), scancode(event.scancode),
        pressed(event.down), repeat(event.repeat), mods(event.mod) {}
};

template <typename... Args> using SDLEvent = Event<u64, Args...>;

typedef SDLEvent<> QuitEvent;

typedef SDLEvent<SDL_DisplayID, u32> DisplayOrientationEvent;
typedef SDLEvent<SDL_DisplayID> DisplayAddEvent;
typedef SDLEvent<SDL_DisplayID> DisplayRemoveEvent;
typedef SDLEvent<SDL_DisplayID, SDL_DisplayMode const *> DisplayModeChangeEvent;
typedef SDLEvent<SDL_DisplayID, SDL_DisplayMode const *>
    DisplayCurrentModeChangeEvent;
typedef SDLEvent<SDL_DisplayID, float> DisplayContentScaleChangeEvent;

typedef SDLEvent<shared<Window>, bool> WindowVisibilityChangeEvent;
typedef SDLEvent<shared<Window>, bool> WindowExposeEvent;
typedef SDLEvent<shared<Window>, i32, i32> WindowMoveEvent;
typedef SDLEvent<shared<Window>, i32, i32> WindowResizeEvent;
typedef SDLEvent<shared<Window>, i32, i32> WindowPixelSizeChangeEvent;
// typedef SDLEvent<shared<Window>> WindowMetalViewResizeEvent;
typedef SDLEvent<shared<Window>> WindowMinimizeEvent;
typedef SDLEvent<shared<Window>> WindowMaximizeEvent;
typedef SDLEvent<shared<Window>> WindowRestoreEvent;
typedef SDLEvent<shared<Window>> WindowMouseEnterEvent;
typedef SDLEvent<shared<Window>> WindowMouseLeaveEvent;
typedef SDLEvent<shared<Window>, bool> WindowKeyboardFocusChangeEvent;
typedef SDLEvent<shared<Window>> WindowCloseRequestedEvent;
typedef SDLEvent<shared<Window>, HitTestResult> WindowHitTestEvent;
// typedef SDLEvent<shared<Window>> WindowICCProfileChangeEvent;
typedef SDLEvent<shared<Window>, SDL_DisplayID> WindowDisplayChangeEvent;
typedef SDLEvent<shared<Window>, float> WindowDisplayScaleChangeEvent;
typedef SDLEvent<shared<Window>, u32, u32> WindowSafeAreaChangeEvent;
typedef SDLEvent<shared<Window>> WindowOccludeEvent;
typedef SDLEvent<shared<Window>, bool> WindowFullscreenChangeEvent;
typedef SDLEvent<shared<Window>> WindowDestroyEvent;
typedef SDLEvent<shared<Window>> WindowHDRStateChangeEvent;

typedef SDLEvent<Key> KeyEvent;

typedef SDLEvent<shared<Window>, str, i32> TextEditingEvent;
typedef SDLEvent<shared<Window>, str> TextInputEvent;

typedef SDLEvent<SDL_KeyboardID> KeyboardAddEvent;
typedef SDLEvent<SDL_KeyboardID> KeyboardRemoveEvent;

typedef SDLEvent<shared<Window>, vec<str>, u32, TextCandidateOrientation>
    TextEditingCandidatesEvent;

typedef SDLEvent<shared<Window>, SDL_MouseID, vec2, vec2> MouseMotionEvent;
typedef SDLEvent<shared<Window>, SDL_MouseID, MouseButton, bool, u8, vec2>
    MouseButtonEvent;
typedef SDLEvent<shared<Window>, SDL_MouseID, MouseWheelDirection, vec2, vec2,
                 pair<u32>>
    MouseWheelEvent;
typedef SDLEvent<SDL_MouseID> MouseAddEvent;
typedef SDLEvent<SDL_MouseID> MouseRemoveEvent;

typedef SDLEvent<shared<Joystick>, u8, i16> JoystickAxisEvent;
typedef SDLEvent<shared<Joystick>, u8, pair<i16>> JoystickBallEvent;
typedef SDLEvent<shared<Joystick>, u8, JoystickHatState> JoystickHatEvent;
typedef SDLEvent<shared<Joystick>, u8, bool> JoystickButtonEvent;
typedef SDLEvent<shared<Joystick>> JoystickAddEvent;
typedef SDLEvent<shared<Joystick>> JoystickRemoveEvent;
typedef SDLEvent<shared<Joystick>> JoystickUpdateCompleteEvent;
typedef SDLEvent<shared<Joystick>, PowerState, i32> JoystickBatteryUpdateEvent;

typedef SDLEvent<shared<Gamepad>, GamepadAxis, i16> GamepadAxisEvent;
typedef SDLEvent<shared<Gamepad>, GamepadButton, bool> GamepadButtonEvent;
typedef SDLEvent<shared<Gamepad>> GamepadAddEvent;
typedef SDLEvent<shared<Gamepad>> GamepadRemoveEvent;
typedef SDLEvent<shared<Gamepad>> GamepadRemappedEvent;
typedef SDLEvent<shared<Gamepad>> GamepadUpdateCompleteEvent;
typedef SDLEvent<shared<Gamepad>> GamepadStreamHandleUpdateEvent;
typedef SDLEvent<shared<Gamepad>, i32, i32, vec2, float> GamepadTouchpadEvent;
typedef SDLEvent<shared<Gamepad>, SDLSensor, vec3, u64> GamepadSensorEvent;

typedef SDLEvent<shared<Window>, SDL_TouchID, SDL_FingerID, vec2, vec2, float>
    TouchFingerEvent;

typedef SDLEvent<bool, vec<str>> ClipboardEvent;

typedef SDLEvent<shared<Window>, vec2, str, str> DropEvent;

typedef SDLEvent<SDL_AudioDeviceID, bool> AudioDeviceAddEvent;
typedef SDLEvent<SDL_AudioDeviceID, bool> AudioDeviceRemoveEvent;
typedef SDLEvent<SDL_AudioDeviceID, bool, AudioFormat, i32, i32, i32>
    AudioDeviceFormatChangeEvent;

typedef SDLEvent<SDL_SensorID, vec<float>> SensorEvent;

typedef SDLEvent<shared<Window>, SDL_PenID> PenProximityEvent;
typedef SDLEvent<shared<Window>, SDL_PenID, Bitflag<PenState>, vec2, bool, bool>
    PenTouchEvent;
typedef SDLEvent<shared<Window>, SDL_PenID, Bitflag<PenState>, vec2, u8, bool>
    PenButtonEvent;
typedef SDLEvent<shared<Window>, SDL_PenID, Bitflag<PenState>, vec2>
    PenMotionEvent;
typedef SDLEvent<shared<Window>, SDL_PenID, PenAxis, vec2, float> PenAxisEvent;

typedef SDLEvent<SDL_CameraID> CameraAddEvent;
typedef SDLEvent<SDL_CameraID> CameraRemoveEvent;
typedef SDLEvent<SDL_CameraID> CameraApprovedEvent;
typedef SDLEvent<SDL_CameraID> CameraDeniedEvent;

typedef SDLEvent<shared<Window>, u32, pair<void *>> UserEvent;

class EventHandler {
private:
  shared<SDLObjectRegistry> mRegistry = nullptr;

public:
  QuitEvent onQuit;

  DisplayOrientationEvent onOrientation;
  DisplayAddEvent onDisplayAdd;
  DisplayRemoveEvent onDisplayRemove;
  DisplayModeChangeEvent onDisplayModeChange;
  DisplayCurrentModeChangeEvent onDisplayCurrentModeChange;
  DisplayContentScaleChangeEvent onDisplayContentScaleChange;

  WindowVisibilityChangeEvent onWindowVisibilityChange;
  WindowExposeEvent onWindowExpose;
  WindowMoveEvent onWindowMove;
  WindowResizeEvent onWindowResize;
  WindowPixelSizeChangeEvent onWindowPixelSizeChange;
  // WindowMetalViewResizeEvent onWindowMetalViewResize;
  WindowMinimizeEvent onWindowMinimize;
  WindowMaximizeEvent onWindowMaximize;
  WindowRestoreEvent onWindowRestore;
  WindowMouseEnterEvent onWindowMouseEnter;
  WindowMouseLeaveEvent onWindowMouseLeave;
  WindowKeyboardFocusChangeEvent onWindowKeyboardFocusChange;
  WindowCloseRequestedEvent onWindowCloseRequested;
  WindowHitTestEvent onWindowHitTest;
  // WindowICCProfileChangeEvent onWindowICCProfileChange;
  WindowDisplayChangeEvent onWindowDisplayChange;
  WindowDisplayScaleChangeEvent onWindowDisplayScaleChange;
  WindowSafeAreaChangeEvent onWindowSafeAreaChange;
  WindowOccludeEvent onWindowOcclude;
  WindowFullscreenChangeEvent onWindowFullscreenChange;
  WindowDestroyEvent onWindowDestroy;
  WindowHDRStateChangeEvent onWindowHDRStateChange;

  KeyEvent onKey;

  TextEditingEvent onTextEditing;
  TextInputEvent onTextInput;

  KeyboardAddEvent onKeyboardAdd;
  KeyboardRemoveEvent onKeyboardRemove;

  TextEditingCandidatesEvent onTextEditingCandidates;

  MouseMotionEvent onMouseMotion;
  MouseButtonEvent onMouseButton;
  MouseWheelEvent onMouseWheel;
  MouseAddEvent onMouseAdd;
  MouseRemoveEvent onMouseRemove;

  JoystickAxisEvent onJoystickAxisMotion;
  JoystickBallEvent onJoystickBallMotion;
  JoystickHatEvent onJoystickHatMotion;
  JoystickButtonEvent onJoystickButton;
  JoystickAddEvent onJoystickAdd;
  JoystickRemoveEvent onJoystickRemove;
  JoystickUpdateCompleteEvent onJoystickUpdateComplete;
  JoystickBatteryUpdateEvent onJoystickBatteryUpdate;

  GamepadAxisEvent onGamepadAxisMotion;
  GamepadButtonEvent onGamepadButton;
  GamepadAddEvent onGamepadAdd;
  GamepadRemoveEvent onGamepadRemove;
  GamepadRemappedEvent onGamepadRemapped;
  GamepadUpdateCompleteEvent onGamepadUpdateComplete;
  GamepadStreamHandleUpdateEvent onGamepadStreamHandleUpdate;
  GamepadTouchpadEvent onGamepadTouchpadEvent;
  GamepadSensorEvent onGamepadSensorEvent;

  TouchFingerEvent onTouchFinger;

  ClipboardEvent onClipboard;

  DropEvent onDrop;

  AudioDeviceAddEvent onAudioDeviceAdd;
  AudioDeviceRemoveEvent onAudioDeviceRemove;
  AudioDeviceFormatChangeEvent onAudioDeviceFormatChange;

  SensorEvent onSensor;

  PenProximityEvent onPenProximity;
  PenTouchEvent onPenTouch;
  PenButtonEvent onPenButton;
  PenMotionEvent onPenMotion;
  PenAxisEvent onPenAxis;

  CameraAddEvent onCameraAdd;
  CameraRemoveEvent onCameraRemove;
  CameraApprovedEvent onCameraApproved;
  CameraDeniedEvent onCameraDenied;

  UserEvent onUser;

private:
  void handle(SDL_QuitEvent const &event) {
    this->onQuit.publish(event.timestamp);
  }
  void handle(SDL_CommonEvent const &event);
  void handle(SDL_DisplayEvent const &event);
  void handle(SDL_WindowEvent const &event);
  void handle(SDL_KeyboardEvent const &event);
  void handle(SDL_TextEditingEvent const &event);
  void handle(SDL_TextInputEvent const &event);
  void handle(SDL_KeyboardDeviceEvent const &event);
  void handle(SDL_TextEditingCandidatesEvent const &event);
  void handle(SDL_MouseMotionEvent const &event);
  void handle(SDL_MouseButtonEvent const &event);
  void handle(SDL_MouseWheelEvent const &event);
  void handle(SDL_MouseDeviceEvent const &event);
  void handle(SDL_JoyAxisEvent const &event);
  void handle(SDL_JoyBallEvent const &event);
  void handle(SDL_JoyHatEvent const &event);
  void handle(SDL_JoyButtonEvent const &event);
  void handle(SDL_JoyDeviceEvent const &event);
  void handle(SDL_JoyBatteryEvent const &event);
  void handle(SDL_GamepadAxisEvent const &event);
  void handle(SDL_GamepadButtonEvent const &event);
  void handle(SDL_GamepadDeviceEvent const &event);
  void handle(SDL_GamepadTouchpadEvent const &event);
  void handle(SDL_GamepadSensorEvent const &event);
  void handle(SDL_TouchFingerEvent const &event);
  void handle(SDL_ClipboardEvent const &event);
  void handle(SDL_DropEvent const &event);
  void handle(SDL_AudioDeviceEvent const &event);
  void handle(SDL_SensorEvent const &event) {
    this->onSensor.publish(event.timestamp, event.which,
                           {event.data[0], event.data[1], event.data[2],
                            event.data[3], event.data[4], event.data[5]});
  }
  void handle(SDL_PenProximityEvent const &event);
  void handle(SDL_PenTouchEvent const &event);
  void handle(SDL_PenButtonEvent const &event);
  void handle(SDL_PenMotionEvent const &event);
  void handle(SDL_PenAxisEvent const &event);
  void handle(SDL_CameraDeviceEvent const &event);
  void handle(SDL_UserEvent const &event);

public:
  EventHandler(shared<SDLObjectRegistry> const &registry)
      : mRegistry(registry) {}
  ~EventHandler() = default;

  void poll();
};
} // namespace Terreate::SDL
