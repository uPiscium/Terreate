#include <font.hpp>

namespace Terreate {
void Font::LoadFont(Str const &path, Uint const &size) {
  std::lock_guard<Mutex> lock(mCharMutex);
  mFont.LoadFont(path, size);
}
} // namespace Terreate
