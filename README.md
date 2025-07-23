# Terreate
license : [![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)\
Terreate is a 3D game library for C++. This repository is designed for creating 3D games with OpenGL without the annoying of state based programming.
> [!NOTE]
> `Terreate` has its own wiki page that provides more details about, including a quick start guide, API definitions, and more!
> - [Terreate wiki](https://upiscium.github.io/Terreate-Wiki/)

# Dependencies
## Library dependence
These packages are used in this library.
- [SDL](https://www.libsdl.org/)
  > Simple DirectMedia Layer is a cross-platform development library designed to provide low level access to audio, keyboard, mouse, joystick, and graphics hardware via OpenGL and Direct3D. It is used by video playback software, emulators, and popular games including Valve's award winning catalog and many Humble Bundle games.
  > SDL officially supports Windows, macOS, Linux, iOS, and Android. Support for other platforms may be found in the source code.
  > SDL is written in C, works natively with C++, and there are bindings available for several other languages, including C# and Python.
  > SDL is distributed under the zlib license. This license allows you to use SDL freely in any software.\
  *from SDL website*
- [glad](https://gen.glad.sh/)
  > Multi-Language Vulkan/GL/GLES/EGL/GLX/WGL Loader-Generator.\
  *from glad website*
- [glm](https://github.com/g-truc/glm.git)
  > OpenGL Mathematics (GLM) is a header only C++ mathematics library for graphics software based on the OpenGL Shading Language (GLSL) specifications.\
  *from GLM README.md*
- [freetype](https://freetype.org/)
  > FreeType is a freely available software library to render fonts. It is written in C, designed to be small, efficient, highly customizable, and portable while capable of producing high-quality output (glyph images) of most vector and bitmap font formats.\
  *from FreeType website*

## Build dependence
This library uses `cmake` to build the projects so make sure to install `cmake` before building this library. 

### on Windows
There is no additional dependence to build this library on Windows.

### on Linux
If you are using linux environment, these packages are needed to build and execute.\
(Package names might be different in package managers. The names listed below are example in **`nix`**)
- alsa-lib
- hidapi
- ibus
- dbus
- jack2
- libdecor
- libGL
- libpulseaudio
- libusb1
- libsysprof-capture
- libdrm
- xorg.libX11
- xorg.libXcursor
- xorg.libXinerama
- xorg.libXrandr
- xorg.libXi
- xorg.libXext
- xorg.libXrender
- xorg.libXtst
- libxkbcommon
- mesa
- pipewire
- sndio
- wayland
- wayland-protocols

> [!NOTE]
> If you are using `nix` package manager and `direnv`, dependence installing is automatically executed.

# Build project
> [!IMPORTANT]
> **Make sure you installed [cmake](https://cmake.org/) before building.**

## Build outputs
In this example, build outputs are stored in `<REPOSITRY ROOT DIR>/build/` folder.\

## `Cmake` options
### `TERREATE_BUILD_TEST`
- Default: ON
- Specifies whether to build test code.

### `TERREATE_DEBUG_BUILD`
- Default: OFF
- Specifies whether to build in debug mode. In debug mode, `TERREATE_DEBUG_MODE` is automatically defined by `cmake`.

## Windows & Linux
**Make sure you installed dependencies before building.**
```shell
cmake -S . -B build
cmake --build build
```

## `nix` installed env
```shell
nix develop // if you have direnv, use `direnv allow` to automatically enter the environment
cmake -S . -B build
cmake --build build
```

# Licensing
[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)\
This repository is provided under the **BSD-3-CLAUSE** license. So you can use this library whatever you want.

# Contact
If you want to contact me, please send an e-mail to `upiscium@gmail.com`.
