#include <resource.hpp>

namespace Terreate::ResourceUtils {
SharedCount::SharedCount(SharedCount const &other) : mCount(other.mCount) {
  if (mCount) {
    mCount->Increment();
  }
}

SharedCount::~SharedCount() {
  if (mCount) {
    mCount->Decrement();
  }
}

void SharedCount::Delete() {
  if (mCount) {
    mCount->Dispose();
    delete mCount;
    mCount = nullptr;
  }
}

SharedCount &SharedCount::operator=(SharedCount const &other) {
  auto *temp = other.mCount;
  if (temp == mCount) {
    return *this;
  }

  if (temp) {
    temp->Increment();
  }
  if (mCount) {
    mCount->Decrement();
  }
  mCount = temp;

  return *this;
}

} // namespace Terreate::ResourceUtils
