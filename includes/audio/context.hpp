#ifndef __TERREATEAUDIO_CONTEXT_HPP__
#define __TERREATEAUDIO_CONTEXT_HPP__

#include <audio/ALdefs.hpp>
#include <audio/ALhelper.hpp>
#include <types.hpp>

namespace Terreate::Audio {
using namespace Terreate::Types;
using namespace Terreate::Audio::ALhelper;

class AudioContext {
private:
  Context *mContext = nullptr;
  Device *mDevice = nullptr;

public:
  AudioContext();
  ~AudioContext();

  void SetCurrent() const { alcMakeContextCurrent(mContext); }
};
} // namespace Terreate::Audio

#endif // __TERREATEAUDIO_CONTEXT_HPP__
