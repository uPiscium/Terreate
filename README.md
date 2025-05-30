# Terreate
license : [![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)\
Terreate is a 3D game library for C++. This repository is designed for creating 3D games with Vulkan and OpenAL.
> [!NOTE]
> `Terreate` has its own wiki page that provides more details about, including a quick start guide, API definitions, and more!
> - [Terreate wiki](https://upiscium.github.io/Terreate-Wiki/)

# Dependencies
## Library dependence
These packages are used in this library.
- [Vulkan](https://www.vulkan.org/)
  > Vulkan is not a company, nor language, but rather a way for developers to program their modern GPU hardware in a cross-platform and cross-vendor fashion. The Khronos Group is a member-driven consortium that created and maintains Vulkan.\
  *from Vulkan documentation*
- [glfw](https://www.glfw.org/docs/latest/)
  > GLFW is a free, Open Source, multi-platform library for OpenGL, OpenGL ES and Vulkan application development. It provides a simple, platform-independent API for creating windows, contexts and surfaces, reading input, handling events, etc.\
  *from glfw website*
- [OpenAL-soft](https://github.com/kcat/openal-soft.git)
  > OpenAL Soft is an LGPL-licensed, cross-platform, software implementation of the OpenAL 3D audio API. It's forked from the open-sourced Windows version available originally from openal.org's SVN repository (now defunct). OpenAL provides capabilities for playing audio in a virtual 3D environment. Distance attenuation, doppler shift, and directional sound emitters are among the features handled by the API. More advanced effects, including air absorption, occlusion, and environmental reverb, are available through the EFX extension. It also facilitates streaming audio, multi-channel buffers, and audio capture.\
  *from OpenAL-soft README.md*

# Building project
**Make sure you installed [cmake](https://cmake.org/) before building.**

## Build outputs
In this example, build outputs are stored in `<REPOSITRY ROOT DIR>/build/` folder.\

## `Cmake` options
### `TERREATE_BUILD_TEST`
Default: ON

Specifies whether to build test code.

### `TERREATE_DEBUG_BUILD`
Default: OFF

Specifies whether to build in debug mode. In debug mode, `TERREATE_DEBUG_MODE` is defined.

## Windows & Linux
**Make sure you installed dependencies before building.**
```shell
cmake -S . -B build
cmake --build build
```

## `nix` installed env
```shell
nix develop // or `direnv allow` if you have direnv.
cmake -S . -B build
cmake --build build
```

## Link
- [*Prev - Dependencies*](./deps.md)
- [*Next - Create window*](../guide/window.md)

# Licensing
[![License](https://img.shields.io/badge/License-BSD_3--Clause-blue.svg)](https://opensource.org/licenses/BSD-3-Clause)\
This repository is provided under the **BSD-3-CLAUSE** license. So you can use this library whatever you want.

# Contact
If you want to contact me, please send an e-mail to `upiscium@gmail.com`.
