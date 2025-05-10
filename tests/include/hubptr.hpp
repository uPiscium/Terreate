#pragma once
#include <exception.hpp>
#include <type.hpp>

namespace Terreate::Util::Pointer {
using namespace Terreate::Type;

/**
 * @namespace TypeTraits
 * @brief Type traits for pointer types to handle concrete and void types
 * uniformly
 */
namespace TypeTraits {
/**
 * @brief Generic pointer traits for any type
 * @tparam S The type for which to define pointer traits
 */
template <typename S> struct PointerTraits {
  typedef S *Ptr;        ///< Raw pointer type
  typedef S const *CPtr; ///< Const pointer type
  typedef S &Ref;        ///< Reference type
  typedef S const &CRef; ///< Const reference type
};

/**
 * @brief Specialization of pointer traits for void
 * @details Special handling since void& is not a valid C++ type
 */
template <> struct PointerTraits<void> {
  typedef void *Ptr;        ///< Raw pointer type
  typedef void const *CPtr; ///< Const pointer type
  typedef void Ref;         ///< Reference type (not actually usable)
  typedef void const CRef;  ///< Const reference type (not actually usable)
};
} // namespace TypeTraits

/**
 * @namespace Wrapper
 * @brief Contains pointer wrapper implementation for memory management
 */
namespace Wrapper {
/**
 * @brief Abstract base class for pointer wrappers
 */
class PointerWrapperBase {
private:
  PROHIBIT_COPY_AND_ASSIGN(PointerWrapperBase); ///< Prevent copying

public:
  PointerWrapperBase();
  virtual ~PointerWrapperBase();

  /**
   * @brief Release and clean up the managed resource
   */
  virtual void dispose() = 0;

  /**
   * @brief Check if wrapper contains a valid pointer
   * @return true if pointer is not null, false otherwise
   */
  virtual operator bool() const = 0;
};

/**
 * @brief Implementation of pointer wrapper for standard delete operation
 * @tparam T Type of the managed object
 */
template <typename T> class PointerWrapperImpl : public PointerWrapperBase {
private:
  PROHIBIT_COPY_AND_ASSIGN(PointerWrapperImpl);
  using Traits = TypeTraits::PointerTraits<T>;
  typedef Traits::Ptr Ptr; ///< Raw pointer type

private:
  Ptr mPtr; ///< The managed pointer

public:
  /**
   * @brief Constructor
   * @param ptr Pointer to manage
   */
  PointerWrapperImpl(Ptr ptr) : mPtr(ptr) {}

  /**
   * @brief Move constructor
   * @param other Source wrapper to move from
   */
  PointerWrapperImpl(PointerWrapperImpl &&other) : mPtr(other.mPtr) {
    other.mPtr = nullptr;
  }

  /**
   * @brief Destructor
   */
  ~PointerWrapperImpl() override {}

  /**
   * @brief Release and delete the managed pointer
   */
  void dispose() override {
    if (mPtr) {
      delete mPtr;
      mPtr = nullptr;
    }
  }

  /**
   * @brief Move assignment operator
   * @param other Source wrapper to move from
   * @return Reference to this wrapper
   */
  PointerWrapperImpl &operator=(PointerWrapperImpl &&other) {
    if (this != &other) {
      this->dispose();
      mPtr = other.mPtr;
      other.mPtr = nullptr;
    }
    return *this;
  }

  /**
   * @brief Check if wrapper contains a valid pointer
   * @return true if pointer is not null, false otherwise
   */
  operator bool() const override { return mPtr != nullptr; }
};

/**
 * @brief Implementation of pointer wrapper with custom deleter
 * @tparam T Type of the managed object
 * @tparam DeleterT Type of the custom deleter
 */
template <typename T, typename DeleterT>
class PointerWrapperImplDeleter : public PointerWrapperBase {
private:
  PROHIBIT_COPY_AND_ASSIGN(PointerWrapperImplDeleter);
  using Traits = TypeTraits::PointerTraits<T>;
  typedef Traits::Ptr Ptr; ///< Raw pointer type

private:
  Ptr mPtr;          ///< The managed pointer
  DeleterT mDeleter; ///< The custom deleter function/functor

public:
  /**
   * @brief Constructor with custom deleter
   * @param ptr Pointer to manage
   * @param deleter Custom deleter to use for cleanup
   */
  PointerWrapperImplDeleter(Ptr ptr, DeleterT deleter)
      : mPtr(ptr), mDeleter(deleter) {}

  /**
   * @brief Move constructor
   * @param other Source wrapper to move from
   */
  PointerWrapperImplDeleter(PointerWrapperImplDeleter &&other)
      : mPtr(other.mPtr), mDeleter(std::move(other.mDeleter)) {
    other.mPtr = nullptr;
  }

  /**
   * @brief Destructor
   */
  ~PointerWrapperImplDeleter() override {}

  /**
   * @brief Release and clean up the managed pointer using the custom deleter
   */
  void dispose() override {
    if (mPtr) {
      mDeleter(mPtr);
      mPtr = nullptr;
    }
  }

  /**
   * @brief Move assignment operator
   * @param other Source wrapper to move from
   * @return Reference to this wrapper
   */
  PointerWrapperImplDeleter &operator=(PointerWrapperImplDeleter &&other) {
    if (this != &other) {
      this->dispose();
      mPtr = other.mPtr;
      mDeleter = std::move(other.mDeleter);
      other.mPtr = nullptr;
    }
    return *this;
  }

  /**
   * @brief Check if wrapper contains a valid pointer
   * @return true if pointer is not null, false otherwise
   */
  operator bool() const override { return mPtr != nullptr; }
};

/**
 * @brief Type-erased wrapper for managing any pointer type
 */
class PointerWrapper {
private:
  PROHIBIT_COPY_AND_ASSIGN(PointerWrapper); ///< Prevent copying

private:
  PointerWrapperBase *mPtr; ///< The underlying implementation pointer

public:
  PointerWrapper();
  PointerWrapper(PointerWrapper &&other);
  ~PointerWrapper();

  /**
   * @brief Constructor for standard pointer types
   * @tparam T Type of the managed object
   * @param ptr Pointer to manage
   */
  template <typename T> PointerWrapper(T *ptr) {
    mPtr = new PointerWrapperImpl<T>(ptr);
  }

  /**
   * @brief Constructor for pointer types with custom deleter
   * @tparam T Type of the managed object
   * @tparam DeleterT Type of the custom deleter
   * @param ptr Pointer to manage
   * @param deleter Custom deleter to use for cleanup
   */
  template <typename T, typename DeleterT>
  PointerWrapper(T *ptr, DeleterT deleter) {
    mPtr = new PointerWrapperImplDeleter<T, DeleterT>(ptr, deleter);
  }

  /**
   * @brief Release and clean up the managed resource
   */
  void dispose();

  /**
   * @brief Move assignment operator
   * @param other Source wrapper to move from
   * @return Reference to this wrapper
   */
  PointerWrapper &operator=(PointerWrapper &&other);

  /**
   * @brief Check if wrapper contains a valid pointer
   * @return true if pointer is not null, false otherwise
   */
  operator bool() const;
};
} // namespace Wrapper

/**
 * @brief Non-owning smart pointer, safe reference to an object
 * @tparam T Type of the referenced object
 */
template <typename T> class RefPointer {
private:
  using Traits = TypeTraits::PointerTraits<T>;
  typedef Traits::Ptr Ptr;   // T*
  typedef Traits::CPtr CPtr; // const T*
  typedef Traits::Ref Ref;   // T&
  typedef Traits::CRef CRef; // const T&

  /// Allow other RefPointer specializations to access private members
  template <typename U> friend class RefPointer;

private:
  /// Delete operations that could lead to dangling pointers
  RefPointer &operator=(Ptr) = delete;
  RefPointer &operator=(CPtr) = delete;
  RefPointer &operator=(Ref) = delete;
  RefPointer &operator=(CRef) = delete;

private:
  Ptr mPtr = nullptr; ///< The referenced pointer (not owned)

public:
  /**
   * @brief Default constructor
   */
  RefPointer() : mPtr(nullptr) {}

  /**
   * @brief Constructor from raw pointer
   * @param ptr Pointer to reference (not owned)
   */
  RefPointer(Ptr ptr) : mPtr(ptr) {}

  /**
   * @brief Copy constructor
   * @param other Source pointer to copy from
   */
  RefPointer(RefPointer const &other) : mPtr(other.mPtr) {}

  /**
   * @brief Destructor
   */
  ~RefPointer() {}

  /**
   * @brief Dereference operator
   * @return Reference to the managed object
   * @throws NullReferenceException if the pointer is null
   */
  Ref operator*() {
    if (!mPtr) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return *mPtr;
  }

  /**
   * @brief Const dereference operator
   * @return Const reference to the managed object
   * @throws NullReferenceException if the pointer is null
   */
  CRef operator*() const {
    if (!mPtr) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return *mPtr;
  }

  /**
   * @brief Arrow operator for member access
   * @return Pointer to the managed object
   * @throws NullReferenceException if the pointer is null
   */
  Ptr operator->() {
    if (!mPtr) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return mPtr;
  }

  /**
   * @brief Const arrow operator for member access
   * @return Const pointer to the managed object
   * @throws NullReferenceException if the pointer is null
   */
  CPtr operator->() const {
    if (!mPtr) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return mPtr;
  }

  /**
   * @brief Copy assignment operator
   * @param other Source pointer to copy from
   * @return Reference to this pointer
   */
  RefPointer &operator=(RefPointer const &other) {
    if (this != &other) {
      mPtr = other.mPtr;
    }
    return *this;
  }

  /**
   * @brief Template copy assignment operator for compatible types
   * @tparam U Type compatible with T
   * @param other Source pointer to copy from
   * @return Reference to this pointer
   */
  template <typename U> RefPointer &operator=(RefPointer<U> const &other) {
    if (this != &other) {
      mPtr = other.mPtr;
    }
    return *this;
  }
};

/**
 * @brief Owning smart pointer with automatic resource management
 * @tparam T Type of the managed object
 */
template <typename T> class HubPointer {
private:
  PROHIBIT_COPY_AND_ASSIGN(HubPointer); ///< Prevent copying
  using Traits = TypeTraits::PointerTraits<T>;
  typedef Traits::Ptr Ptr;   // T*
  typedef Traits::CPtr CPtr; // const T*
  typedef Traits::Ref Ref;   // T&
  typedef Traits::CRef CRef; // const T&

  /// Allow other HubPointer specializations to access private members
  template <typename U> friend class HubPointer;

private:
  Ptr mPtr = nullptr;               ///< The managed pointer
  Wrapper::PointerWrapper mWrapper; ///< Wrapper handling memory management

public:
  /**
   * @brief Default constructor
   */
  HubPointer() : mPtr(nullptr) {}

  /**
   * @brief Constructor from nullptr
   * @param nullptr_t nullptr value
   */
  HubPointer(std::nullptr_t) : mPtr(nullptr) {}

  /**
   * @brief Move constructor
   * @param other Source pointer to move from
   */
  HubPointer(HubPointer &&other)
      : mPtr(other.mPtr), mWrapper(std::move(other.mWrapper)) {
    other.mPtr = nullptr;
  }

  /**
   * @brief Constructor from raw pointer
   * @tparam U Type compatible with T
   * @param ptr Pointer to take ownership of
   */
  template <typename U> HubPointer(U *ptr) : mPtr(ptr) {
    mWrapper = Wrapper::PointerWrapper(ptr);
  }

  /**
   * @brief Constructor from raw pointer with custom deleter
   * @tparam U Type compatible with T
   * @tparam DeleterT Type of the custom deleter
   * @param ptr Pointer to take ownership of
   * @param deleter Custom deleter to use for cleanup
   */
  template <typename U, typename DeleterT>
  HubPointer(U *ptr, DeleterT deleter) : mPtr(ptr) {
    mWrapper = Wrapper::PointerWrapper(ptr, deleter);
  }

  /**
   * @brief Move constructor from compatible HubPointer
   * @tparam U Type compatible with T
   * @param ptr Source pointer to move from
   */
  template <typename U> HubPointer(HubPointer<U> &&ptr) : mPtr(ptr.mPtr) {
    mWrapper = std::move(ptr.mWrapper);
    ptr.mPtr = nullptr;
  }

  /**
   * @brief Destructor, automatically calls dispose()
   */
  ~HubPointer() { this->dispose(); }

  /**
   * @brief Creates a non-owning reference pointer to the managed object
   * @return RefPointer that safely references the current object
   */
  RefPointer<T> ref() { return RefPointer<T>(mPtr); }

  /**
   * @brief Release and clean up the managed resource
   */
  void dispose() { mWrapper.dispose(); }

  /**
   * @brief Dereference operator
   * @return Reference to the managed object
   * @throws NullReferenceException if the pointer is null
   */
  Ref operator*() {
    if (!mPtr) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return *mPtr;
  }

  /**
   * @brief Const dereference operator
   * @return Const reference to the managed object
   * @throws NullReferenceException if the pointer is null
   */
  CRef operator*() const {
    if (!mPtr) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return *mPtr;
  }

  /**
   * @brief Arrow operator for member access
   * @return Pointer to the managed object
   * @throws NullReferenceException if the pointer is null
   */
  Ptr operator->() {
    if (!mPtr) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return mPtr;
  }

  /**
   * @brief Const arrow operator for member access
   * @return Const pointer to the managed object
   * @throws NullReferenceException if the pointer is null
   */
  CPtr operator->() const {
    if (!mPtr) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return mPtr;
  }

  /**
   * @brief Move assignment operator from raw pointer
   * @param ptr Pointer to take ownership of
   * @return Reference to this pointer
   */
  HubPointer &operator=(Ptr &&ptr) {
    if (this != &ptr) {
      this->dispose();
      mPtr = ptr;
      mWrapper = Wrapper::PointerWrapper(ptr);
    }
    return *this;
  }

  /**
   * @brief Move assignment operator
   * @param other Source pointer to move from
   * @return Reference to this pointer
   */
  HubPointer &operator=(HubPointer &&other) {
    if (this != &other) {
      this->dispose();
      mPtr = other.mPtr;
      mWrapper = std::move(other.mWrapper);
      other.mPtr = nullptr;
    }
    return *this;
  }

  /**
   * @brief Boolean conversion operator
   * @return true if pointer is not null, false otherwise
   */
  operator bool() const { return mPtr != nullptr; }

public:
  template <typename... Args> static HubPointer create(Args &&...args) {
    return HubPointer(new T(std::forward<Args>(args)...));
  }
};

} // namespace Terreate::Util::Pointer
