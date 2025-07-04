#pragma once

#include "type.hpp"

namespace Terreate {
enum class Attachment {
  COLOR0 = GL_COLOR_ATTACHMENT0,
  COLOR1 = GL_COLOR_ATTACHMENT1,
  COLOR2 = GL_COLOR_ATTACHMENT2,
  COLOR3 = GL_COLOR_ATTACHMENT3,
  COLOR4 = GL_COLOR_ATTACHMENT4,
  COLOR5 = GL_COLOR_ATTACHMENT5,
  COLOR6 = GL_COLOR_ATTACHMENT6,
  COLOR7 = GL_COLOR_ATTACHMENT7,
  COLOR8 = GL_COLOR_ATTACHMENT8,
  COLOR9 = GL_COLOR_ATTACHMENT9,
  COLOR10 = GL_COLOR_ATTACHMENT10,
  COLOR11 = GL_COLOR_ATTACHMENT11,
  COLOR12 = GL_COLOR_ATTACHMENT12,
  COLOR13 = GL_COLOR_ATTACHMENT13,
  COLOR14 = GL_COLOR_ATTACHMENT14,
  COLOR15 = GL_COLOR_ATTACHMENT15,
  COLOR16 = GL_COLOR_ATTACHMENT16,
  COLOR17 = GL_COLOR_ATTACHMENT17,
  COLOR18 = GL_COLOR_ATTACHMENT18,
  COLOR19 = GL_COLOR_ATTACHMENT19,
  COLOR20 = GL_COLOR_ATTACHMENT20,
  COLOR21 = GL_COLOR_ATTACHMENT21,
  COLOR22 = GL_COLOR_ATTACHMENT22,
  COLOR23 = GL_COLOR_ATTACHMENT23,
  COLOR24 = GL_COLOR_ATTACHMENT24,
  COLOR25 = GL_COLOR_ATTACHMENT25,
  COLOR26 = GL_COLOR_ATTACHMENT26,
  COLOR27 = GL_COLOR_ATTACHMENT27,
  COLOR28 = GL_COLOR_ATTACHMENT28,
  COLOR29 = GL_COLOR_ATTACHMENT29,
  COLOR30 = GL_COLOR_ATTACHMENT30,
  COLOR31 = GL_COLOR_ATTACHMENT31
};

enum class BufferBit {
  DEPTH_BUFFER = GL_DEPTH_BUFFER_BIT,
  STENCIL_BUFFER = GL_STENCIL_BUFFER_BIT,
  COLOR_BUFFER = GL_COLOR_BUFFER_BIT
};

enum class BufferUsage {
  STREAM_DRAW = GL_STREAM_DRAW,
  STREAM_READ = GL_STREAM_READ,
  STREAM_COPY = GL_STREAM_COPY,
  STATIC_DRAW = GL_STATIC_DRAW,
  STATIC_READ = GL_STATIC_READ,
  STATIC_COPY = GL_STATIC_COPY,
  DYNAMIC_DRAW = GL_DYNAMIC_DRAW,
  DYNAMIC_READ = GL_DYNAMIC_READ,
  DYNAMIC_COPY = GL_DYNAMIC_COPY
};

enum class BufferType {
  ARRAY_BUFFER = GL_ARRAY_BUFFER,
  ELEMENT_BUFFER = GL_ELEMENT_ARRAY_BUFFER
};

enum class BlendingFunction {
  ZERO = GL_ZERO,
  ONE = GL_ONE,
  SRC_COLOR = GL_SRC_COLOR,
  ONE_MINUS_SRC_COLOR = GL_ONE_MINUS_SRC_COLOR,
  DST_COLOR = GL_DST_COLOR,
  ONE_MINUS_DST_COLOR = GL_ONE_MINUS_DST_COLOR,
  SRC_ALPHA = GL_SRC_ALPHA,
  ONE_MINUS_SRC_ALPHA = GL_ONE_MINUS_SRC_ALPHA,
  DST_ALPHA = GL_DST_ALPHA,
  ONE_MINUS_DST_ALPHA = GL_ONE_MINUS_DST_ALPHA,
  CONSTANT_COLOR = GL_CONSTANT_COLOR,
  ONE_MINUS_CONSTANT_COLOR = GL_ONE_MINUS_CONSTANT_COLOR,
  CONSTANT_ALPHA = GL_CONSTANT_ALPHA,
  ONE_MINUS_CONSTANT_ALPHA = GL_ONE_MINUS_CONSTANT_ALPHA,
  SRC_ALPHA_SATURATE = GL_SRC_ALPHA_SATURATE
};

enum class CubeFace {
  RIGHT = GL_TEXTURE_CUBE_MAP_POSITIVE_X,
  LEFT = GL_TEXTURE_CUBE_MAP_NEGATIVE_X,
  TOP = GL_TEXTURE_CUBE_MAP_POSITIVE_Y,
  BOTTOM = GL_TEXTURE_CUBE_MAP_NEGATIVE_Y,
  FRONT = GL_TEXTURE_CUBE_MAP_POSITIVE_Z,
  BACK = GL_TEXTURE_CUBE_MAP_NEGATIVE_Z
};

enum class CullingFace {
  FRONT = GL_FRONT,
  BACK = GL_BACK,
  FRONT_AND_BACK = GL_FRONT_AND_BACK
};

enum class CullingMode { CW = GL_CW, CCW = GL_CCW };

enum class CursorMode {
  NORMAL = GLFW_CURSOR_NORMAL,
  HIDDEN = GLFW_CURSOR_HIDDEN,
  DISABLED = GLFW_CURSOR_DISABLED,
  CAPTURED = GLFW_CURSOR_CAPTURED
};

enum class CursorShape {
  DEFAULT = -1,
  ARROW = GLFW_ARROW_CURSOR,
  IBEAM = GLFW_IBEAM_CURSOR,
  CROSSHAIR = GLFW_CROSSHAIR_CURSOR,
  HAND = GLFW_HAND_CURSOR,
  HRESIZE = GLFW_HRESIZE_CURSOR,
  VRESIZE = GLFW_VRESIZE_CURSOR,
  POINTGLFW_POINTING_HAND = GLFW_POINTING_HAND_CURSOR,
  RESIZE_EW = GLFW_RESIZE_EW_CURSOR,
  RESIZE_NS = GLFW_RESIZE_NS_CURSOR,
  RESIZE_NWSE = GLFW_RESIZE_NWSE_CURSOR,
  RESIZE_NESW = GLFW_RESIZE_NESW_CURSOR,
  RESIZE_ALL = GLFW_RESIZE_ALL_CURSOR,
  NOT_ALLOWED = GLFW_NOT_ALLOWED_CURSOR
};

enum class DepthFunction {
  NEVER = GL_NEVER,
  LESS = GL_LESS,
  EQUAL = GL_EQUAL,
  LEQUAL = GL_LEQUAL,
  GREATER = GL_GREATER,
  NOTEQUAL = GL_NOTEQUAL,
  GEQUAL = GL_GEQUAL,
  ALWAYS = GL_ALWAYS
};

enum class DrawMode {
  POINTS = GL_POINTS,
  LINES = GL_LINES,
  LINE_LOOP = GL_LINE_LOOP,
  LINE_STRIP = GL_LINE_STRIP,
  TRIANGLES = GL_TRIANGLES,
  TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
  TRIANGLE_FAN = GL_TRIANGLE_FAN,
  QUADS = GL_QUADS
};

enum class FilterType {
  NEAREST = GL_NEAREST,
  LINEAR = GL_LINEAR,
  NEAREST_MIPMAP_NEAREST = GL_NEAREST_MIPMAP_NEAREST, // MIN_FILTER only
  LINEAR_MIPMAP_NEAREST = GL_LINEAR_MIPMAP_NEAREST,   // MIN_FILTER only
  NEAREST_MIPMAP_LINEAR = GL_NEAREST_MIPMAP_LINEAR,   // MIN_FILTER only
  LINEAR_MIPMAP_LINEAR = GL_LINEAR_MIPMAP_LINEAR      // MIN_FILTER only
};

enum class GLError {
  NO_ERROR = GL_NO_ERROR,
  INVALID_ENUM = GL_INVALID_ENUM,
  INVALID_VALUE = GL_INVALID_VALUE,
  INVALID_OPERATION = GL_INVALID_OPERATION,
  STACK_OVERFLOW = GL_STACK_OVERFLOW,
  STACK_UNDERFLOW = GL_STACK_UNDERFLOW,
  OUT_OF_MEMORY = GL_OUT_OF_MEMORY,
  INVALID_FRAMEBUFFER_OPERATION = GL_INVALID_FRAMEBUFFER_OPERATION,
  CONTEXT_LOST = GL_CONTEXT_LOST
};

enum class GLFeature {
  BLEND = GL_BLEND,
  CULL_FACE = GL_CULL_FACE,
  DEPTH_TEST = GL_DEPTH_TEST,
  DITHER = GL_DITHER,
  POLYGON_OFFSET_FILL = GL_POLYGON_OFFSET_FILL,
  SAMPLE_ALPHA_TO_COVERAGE = GL_SAMPLE_ALPHA_TO_COVERAGE,
  SAMPLE_COVERAGE = GL_SAMPLE_COVERAGE,
  SCISSOR_TEST = GL_SCISSOR_TEST,
  STENCIL_TEST = GL_STENCIL_TEST
};

enum class InputType {
  STICKY_KEYS = GLFW_STICKY_KEYS,
  STICKY_MOUSE_BUTTONS = GLFW_STICKY_MOUSE_BUTTONS,
  LOCK_KEY_MODS = GLFW_LOCK_KEY_MODS,
  RAW_MOUSE_MOTION = GLFW_RAW_MOUSE_MOTION,
  MOUSE_PASSTHROUGH = GLFW_MOUSE_PASSTHROUGH
};

enum class JoystickAxis {
  LEFT_X = GLFW_GAMEPAD_AXIS_LEFT_X,
  LEFT_Y = GLFW_GAMEPAD_AXIS_LEFT_Y,
  RIGHT_X = GLFW_GAMEPAD_AXIS_RIGHT_X,
  RIGHT_Y = GLFW_GAMEPAD_AXIS_RIGHT_Y,
  LEFT_TRIGGER = GLFW_GAMEPAD_AXIS_LEFT_TRIGGER,
  RIGHT_TRIGGER = GLFW_GAMEPAD_AXIS_RIGHT_TRIGGER,
  LAST = GLFW_GAMEPAD_AXIS_LAST
};

enum class JoystickButton {
  A = GLFW_GAMEPAD_BUTTON_A,
  B = GLFW_GAMEPAD_BUTTON_B,
  X = GLFW_GAMEPAD_BUTTON_X,
  Y = GLFW_GAMEPAD_BUTTON_Y,
  LEFT_BUMPER = GLFW_GAMEPAD_BUTTON_LEFT_BUMPER,
  RIGHT_BUMPER = GLFW_GAMEPAD_BUTTON_RIGHT_BUMPER,
  BACK = GLFW_GAMEPAD_BUTTON_BACK,
  START = GLFW_GAMEPAD_BUTTON_START,
  GUIDE = GLFW_GAMEPAD_BUTTON_GUIDE,
  LEFT_THUMB = GLFW_GAMEPAD_BUTTON_LEFT_THUMB,
  RIGHT_THUMB = GLFW_GAMEPAD_BUTTON_RIGHT_THUMB,
  DPAD_UP = GLFW_GAMEPAD_BUTTON_DPAD_UP,
  DPAD_RIGHT = GLFW_GAMEPAD_BUTTON_DPAD_RIGHT,
  DPAD_DOWN = GLFW_GAMEPAD_BUTTON_DPAD_DOWN,
  DPAD_LEFT = GLFW_GAMEPAD_BUTTON_DPAD_LEFT,
  CROSS = GLFW_GAMEPAD_BUTTON_CROSS,
  CIRCLE = GLFW_GAMEPAD_BUTTON_CIRCLE,
  SQUARE = GLFW_GAMEPAD_BUTTON_SQUARE,
  TRIANGLE = GLFW_GAMEPAD_BUTTON_TRIANGLE,
  LAST = GLFW_GAMEPAD_BUTTON_LAST
};

enum class JoystickEvent {
  CONNECTED = GLFW_CONNECTED,
  DISCONNECTED = GLFW_DISCONNECTED
};

enum class JoystickHat {
  UP = GLFW_HAT_UP,
  RIGHT = GLFW_HAT_RIGHT,
  DOWN = GLFW_HAT_DOWN,
  LEFT = GLFW_HAT_LEFT,
  RIGHT_UP = GLFW_HAT_RIGHT_UP,
  RIGHT_DOWN = GLFW_HAT_RIGHT_DOWN,
  LEFT_UP = GLFW_HAT_LEFT_UP,
  LEFT_DOWN = GLFW_HAT_LEFT_DOWN
};

enum class JoystickID {
  NONE = -1,
  JOYSTICK1 = GLFW_JOYSTICK_1,
  JOYSTICK2 = GLFW_JOYSTICK_2,
  JOYSTICK3 = GLFW_JOYSTICK_3,
  JOYSTICK4 = GLFW_JOYSTICK_4,
  JOYSTICK5 = GLFW_JOYSTICK_5,
  JOYSTICK6 = GLFW_JOYSTICK_6,
  JOYSTICK7 = GLFW_JOYSTICK_7,
  JOYSTICK8 = GLFW_JOYSTICK_8,
  JOYSTICK9 = GLFW_JOYSTICK_9,
  JOYSTICK10 = GLFW_JOYSTICK_10,
  JOYSTICK11 = GLFW_JOYSTICK_11,
  JOYSTICK12 = GLFW_JOYSTICK_12,
  JOYSTICK13 = GLFW_JOYSTICK_13,
  JOYSTICK14 = GLFW_JOYSTICK_14,
  JOYSTICK15 = GLFW_JOYSTICK_15,
  JOYSTICK16 = GLFW_JOYSTICK_16,
  LAST = GLFW_JOYSTICK_LAST
};

enum class Keyboard {
  K_SPACE = GLFW_KEY_SPACE,
  K_APOSTROPHE = GLFW_KEY_APOSTROPHE,
  K_COMMA = GLFW_KEY_COMMA,
  K_MINUS = GLFW_KEY_MINUS,
  K_PERIOD = GLFW_KEY_PERIOD,
  K_SLASH = GLFW_KEY_SLASH,
  K_0 = GLFW_KEY_0,
  K_1 = GLFW_KEY_1,
  K_2 = GLFW_KEY_2,
  K_3 = GLFW_KEY_3,
  K_4 = GLFW_KEY_4,
  K_5 = GLFW_KEY_5,
  K_6 = GLFW_KEY_6,
  K_7 = GLFW_KEY_7,
  K_8 = GLFW_KEY_8,
  K_9 = GLFW_KEY_9,
  K_SEMICOLON = GLFW_KEY_SEMICOLON,
  K_EQUAL = GLFW_KEY_EQUAL,
  K_A = GLFW_KEY_A,
  K_B = GLFW_KEY_B,
  K_C = GLFW_KEY_C,
  K_D = GLFW_KEY_D,
  K_E = GLFW_KEY_E,
  K_F = GLFW_KEY_F,
  K_G = GLFW_KEY_G,
  K_H = GLFW_KEY_H,
  K_I = GLFW_KEY_I,
  K_J = GLFW_KEY_J,
  K_K = GLFW_KEY_K,
  K_L = GLFW_KEY_L,
  K_M = GLFW_KEY_M,
  K_N = GLFW_KEY_N,
  K_O = GLFW_KEY_O,
  K_P = GLFW_KEY_P,
  K_Q = GLFW_KEY_Q,
  K_R = GLFW_KEY_R,
  K_S = GLFW_KEY_S,
  K_T = GLFW_KEY_T,
  K_U = GLFW_KEY_U,
  K_V = GLFW_KEY_V,
  K_W = GLFW_KEY_W,
  K_X = GLFW_KEY_X,
  K_Y = GLFW_KEY_Y,
  K_Z = GLFW_KEY_Z,
  K_LEFT_BRACKET = GLFW_KEY_LEFT_BRACKET,
  K_BACKSLASH = GLFW_KEY_BACKSLASH,
  K_RIGHT_BRACKET = GLFW_KEY_RIGHT_BRACKET,
  K_GRAVE_ACCENT = GLFW_KEY_GRAVE_ACCENT,
  K_WORLD_1 = GLFW_KEY_WORLD_1,
  K_WORLD_2 = GLFW_KEY_WORLD_2,
  K_ESCAPE = GLFW_KEY_ESCAPE,
  K_ENTER = GLFW_KEY_ENTER,
  K_TAB = GLFW_KEY_TAB,
  K_BACKSPACE = GLFW_KEY_BACKSPACE,
  K_INSERT = GLFW_KEY_INSERT,
  K_DELETE = GLFW_KEY_DELETE,
  K_RIGHT = GLFW_KEY_RIGHT,
  K_LEFT = GLFW_KEY_LEFT,
  K_DOWN = GLFW_KEY_DOWN,
  K_UP = GLFW_KEY_UP,
  K_PAGE_UP = GLFW_KEY_PAGE_UP,
  K_PAGE_DOWN = GLFW_KEY_PAGE_DOWN,
  K_HOME = GLFW_KEY_HOME,
  K_END = GLFW_KEY_END,
  K_CAPS_LOCK = GLFW_KEY_CAPS_LOCK,
  K_SCROLL_LOCK = GLFW_KEY_SCROLL_LOCK,
  K_NUM_LOCK = GLFW_KEY_NUM_LOCK,
  K_PRINT_SCREEN = GLFW_KEY_PRINT_SCREEN,
  K_PAUSE = GLFW_KEY_PAUSE,
  K_F1 = GLFW_KEY_F1,
  K_F2 = GLFW_KEY_F2,
  K_F3 = GLFW_KEY_F3,
  K_F4 = GLFW_KEY_F4,
  K_F5 = GLFW_KEY_F5,
  K_F6 = GLFW_KEY_F6,
  K_F7 = GLFW_KEY_F7,
  K_F8 = GLFW_KEY_F8,
  K_F9 = GLFW_KEY_F9,
  K_F10 = GLFW_KEY_F10,
  K_F11 = GLFW_KEY_F11,
  K_F12 = GLFW_KEY_F12,
  K_F13 = GLFW_KEY_F13,
  K_F14 = GLFW_KEY_F14,
  K_F15 = GLFW_KEY_F15,
  K_F16 = GLFW_KEY_F16,
  K_F17 = GLFW_KEY_F17,
  K_F18 = GLFW_KEY_F18,
  K_F19 = GLFW_KEY_F19,
  K_F20 = GLFW_KEY_F20,
  K_F21 = GLFW_KEY_F21,
  K_F22 = GLFW_KEY_F22,
  K_F23 = GLFW_KEY_F23,
  K_F24 = GLFW_KEY_F24,
  K_F25 = GLFW_KEY_F25,
  K_KP_0 = GLFW_KEY_KP_0,
  K_KP_1 = GLFW_KEY_KP_1,
  K_KP_2 = GLFW_KEY_KP_2,
  K_KP_3 = GLFW_KEY_KP_3,
  K_KP_4 = GLFW_KEY_KP_4,
  K_KP_5 = GLFW_KEY_KP_5,
  K_KP_6 = GLFW_KEY_KP_6,
  K_KP_7 = GLFW_KEY_KP_7,
  K_KP_8 = GLFW_KEY_KP_8,
  K_KP_9 = GLFW_KEY_KP_9,
  K_KP_DECIMAL = GLFW_KEY_KP_DECIMAL,
  K_KP_DIVIDE = GLFW_KEY_KP_DIVIDE,
  K_KP_MULTIPLY = GLFW_KEY_KP_MULTIPLY,
  K_KP_SUBTRACT = GLFW_KEY_KP_SUBTRACT,
  K_KP_ADD = GLFW_KEY_KP_ADD,
  K_KP_ENTER = GLFW_KEY_KP_ENTER,
  K_KP_EQUAL = GLFW_KEY_KP_EQUAL,
  K_LEFT_SHIFT = GLFW_KEY_LEFT_SHIFT,
  K_LEFT_CONTROL = GLFW_KEY_LEFT_CONTROL,
  K_LEFT_ALT = GLFW_KEY_LEFT_ALT,
  K_LEFT_SUPER = GLFW_KEY_LEFT_SUPER,
  K_RIGHT_SHIFT = GLFW_KEY_RIGHT_SHIFT,
  K_RIGHT_CONTROL = GLFW_KEY_RIGHT_CONTROL,
  K_RIGHT_ALT = GLFW_KEY_RIGHT_ALT,
  K_RIGHT_SUPER = GLFW_KEY_RIGHT_SUPER,
  K_MENU = GLFW_KEY_MENU,
  K_LAST = GLFW_KEY_LAST
};

enum class MousebuttonInput {
  BUTTON1 = GLFW_MOUSE_BUTTON_1,
  LEFT_BUTTON = GLFW_MOUSE_BUTTON_LEFT,
  BUTTON2 = GLFW_MOUSE_BUTTON_2,
  RIGHT_BUTTON = GLFW_MOUSE_BUTTON_RIGHT,
  BUTTON3 = GLFW_MOUSE_BUTTON_3,
  MIDDLE_BUTTON = GLFW_MOUSE_BUTTON_MIDDLE,
  BUTTON4 = GLFW_MOUSE_BUTTON_4,
  BUTTON5 = GLFW_MOUSE_BUTTON_5,
  BUTTON6 = GLFW_MOUSE_BUTTON_6,
  BUTTON7 = GLFW_MOUSE_BUTTON_7,
  BUTTON8 = GLFW_MOUSE_BUTTON_8
};

enum class StencilFunction {
  NEVER = GL_NEVER,
  LESS = GL_LESS,
  EQUAL = GL_EQUAL,
  LEQUAL = GL_LEQUAL,
  GREATER = GL_GREATER,
  NOTEQUAL = GL_NOTEQUAL,
  GEQUAL = GL_GEQUAL,
  ALWAYS = GL_ALWAYS
};

enum class StencilOperation {
  KEEP = GL_KEEP,
  ZERO = GL_ZERO,
  REPLACE = GL_REPLACE,
  INCR = GL_INCR,
  INCR_WRAP = GL_INCR_WRAP,
  DECR = GL_DECR,
  DECR_WRAP = GL_DECR_WRAP,
  INVERT = GL_INVERT
};

enum class TextureChannelType {
  /* RED = GL_RED, */
  /* R16F = GL_R16F, */
  /* R32F = GL_R32F, */
  /* RG = GL_RG, */
  /* RG16F = GL_RG16F, */
  /* RG32F = GL_RG32F, */
  /* RGB = GL_RGB, */
  /* RGB16F = GL_RGB16F, */
  /* RGB32F = GL_RGB32F, */
  RGBA = GL_RGBA,
  RGBA16F = GL_RGBA16F,
  RGBA32F = GL_RGBA32F
};

enum class TextureTargets {
  TEX_0 = GL_TEXTURE0,
  TEX_1 = GL_TEXTURE1,
  TEX_2 = GL_TEXTURE2,
  TEX_3 = GL_TEXTURE3,
  TEX_4 = GL_TEXTURE4,
  TEX_5 = GL_TEXTURE5,
  TEX_6 = GL_TEXTURE6,
  TEX_7 = GL_TEXTURE7,
  TEX_8 = GL_TEXTURE8,
  TEX_9 = GL_TEXTURE9,
  TEX_10 = GL_TEXTURE10,
  TEX_11 = GL_TEXTURE11,
  TEX_12 = GL_TEXTURE12,
  TEX_13 = GL_TEXTURE13,
  TEX_14 = GL_TEXTURE14,
  TEX_15 = GL_TEXTURE15,
  TEX_16 = GL_TEXTURE16,
  TEX_17 = GL_TEXTURE17,
  TEX_18 = GL_TEXTURE18,
  TEX_19 = GL_TEXTURE19,
  TEX_20 = GL_TEXTURE20,
  TEX_21 = GL_TEXTURE21,
  TEX_22 = GL_TEXTURE22,
  TEX_23 = GL_TEXTURE23,
  TEX_24 = GL_TEXTURE24,
  TEX_25 = GL_TEXTURE25,
  TEX_26 = GL_TEXTURE26,
  TEX_27 = GL_TEXTURE27,
  TEX_28 = GL_TEXTURE28,
  TEX_29 = GL_TEXTURE29,
  TEX_30 = GL_TEXTURE30,
  TEX_31 = GL_TEXTURE31,
};

enum class WrappingType {
  CLAMP_TO_EDGE = GL_CLAMP_TO_EDGE,
  CLAMP_TO_BORDER = GL_CLAMP_TO_BORDER,
  MIRRORED_REPEAT = GL_MIRRORED_REPEAT,
  REPEAT = GL_REPEAT
};
} // namespace Terreate
