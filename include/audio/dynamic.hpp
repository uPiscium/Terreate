#ifndef __TERREATE_AUDIO_DYNAMIC_HPP__
#define __TERREATE_AUDIO_DYNAMIC_HPP__

#include <types.hpp>

namespace Terreate::Audio::Dynamic {
using namespace Terreate::Types;
#ifdef _WIN32
#include <Windows.h>

typedef HMODULE Module;
inline Module LoadDynamic(Str const &filename) {
  return LoadLibrary(filename.c_str());
}
inline void FreeDynamic(Module module) { FreeLibrary(module); }
template <typename T>
inline T ExtractFunction(Module module, Str const &function) {
  return reinterpret_cast<T>(GetProcAddress(module, function.c_str()));
}

static Str const OPENAL_LIB_PATH = "openal32.dll";
#else
#include <dlfcn.h>

typedef void *Module;
inline Module LoadDynamic(Str const &filename) {
  return dlopen(filename.c_str(), RTLD_LAZY);
}
inline void FreeDynamic(Module module) { dlclose(module); }
template <typename T>
inline T ExtractFunction(Module module, Str const &function) {
  return reinterpret_cast<T>(dlsym(module, function.c_str()));
}

static Str const OPENAL_LIB_PATH = "libopenal.so";
#endif

extern Module ALModule;
} // namespace Terreate::Audio::Dynamic

#endif // __TERREATE_AUDIO_DYNAMIC_HPP__
