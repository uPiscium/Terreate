#include <pointer.hpp>

namespace Terreate::Util::Pointer {

namespace Wrapper {
Pointer::Pointer(Pointer &&other) : mPointer(other.mPointer) {
  other.mPointer = nullptr;
}

Pointer::~Pointer() {
  if (mPointer) {
    mPointer->dispose();
    delete mPointer;
    mPointer = nullptr;
  }
}

void Pointer::dispose() {
  if (mPointer) {
    mPointer->dispose();
    delete mPointer;
    mPointer = nullptr;
  }
}

Pointer::operator bool() const { return mPointer != nullptr; }

} // namespace Wrapper

} // namespace Terreate::Util::Pointer
