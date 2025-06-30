# Terreate
license : [![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)\
Terreate is a 3D game library for C++. This repository is designed for creating 3D games with OpenGL and OpenAL without the annoying of state based programming.
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
- [glad](https://glad.dav1d.de/)
  > Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs.\
  *from glad website*
- [OpenAL-soft](https://github.com/kcat/openal-soft.git)
  > OpenAL Soft is an LGPL-licensed, cross-platform, software implementation of the OpenAL 3D audio API. It's forked from the open-sourced Windows version available originally from openal.org's SVN repository (now defunct). OpenAL provides capabilities for playing audio in a virtual 3D environment. Distance attenuation, doppler shift, and directional sound emitters are among the features handled by the API. More advanced effects, including air absorption, occlusion, and environmental reverb, are available through the EFX extension. It also facilitates streaming audio, multi-channel buffers, and audio capture.\
  *from OpenAL-soft README.md*

## Build dependence
This library uses `cmake` to build the projects so make sure to install `cmake` before building this library. 

### on Windows
There is no additional dependence to build this library on Windows.

### on Linux
If you are using X11/Wayland environment, these packages are needed to build and execute.\
(Package names might be different in package managers. The names listed below are example in **`apt`**)
- libwayland-bin
- libwayland-dev
- wayland-protocols
- libxkbcommon-dev
- libx11-dev
- libxrandr-dev
- libxinerama-dev
- libxcursor-dev
- libxi-dev

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
