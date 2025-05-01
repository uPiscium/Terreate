#include <resource.hpp>

namespace Terreate::Core::ResourceUtils {

CounterBase::CounterBase() : mCount(0u) {}

CounterBase::CounterBase(u64 const &count) : mCount(count) {}

CounterBase::~CounterBase() {}

u64 const &CounterBase::GetCount() const { return mCount; }

void CounterBase::Increment() { ++mCount; }

void CounterBase::Decrement() { --mCount; }

SharedCount::SharedCount() : mCount(nullptr) {}

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

u64 const &SharedCount::GetCount() const { return mCount->GetCount(); }

void SharedCount::Delete() {
  if (mCount) {
    CounterBase *temp = mCount;
    mCount = nullptr;
    temp->Dispose();
    delete mCount;
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

SharedCount::operator bool() const { return mCount ? (bool)(*mCount) : false; }

} // namespace Terreate::Core::ResourceUtils
