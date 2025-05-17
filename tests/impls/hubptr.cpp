// #include <hubptr.hpp>

// namespace Terreate::Util::Pointer {

// namespace Wrapper {
// PointerWrapperBase::PointerWrapperBase() {}
// PointerWrapperBase::~PointerWrapperBase() {}

// PointerWrapper::PointerWrapper() : mPtr(nullptr) {}
// PointerWrapper::PointerWrapper(PointerWrapper &&other) : mPtr(other.mPtr) {
//   other.mPtr = nullptr;
// }
// PointerWrapper::~PointerWrapper() {}

// void PointerWrapper::dispose() {
//   if (mPtr) {
//     mPtr->dispose();
//     delete mPtr;
//     mPtr = nullptr;
//   }
// }

// PointerWrapper &PointerWrapper::operator=(PointerWrapper &&other) {
//   if (this != &other) {
//     this->dispose();
//     mPtr = other.mPtr;
//     other.mPtr = nullptr;
//   }
//   return *this;
// }

// PointerWrapper::operator bool() const { return mPtr != nullptr; }

// } // namespace Wrapper

// } // namespace Terreate::Util::Pointer
