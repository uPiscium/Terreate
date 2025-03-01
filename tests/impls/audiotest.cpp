#include <audiotest.hpp>
#include <iostream>

namespace Terreate::Test::Audio {
Vec<Short> GenerateSineWave(Uint const sampleRate, Uint const Hz) {
  Vec<Short> data(sampleRate);
  Float duration = 1.0f;
  for (Uint i = 0; i < sampleRate; ++i, duration *= 0.99999f) {
    data[i] = duration * (std::pow(2, 15) - 1) *
              std::sin(2.0f * 3.14159f * Hz * i / sampleRate);
  }
  std::cout << "Generated sine wave with " << Hz << " Hz" << std::endl;
  return data;
}

void TestAudio() {
  std::cout << ">>>>>>>>>>>>>>>>>>>> TestAudio" << std::endl;
  Initialize();
  std::cout << "Initialized audio" << std::endl;
  {
    AudioContext context;
    AudioStream stream;
    Uint idx = 0;
    Vec<AudioBuffer> buffers(4);
    std::cout << "Created audio stream" << std::endl;

    Uint const sampleRate = 44100;
    Uint Hz = 440;
    Uint h = 0;
    for (Uint i = 0; i < buffers.size(); ++i, ++h) {
      Vec<Short> data =
          GenerateSineWave(sampleRate, Hz * std::pow(2, h / 12.0));
      buffers[i].SetBuffer(data, sampleRate);
      stream.QueueBuffer(buffers[i]);
      std::cout << "Queued buffer: " << i << std::endl;
    }

    std::cout << "Starting stream" << std::endl;
    stream.Play();

    while (h < 13) {
      Uint processed = stream.GetNumProcessedBuffers();
      if (processed == 0) {
        continue;
      }
      std::cout << "Processed buffers: " << processed << std::endl;
      std::cout << "Current buffer index: " << idx << std::endl;
      for (Uint i = 0; i < processed; ++i, ++h) {
        if (h > 12) {
          break;
        }
        stream.UnqueueBuffer(buffers[idx + i]);
        std::cout << "Unqueued buffer: " << idx + i << std::endl;
        Vec<Short> data =
            GenerateSineWave(sampleRate, Hz * std::pow(2, h / 12.0));
        buffers[idx + i].SetBuffer(data, sampleRate);
        stream.QueueBuffer(buffers[idx + i]);
        std::cout << "Queued buffer: " << idx + i << std::endl;
      }
      idx += processed;
      idx %= buffers.size();
    }

    std::cout << "Waiting for stream to finish" << std::endl;
    while (stream.IsPlaying()) {
    }

    for (Uint i = 0; i < buffers.size(); ++i, ++h) {
      stream.UnqueueBuffer(buffers[i]);
      std::cout << "Unqueued buffer: " << i << std::endl;
    }

    stream.Stop();
    std::cout << "Stopped stream" << std::endl;
  }
  Terminate();
  std::cout << "<<Audio test done>>" << std::endl;
}

} // namespace Terreate::Test::Audio
