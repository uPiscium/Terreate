#pragma once

#include "common/bitflag.hpp"
#include "common/event.hpp"
#include "common/type.hpp"

#include "camera.hpp"
#include "enum.hpp"
#include "gamepad.hpp"
#include "joystick.hpp"
#include "mouse.hpp"
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

typedef Event<> QuitEvent;

typedef Event<> TerminateEvent;
typedef Event<> LowMemoryEvent;
typedef Event<> WillEnterBackgroundEvent;
typedef Event<> DidEnterBackgroundEvent;
typedef Event<> LocaleChangeEvent;
typedef Event<> SystemThemeChangeEvent;
typedef Event<> PrivateEvent;
typedef Event<> PollStencilEvent;
typedef Event<> RenderTargetResetEvent;
typedef Event<> RenderDeviceResetEvent;
typedef Event<> RenderDeviceLostEvent;

typedef Event<DisplayID, u32> DisplayOrientationEvent;
typedef Event<DisplayID> DisplayAddEvent;
typedef Event<DisplayID> DisplayRemoveEvent;
typedef Event<DisplayID, SDL_DisplayMode const *> DisplayModeChangeEvent;
typedef Event<DisplayID, SDL_DisplayMode const *> DisplayCurrentModeChangeEvent;
typedef Event<DisplayID, float> DisplayContentScaleChangeEvent;

typedef Event<shared<Window>, bool> WindowVisibilityChangeEvent;
typedef Event<shared<Window>, bool> WindowExposeEvent;
typedef Event<shared<Window>, i32, i32> WindowMoveEvent;
typedef Event<shared<Window>, i32, i32> WindowResizeEvent;
typedef Event<shared<Window>, i32, i32> WindowPixelSizeChangeEvent;
// typedef Event<shared<Window>> WindowMetalViewResizeEvent;
typedef Event<shared<Window>> WindowMinimizeEvent;
typedef Event<shared<Window>> WindowMaximizeEvent;
typedef Event<shared<Window>> WindowRestoreEvent;
typedef Event<shared<Window>> WindowMouseEnterEvent;
typedef Event<shared<Window>> WindowMouseLeaveEvent;
typedef Event<shared<Window>, bool> WindowKeyboardFocusChangeEvent;
typedef Event<shared<Window>> WindowCloseRequestedEvent;
typedef Event<shared<Window>, HitTestResult> WindowHitTestEvent;
// typedef Event<shared<Window>> WindowICCProfileChangeEvent;
typedef Event<shared<Window>, DisplayID> WindowDisplayChangeEvent;
typedef Event<shared<Window>, float> WindowDisplayScaleChangeEvent;
typedef Event<shared<Window>, u32, u32> WindowSafeAreaChangeEvent;
typedef Event<shared<Window>> WindowOccludeEvent;
typedef Event<shared<Window>, bool> WindowFullscreenChangeEvent;
typedef Event<shared<Window>> WindowDestroyEvent;
typedef Event<shared<Window>> WindowHDRStateChangeEvent;

typedef Event<Key> KeyEvent;

typedef Event<shared<Window>, str, i32> TextEditingEvent;
typedef Event<shared<Window>, str> TextInputEvent;

typedef Event<KeyboardID> KeyboardAddEvent;
typedef Event<KeyboardID> KeyboardRemoveEvent;

typedef Event<shared<Window>, vec<str>, u32, TextCandidateOrientation>
    TextEditingCandidatesEvent;

typedef Event<shared<Window>, shared<Mouse>, vec2, vec2> MouseMotionEvent;
typedef Event<shared<Window>, shared<Mouse>, MouseButton, bool, u8, vec2>
    MouseButtonEvent;
typedef Event<shared<Window>, shared<Mouse>, MouseWheelDirection, vec2, vec2,
              pair<u32>>
    MouseWheelEvent;
typedef Event<shared<Mouse>> MouseAddEvent;
typedef Event<shared<Mouse>> MouseRemoveEvent;

typedef Event<shared<Joystick>, u8, i16> JoystickAxisEvent;
typedef Event<shared<Joystick>, u8, pair<i16>> JoystickBallEvent;
typedef Event<shared<Joystick>, u8, JoystickHatState> JoystickHatEvent;
typedef Event<shared<Joystick>, u8, bool> JoystickButtonEvent;
typedef Event<shared<Joystick>> JoystickAddEvent;
typedef Event<shared<Joystick>> JoystickRemoveEvent;
typedef Event<shared<Joystick>> JoystickUpdateCompleteEvent;
typedef Event<shared<Joystick>, PowerState, i32> JoystickBatteryUpdateEvent;

typedef Event<shared<Gamepad>, GamepadAxis, i16> GamepadAxisEvent;
typedef Event<shared<Gamepad>, GamepadButton, bool> GamepadButtonEvent;
typedef Event<shared<Gamepad>> GamepadAddEvent;
typedef Event<shared<Gamepad>> GamepadRemoveEvent;
typedef Event<shared<Gamepad>> GamepadRemappedEvent;
typedef Event<shared<Gamepad>> GamepadUpdateCompleteEvent;
typedef Event<shared<Gamepad>> GamepadStreamHandleUpdateEvent;
typedef Event<shared<Gamepad>, i32, i32, vec2, float> GamepadTouchpadEvent;
typedef Event<shared<Gamepad>, SDLSensor, vec3, u64> GamepadSensorEvent;

typedef Event<shared<Window>, SDL_TouchID, SDL_FingerID, vec2, vec2, float>
    TouchFingerEvent;

typedef Event<bool, vec<str>> ClipboardEvent;

typedef Event<shared<Window>, vec2, str, str> DropEvent;

typedef Event<shared<AudioDevice>, bool> AudioDeviceAddEvent;
typedef Event<shared<AudioDevice>, bool> AudioDeviceRemoveEvent;
typedef Event<shared<AudioDevice>, bool, AudioFormat, i32, i32, i32>
    AudioDeviceFormatChangeEvent;

typedef Event<SensorID, vec<float>> SensorEvent;

typedef Event<shared<Window>, PenID> PenProximityEvent;
typedef Event<shared<Window>, PenID, Bitflag<PenState>, vec2, bool, bool>
    PenTouchEvent;
typedef Event<shared<Window>, PenID, Bitflag<PenState>, vec2, u8, bool>
    PenButtonEvent;
typedef Event<shared<Window>, PenID, Bitflag<PenState>, vec2> PenMotionEvent;
typedef Event<shared<Window>, PenID, PenAxis, vec2, float> PenAxisEvent;

typedef Event<shared<Camera>> CameraAddEvent;
typedef Event<shared<Camera>> CameraRemoveEvent;
typedef Event<shared<Camera>> CameraApprovedEvent;
typedef Event<shared<Camera>> CameraDeniedEvent;

typedef Event<shared<Window>, u32, pair<void *>> UserEvent;

class IEventHandler {
public:
  virtual ~IEventHandler() = default;
  virtual void inject(shared<Registry> const &registry) = 0;
};

class CommonEventHandler : public IEventHandler {
private:
  shared<Registry> mRegistry = nullptr;

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
  void inject(shared<Registry> const &registry) override;
  void handle(SDL_CommonEvent const &event);
};

class DisplayEventHandler : public IEventHandler {
private:
  shared<Registry> mRegistry = nullptr;

public:
  DisplayOrientationEvent onOrientationChange;
  DisplayAddEvent onDisplayAdd;
  DisplayRemoveEvent onDisplayRemove;
  DisplayModeChangeEvent onDisplayModeChange;
  DisplayCurrentModeChangeEvent onDisplayCurrentModeChange;
  DisplayContentScaleChangeEvent onDisplayContentScaleChange;

public:
  DisplayEventHandler() = default;
  void inject(shared<Registry> const &registry) override;
  void handle(SDL_DisplayEvent const &event);
};

class WindowEventHandler : public IEventHandler {
private:
  shared<Registry> mRegistry = nullptr;

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
  void inject(shared<Registry> const &registry) override;
  void handle(SDL_WindowEvent const &event);
  void handle(SDL_DropEvent const &event);
};

class KeyboardEventHandler : public IEventHandler {
private:
  shared<Registry> mRegistry = nullptr;

public:
  KeyEvent onKeyInput;
  KeyboardAddEvent onKeyboardAdd;
  KeyboardRemoveEvent onKeyboardRemove;

public:
  KeyboardEventHandler() = default;
  void inject(shared<Registry> const &registry) override;
  void handle(SDL_KeyboardEvent const &event);
  void handle(SDL_KeyboardDeviceEvent const &event);
};

class TextEventHandler : public IEventHandler {
private:
  shared<Registry> mRegistry = nullptr;

public:
  TextEditingEvent onTextEditing;
  TextInputEvent onTextInput;
  TextEditingCandidatesEvent onTextEditingCandidates;
  ClipboardEvent onClipboard;

public:
  TextEventHandler() = default;
  void inject(shared<Registry> const &registry) override;
  void handle(SDL_TextEditingEvent const &event);
  void handle(SDL_TextInputEvent const &event);
  void handle(SDL_TextEditingCandidatesEvent const &event);
  void handle(SDL_ClipboardEvent const &event);
};

class MouseEventHandler : public IEventHandler {
private:
  shared<Registry> mRegistry = nullptr;

public:
  MouseMotionEvent onMouseMotion;
  MouseButtonEvent onMouseButton;
  MouseWheelEvent onMouseWheel;
  MouseAddEvent onMouseAdd;
  MouseRemoveEvent onMouseRemove;

public:
  MouseEventHandler() = default;
  void inject(shared<Registry> const &registry) override;
  void handle(SDL_MouseMotionEvent const &event);
  void handle(SDL_MouseButtonEvent const &event);
  void handle(SDL_MouseWheelEvent const &event);
  void handle(SDL_MouseDeviceEvent const &event);
};

class JoystickEventHandler : public IEventHandler {
private:
  shared<Registry> mRegistry = nullptr;

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
  void inject(shared<Registry> const &registry) override;
  void handle(SDL_JoyAxisEvent const &event);
  void handle(SDL_JoyBallEvent const &event);
  void handle(SDL_JoyHatEvent const &event);
  void handle(SDL_JoyButtonEvent const &event);
  void handle(SDL_JoyDeviceEvent const &event);
  void handle(SDL_JoyBatteryEvent const &event);
};

class GamepadEventHandler : public IEventHandler {
private:
  shared<Registry> mRegistry = nullptr;

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
  void inject(shared<Registry> const &registry) override;
  void handle(SDL_GamepadAxisEvent const &event);
  void handle(SDL_GamepadButtonEvent const &event);
  void handle(SDL_GamepadDeviceEvent const &event);
  void handle(SDL_GamepadTouchpadEvent const &event);
  void handle(SDL_GamepadSensorEvent const &event);
};

class AudioDeviceEventHandler : public IEventHandler {
private:
  shared<Registry> mRegistry = nullptr;

public:
  AudioDeviceAddEvent onAudioDeviceAdd;
  AudioDeviceRemoveEvent onAudioDeviceRemove;
  AudioDeviceFormatChangeEvent onAudioDeviceFormatChange;

public:
  AudioDeviceEventHandler() = default;
  void inject(shared<Registry> const &registry) override;
  void handle(SDL_AudioDeviceEvent const &event);
};

class PenEventHandler : public IEventHandler {
private:
  shared<Registry> mRegistry = nullptr;

public:
  PenProximityEvent onPenProximity;
  PenTouchEvent onPenTouch;
  PenButtonEvent onPenButton;
  PenMotionEvent onPenMotion;
  PenAxisEvent onPenAxis;

public:
  PenEventHandler() = default;
  void inject(shared<Registry> const &registry) override;
  void handle(SDL_PenProximityEvent const &event);
  void handle(SDL_PenTouchEvent const &event);
  void handle(SDL_PenButtonEvent const &event);
  void handle(SDL_PenMotionEvent const &event);
  void handle(SDL_PenAxisEvent const &event);
};

class CameraEventHandler : public IEventHandler {
private:
  shared<Registry> mRegistry = nullptr;

public:
  CameraAddEvent onCameraAdd;
  CameraRemoveEvent onCameraRemove;
  CameraApprovedEvent onCameraApproved;
  CameraDeniedEvent onCameraDenied;

public:
  CameraEventHandler() = default;
  void inject(shared<Registry> const &registry) override;
  void handle(SDL_CameraDeviceEvent const &event);
};

class TouchFingerEventHandler : public IEventHandler {
private:
  shared<Registry> mRegistry = nullptr;

public:
  TouchFingerEvent onFingerTouch;

public:
  TouchFingerEventHandler() = default;
  void inject(shared<Registry> const &registry) override;
  void handle(SDL_TouchFingerEvent const &event);
};

class SensorEventHandler : public IEventHandler {
private:
  shared<Registry> mRegistry = nullptr;

public:
  SensorEvent onSensorInput;

public:
  SensorEventHandler() = default;
  void inject(shared<Registry> const &registry) override;
  void handle(SDL_SensorEvent const &event);
};

class UserEventHandler : public IEventHandler {
private:
  shared<Registry> mRegistry = nullptr;

public:
  UserEvent onUser;

public:
  UserEventHandler() = default;
  void inject(shared<Registry> const &registry) override;
  void handle(SDL_UserEvent const &event);
};

class EventHandler {
private:
  shared<Registry> mRegistry = nullptr;

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
  void handle(SDL_QuitEvent const &event);

public:
  EventHandler(shared<Registry> const &registry);
  ~EventHandler() = default;

  void poll();

public:
  static shared<EventHandler> create(shared<Registry> const &registry) {
    return std::make_shared<EventHandler>(registry);
  }
};
} // namespace Terreate::SDL
