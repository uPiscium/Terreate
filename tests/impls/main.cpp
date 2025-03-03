#include <TerreateTest.hpp>
#include <audiotest.hpp>
#include <coretest.hpp>
#include <exceptions.hpp>
#include <graphictest.hpp>

#include <iostream>

namespace Core = Terreate::Test::Core;
namespace Audio = Terreate::Test::Audio;
namespace Graphic = Terreate::Test::Graphic;

int main() {
#ifdef BUILD_CORE_TESTS // Default: OFF
  // Core tests
  Core::TestEvent();
  Core::TestExecutor();
  Core::TestNullable();
  Core::TestObject();
  Core::TestProperty();
  Core::TestUUID();
#endif // BUILD_CORE_TESTS

#ifdef BUILD_AUDIO_TESTS // Default: OFF
  // Audio tests
  Audio::TestAudio();
#endif // BUILD_AUDIO_TESTS

#ifdef BUILD_GRAPHIC_TESTS // Default: ON
  // Graphic tests
  /* std::thread graphicThread(Graphic::Run); */
  /* graphicThread.join(); */
#endif // BUILD_GRAPHIC_TESTS

  Terreate::Test::Run();
}
