# Terreate
`main` CI status : ![build workflow](https://github.com/uPiscium/Terreate/actions/workflows/cmake-multi-platform.yml/badge.svg)\
license : [![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)\
Terreate is a 3D game library for C++. This repository is designed for creating 3D games with OpenGL and OpenAL without the annoying of state based programming.
- ~~[Reference (WIP)](---)~~

# Dependencies
## Library dependence
These packages are used in this library.
- [glfw](https://www.glfw.org/docs/latest/)
  > GLFW is a free, Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan application development. It provides a simple, platform-independent API for creating windows, contexts and surfaces, reading input, handling events, etc.\
  *from glfw website*
- [glad](https://glad.dav1d.de/)
  > Multi-Language GL/GLES/EGL/GLX/WGL Loader-Generator based on the official specs.\
  *from glad website*
- [OpenAL-soft](https://github.com/kcat/openal-soft.git)
  > OpenAL Soft is an LGPL-licensed, cross-platform, software implementation of the OpenAL 3D audio API. It's forked from the open-sourced Windows version available originally from openal.org's SVN repository (now defunct). OpenAL provides capabilities for playing audio in a virtual 3D environment. Distance attenuation, doppler shift, and directional sound emitters are among the features handled by the API. More advanced effects, including air absorption, occlusion, and environmental reverb, are available through the EFX extension. It also facilitates streaming audio, multi-channel buffers, and audio capture.\
  *from OpenAL-soft README.md*

## Build dependence
This library uses `cmake` to build the projects so make sure to install `cmake` before building this library. 

## X11/Wayland dependence(*in Linux*)
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

Note:
> If you are using `nix` package manager and `direnv`, dependence installing is automatically executed.

# How to build
**Make sure you installed [cmake](https://cmake.org/) before building.**
In this example, build outputs are stored in `<REPOSITRY ROOT DIR>/build/` folder.\
(If you don't want to build tests, use `cmake -S . -B build -DBUILD_TERREATE_TEST=off` instead of `cmake -S . -B build` command.)

## NixOS(or `nix` installed environment)
### with `direnv`
```shell
cd <PATH TO CLONED/UNZIPPED REPOSITORY>
cmake -S . -B build
cmake --build build
```

### without `direnv`
```shell
cd <PATH TO CLONED/UNZIPPED REPOSITORY>
nix develop
cmake -S . -B build
cmake --build build
```

## Linux
**Make sure you installed [dependencies](#x11wayland-dependence) before building.**
```shell
cd <PATH TO CLONED/UNZIPPED REPOSITORY>
cmake -S . -B build
cmake --build build
```

## Windows
```shell
cd <PATH TO CLONED/UNZIPPED REPOSITORY>
cmake -S . -B build
cmake --build build
```

## MacOS
***Currently, MacOS is not officially supported.***
```shell
cd <PATH TO CLONED/UNZIPPED REPOSITORY>
cmake -S . -B build
cmake --build build
```

# Licensing
[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)\
This repository is provided under the **BSD-3-CLAUSE** license. So you can use this library whatever you want.\
You don't have to contact me to use.

# Contact
If you want to contact me, please send an e-mail to `upiscium@gmail.com`.
