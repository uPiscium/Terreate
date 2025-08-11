#pragma once

#include "../common/bitflag.hpp"
#include "../common/event.hpp"
#include "../common/type.hpp"

#include "camera.hpp"
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

typedef SDLEvent<> TerminateEvent;
typedef SDLEvent<> LowMemoryEvent;
typedef SDLEvent<> WillEnterBackgroundEvent;
typedef SDLEvent<> DidEnterBackgroundEvent;
typedef SDLEvent<> LocaleChangeEvent;
typedef SDLEvent<> SystemThemeChangeEvent;
typedef SDLEvent<> PrivateEvent;
typedef SDLEvent<> PollStencilEvent;
typedef SDLEvent<> RenderTargetResetEvent;
typedef SDLEvent<> RenderDeviceResetEvent;
typedef SDLEvent<> RenderDeviceLostEvent;

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

typedef SDLEvent<shared<Window>, shared<Mouse>, vec2, vec2> MouseMotionEvent;
typedef SDLEvent<shared<Window>, shared<Mouse>, MouseButton, bool, u8, vec2>
    MouseButtonEvent;
typedef SDLEvent<shared<Window>, shared<Mouse>, MouseWheelDirection, vec2, vec2,
                 pair<u32>>
    MouseWheelEvent;
typedef SDLEvent<shared<Mouse>> MouseAddEvent;
typedef SDLEvent<shared<Mouse>> MouseRemoveEvent;

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

typedef SDLEvent<shared<AudioDevice>, bool> AudioDeviceAddEvent;
typedef SDLEvent<shared<AudioDevice>, bool> AudioDeviceRemoveEvent;
typedef SDLEvent<shared<AudioDevice>, bool, AudioFormat, i32, i32, i32>
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

typedef SDLEvent<shared<Camera>> CameraAddEvent;
typedef SDLEvent<shared<Camera>> CameraRemoveEvent;
typedef SDLEvent<shared<Camera>> CameraApprovedEvent;
typedef SDLEvent<shared<Camera>> CameraDeniedEvent;

typedef SDLEvent<shared<Window>, u32, pair<void *>> UserEvent;

class IEventHandler {
public:
  virtual ~IEventHandler() = default;
  virtual void inject(shared<ObjectRegistry> const &registry) = 0;
};

class CommonEventHandler : public IEventHandler {
private:
  shared<ObjectRegistry> mRegistry = nullptr;

public:
  TerminateEvent onTerminate;
  LowMemoryEvent onLowMemory;
  WillEnterBackgroundEvent onWillEnterBackground;
  DidEnterBackgroundEvent onDidEnterBackground;
  LocaleChangeEvent onLocaleChange;
  SystemThemeChangeEvent onSystemThemeChange;
  PrivateEvent onPrivate0;
  PrivateEvent onPrivate1;
  PrivateEvent onPrivate2;
  PrivateEvent onPrivate3;
  PollStencilEvent onPollStencil;
  RenderTargetResetEvent onRenderTargetReset;
  RenderDeviceResetEvent onRenderDeviceReset;
  RenderDeviceLostEvent onRenderDeviceLost;

public:
  CommonEventHandler() = default;
  void inject(shared<ObjectRegistry> const &registry) override {
    mRegistry = registry;
  }
  void handle(SDL_CommonEvent const &event);
};

class DisplayEventHandler : public IEventHandler {
private:
  shared<ObjectRegistry> mRegistry = nullptr;

public:
  DisplayOrientationEvent onOrientationChange;
  DisplayAddEvent onDisplayAdd;
  DisplayRemoveEvent onDisplayRemove;
  DisplayModeChangeEvent onDisplayModeChange;
  DisplayCurrentModeChangeEvent onDisplayCurrentModeChange;
  DisplayContentScaleChangeEvent onDisplayContentScaleChange;

public:
  DisplayEventHandler() = default;
  void inject(shared<ObjectRegistry> const &registry) override {
    mRegistry = registry;
  }
  void handle(SDL_DisplayEvent const &event);
};

class WindowEventHandler : public IEventHandler {
private:
  shared<ObjectRegistry> mRegistry = nullptr;

public:
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
  DropEvent onDrop;

public:
  WindowEventHandler() = default;
  void inject(shared<ObjectRegistry> const &registry) override {
    mRegistry = registry;
  }
  void handle(SDL_WindowEvent const &event);
  void handle(SDL_DropEvent const &event);
};

class KeyboardEventHandler : public IEventHandler {
private:
  shared<ObjectRegistry> mRegistry = nullptr;

public:
  KeyEvent onKeyInput;
  KeyboardAddEvent onKeyboardAdd;
  KeyboardRemoveEvent onKeyboardRemove;

public:
  KeyboardEventHandler() = default;
  void inject(shared<ObjectRegistry> const &registry) override {
    mRegistry = registry;
  }
  void handle(SDL_KeyboardEvent const &event);
  void handle(SDL_KeyboardDeviceEvent const &event);
};

class TextEventHandler : public IEventHandler {
private:
  shared<ObjectRegistry> mRegistry = nullptr;

public:
  TextEditingEvent onTextEditing;
  TextInputEvent onTextInput;
  TextEditingCandidatesEvent onTextEditingCandidates;
  ClipboardEvent onClipboard;

public:
  TextEventHandler() = default;
  void inject(shared<ObjectRegistry> const &registry) override {
    mRegistry = registry;
  }
  void handle(SDL_TextEditingEvent const &event);
  void handle(SDL_TextInputEvent const &event);
  void handle(SDL_TextEditingCandidatesEvent const &event);
  void handle(SDL_ClipboardEvent const &event);
};

class MouseEventHandler : public IEventHandler {
private:
  shared<ObjectRegistry> mRegistry = nullptr;

public:
  MouseMotionEvent onMouseMotion;
  MouseButtonEvent onMouseButton;
  MouseWheelEvent onMouseWheel;
  MouseAddEvent onMouseAdd;
  MouseRemoveEvent onMouseRemove;

public:
  MouseEventHandler() = default;
  void inject(shared<ObjectRegistry> const &registry) override {
    mRegistry = registry;
  }
  void handle(SDL_MouseMotionEvent const &event);
  void handle(SDL_MouseButtonEvent const &event);
  void handle(SDL_MouseWheelEvent const &event);
  void handle(SDL_MouseDeviceEvent const &event);
};

class JoystickEventHandler : public IEventHandler {
private:
  shared<ObjectRegistry> mRegistry = nullptr;

public:
  JoystickAxisEvent onJoystickAxisMotion;
  JoystickBallEvent onJoystickBallMotion;
  JoystickHatEvent onJoystickHatMotion;
  JoystickButtonEvent onJoystickButton;
  JoystickAddEvent onJoystickAdd;
  JoystickRemoveEvent onJoystickRemove;
  JoystickUpdateCompleteEvent onJoystickUpdateComplete;
  JoystickBatteryUpdateEvent onJoystickBatteryUpdate;

public:
  JoystickEventHandler() = default;
  void inject(shared<ObjectRegistry> const &registry) override {
    mRegistry = registry;
  }
  void handle(SDL_JoyAxisEvent const &event);
  void handle(SDL_JoyBallEvent const &event);
  void handle(SDL_JoyHatEvent const &event);
  void handle(SDL_JoyButtonEvent const &event);
  void handle(SDL_JoyDeviceEvent const &event);
  void handle(SDL_JoyBatteryEvent const &event);
};

class GamepadEventHandler : public IEventHandler {
private:
  shared<ObjectRegistry> mRegistry = nullptr;

public:
  GamepadAxisEvent onGamepadAxisMotion;
  GamepadButtonEvent onGamepadButton;
  GamepadAddEvent onGamepadAdd;
  GamepadRemoveEvent onGamepadRemove;
  GamepadRemappedEvent onGamepadRemapped;
  GamepadUpdateCompleteEvent onGamepadUpdateComplete;
  GamepadStreamHandleUpdateEvent onGamepadStreamHandleUpdate;
  GamepadTouchpadEvent onGamepadTouchpadEvent;
  GamepadSensorEvent onGamepadSensorEvent;

public:
  GamepadEventHandler() = default;
  void inject(shared<ObjectRegistry> const &registry) override {
    mRegistry = registry;
  }
  void handle(SDL_GamepadAxisEvent const &event);
  void handle(SDL_GamepadButtonEvent const &event);
  void handle(SDL_GamepadDeviceEvent const &event);
  void handle(SDL_GamepadTouchpadEvent const &event);
  void handle(SDL_GamepadSensorEvent const &event);
};

class AudioDeviceEventHandler : public IEventHandler {
private:
  shared<ObjectRegistry> mRegistry = nullptr;

public:
  AudioDeviceAddEvent onAudioDeviceAdd;
  AudioDeviceRemoveEvent onAudioDeviceRemove;
  AudioDeviceFormatChangeEvent onAudioDeviceFormatChange;

public:
  AudioDeviceEventHandler() = default;
  void inject(shared<ObjectRegistry> const &registry) override {
    mRegistry = registry;
  }
  void handle(SDL_AudioDeviceEvent const &event);
};

class PenEventHandler : public IEventHandler {
private:
  shared<ObjectRegistry> mRegistry = nullptr;

public:
  PenProximityEvent onPenProximity;
  PenTouchEvent onPenTouch;
  PenButtonEvent onPenButton;
  PenMotionEvent onPenMotion;
  PenAxisEvent onPenAxis;

public:
  PenEventHandler() = default;
  void inject(shared<ObjectRegistry> const &registry) override {
    mRegistry = registry;
  }
  void handle(SDL_PenProximityEvent const &event);
  void handle(SDL_PenTouchEvent const &event);
  void handle(SDL_PenButtonEvent const &event);
  void handle(SDL_PenMotionEvent const &event);
  void handle(SDL_PenAxisEvent const &event);
};

class CameraEventHandler : public IEventHandler {
private:
  shared<ObjectRegistry> mRegistry = nullptr;

public:
  CameraAddEvent onCameraAdd;
  CameraRemoveEvent onCameraRemove;
  CameraApprovedEvent onCameraApproved;
  CameraDeniedEvent onCameraDenied;

public:
  CameraEventHandler() = default;
  void inject(shared<ObjectRegistry> const &registry) override {
    mRegistry = registry;
  }
  void handle(SDL_CameraDeviceEvent const &event);
};

class TouchFingerEventHandler : public IEventHandler {
private:
  shared<ObjectRegistry> mRegistry = nullptr;

public:
  TouchFingerEvent onFingerTouch;

public:
  TouchFingerEventHandler() = default;
  void inject(shared<ObjectRegistry> const &registry) override {
    mRegistry = registry;
  }
  void handle(SDL_TouchFingerEvent const &event);
};

class SensorEventHandler : public IEventHandler {
private:
  shared<ObjectRegistry> mRegistry = nullptr;

public:
  SensorEvent onSensorInput;

public:
  SensorEventHandler() = default;
  void inject(shared<ObjectRegistry> const &registry) override {
    mRegistry = registry;
  }
  void handle(SDL_SensorEvent const &event) {
    this->onSensorInput.publish(event.timestamp, event.which,
                                {event.data[0], event.data[1], event.data[2],
                                 event.data[3], event.data[4], event.data[5]});
  }
};

class UserEventHandler : public IEventHandler {
private:
  shared<ObjectRegistry> mRegistry = nullptr;

public:
  UserEvent onUser;

public:
  UserEventHandler() = default;
  void inject(shared<ObjectRegistry> const &registry) override {
    mRegistry = registry;
  }
  void handle(SDL_UserEvent const &event);
};

class EventHandler {
private:
  shared<ObjectRegistry> mRegistry = nullptr;

public:
  QuitEvent onQuit;
  CommonEventHandler common;
  DisplayEventHandler display;
  WindowEventHandler window;
  KeyboardEventHandler keyboard;
  TextEventHandler text;
  MouseEventHandler mouse;
  JoystickEventHandler joystick;
  GamepadEventHandler gamepad;
  AudioDeviceEventHandler audioDevice;
  PenEventHandler pen;
  CameraEventHandler camera;
  TouchFingerEventHandler touchFinger;
  SensorEventHandler sensor;
  UserEventHandler user;

private:
  void handle(SDL_QuitEvent const &event) {
    this->onQuit.publish(event.timestamp);
  }

public:
  EventHandler(shared<ObjectRegistry> const &registry);
  ~EventHandler() = default;

  void poll();

public:
  static shared<EventHandler> create(shared<ObjectRegistry> const &registry) {
    return std::make_shared<EventHandler>(registry);
  }
};
} // namespace Terreate::SDL
