#pragma once
#include <exception.hpp>
#include <type.hpp>

namespace Terreate::Util::Pointer {
using namespace Terreate::Type;

namespace TypeTraits {
template <typename S> struct PointerTraits {
  typedef S *Ptr;
  typedef S const *CPtr;
  typedef S &Ref;
  typedef S const &CRef;
};

template <> struct PointerTraits<void> {
  typedef void *Ptr;
  typedef void const *CPtr;
  typedef void Ref;
  typedef void const CRef;
};
} // namespace TypeTraits

namespace Wrapper {
/**
 * @brief Base class for all pointer implementations in the system.
 *
 * PointerBase serves as an abstract interface for different pointer
 * implementations. It defines the core operations that all pointer
 * types must support.
 */
class PointerBase {
private:
  // Prevents copying of this class.
  PROHIBIT_COPY_AND_ASSIGN(PointerBase);

public:
  /** @brief Default constructor. */
  PointerBase() = default;

  /**
   * @brief Virtual destructor to ensure proper cleanup in derived classes.
   */
  virtual ~PointerBase() = default;

  /**
   * @brief Releases resources managed by this pointer.
   *
   * Implementations should free any resources and reset the internal state.
   */
  virtual void dispose() = 0;

  /**
   * @brief Checks if the pointer contains a valid (non-null) reference.
   *
   * @return true if the pointer refers to a valid object, false otherwise.
   */
  virtual operator bool() const = 0;
};

/**
 * @brief Standard pointer implementation with default delete behavior.
 *
 * @tparam T The type of object being pointed to.
 */
template <typename T> class PointerImpl : public PointerBase {
private:
  // Prevents copying of this class.
  PROHIBIT_COPY_AND_ASSIGN(PointerImpl);

private:
  // The raw pointer being managed.
  T *mPointer = nullptr;

public:
  // Default constructor.
  PointerImpl() = default;

  /**
   * @brief Constructs a pointer implementation wrapping the given raw pointer.
   * @param pointer The raw pointer to manage.
   */
  PointerImpl(T *pointer) : mPointer(pointer) {}

  /**
   * @brief Move constructor.
   * @param other The pointer implementation to move from.
   *
   * Takes ownership of the pointer from other and nullifies other.
   */
  PointerImpl(PointerImpl &&other) : mPointer(other.mPointer) {
    other.mPointer = nullptr;
  }

  // Virtual destructor required for proper inheritance.
  ~PointerImpl() override {}

  /**
   * @brief Deletes the managed object and resets the pointer.
   *
   * Uses the delete operator which calls the destructor of the pointed object.
   */
  void dispose() override {
    if (mPointer) {
      delete mPointer;
      mPointer = nullptr;
    }
  }

  /**
   * @brief Checks if the pointer is non-null.
   * @return true if the pointer is valid, false otherwise.
   */
  operator bool() const override { return mPointer != nullptr; }
};

/**
 * @brief Pointer implementation with custom deletion behavior.
 *
 * @tparam T The type of object being pointed to.
 * @tparam Deleter A functor type that defines how to delete the object.
 */
template <typename T, typename Deleter>
class PointerImplDeleter : public PointerBase {
private:
  // Prevents copying of this class.
  PROHIBIT_COPY_AND_ASSIGN(PointerImplDeleter);

private:
  // The raw pointer being managed.
  T *mPointer = nullptr;
  // Custom deleter function/functor.
  Deleter mDeleter;

public:
  // Default constructor.
  PointerImplDeleter() = default;

  /**
   * @brief Constructs a deleter-based pointer implementation.
   * @param pointer The raw pointer to manage.
   * @param deleter The custom deleter function/functor.
   */
  PointerImplDeleter(T *pointer, Deleter deleter)
      : mPointer(pointer), mDeleter(deleter) {}

  /**
   * @brief Move constructor.
   * @param other The pointer implementation to move from.
   *
   * Takes ownership of the pointer and deleter from other and nullifies other.
   */
  PointerImplDeleter(PointerImplDeleter &&other)
      : mPointer(other.mPointer), mDeleter(other.mDeleter) {
    other.mPointer = nullptr;
  }

  // Virtual destructor required for proper inheritance.
  ~PointerImplDeleter() override {}

  /**
   * @brief Deletes the managed object using the custom deleter and resets the
   * pointer.
   */
  void dispose() override {
    if (mPointer) {
      mDeleter(mPointer);
      mPointer = nullptr;
    }
  }

  /**
   * @brief Checks if the pointer is non-null.
   * @return true if the pointer is valid, false otherwise.
   */
  operator bool() const override { return mPointer != nullptr; }
};

/**
 * @brief Type-erased smart pointer implementation.
 *
 * Provides a common interface to different pointer implementation strategies.
 */
class Pointer {
private:
  // Prevents copying of this class.
  PROHIBIT_COPY_AND_ASSIGN(Pointer);

private:
  // The underlying pointer implementation.
  PointerBase *mPointer = nullptr;

public:
  /** Default constructor creates a null pointer. */
  Pointer() = default;

  /**
   * @brief Constructs a pointer with standard deletion behavior.
   * @tparam T The type of object being pointed to.
   * @param pointer The raw pointer to manage.
   */
  template <typename T> Pointer(T *pointer) {
    mPointer = new PointerImpl<T>(pointer);
  }

  /**
   * @brief Constructs a pointer with custom deletion behavior.
   * @tparam T The type of object being pointed to.
   * @tparam Deleter The type of the custom deleter.
   * @param pointer The raw pointer to manage.
   * @param deleter The custom deleter function/functor.
   */
  template <typename T, typename Deleter> Pointer(T *pointer, Deleter deleter) {
    mPointer = new PointerImplDeleter<T, Deleter>(pointer, deleter);
  }

  /**
   * @brief Move constructor.
   * @param other The pointer to move from.
   */
  Pointer(Pointer &&other);

  /**
   * @brief Destructor that releases the managed pointer.
   */
  ~Pointer();

  /**
   * @brief Releases resources managed by this pointer.
   */
  void dispose();

  /**
   * @brief Checks if the pointer contains a valid (non-null) reference.
   * @return true if the pointer refers to a valid object, false otherwise.
   */
  operator bool() const;

  /**
   * @brief Move assignment operator.
   * @param other The pointer to move from.
   * @return Reference to this pointer after assignment.
   *
   * Releases any resources managed by this pointer, then takes ownership of
   * the implementation from the other pointer, and nullifies the other pointer.
   */
  Pointer &operator=(Pointer &&other) {
    if (this != &other) {
      this->dispose();
      mPointer = other.mPointer;
      other.mPointer = nullptr;
    }
    return *this;
  }
};

} // namespace Wrapper

template <typename T> class ReferencePointer {
private:
  // Type aliases for convenience and readability
  typedef Wrapper::Pointer Pointer;
  typedef TypeTraits::PointerTraits<T> Traits;
  typedef Traits::Ptr Ptr;   // T*
  typedef Traits::CPtr CPtr; // const T*
  typedef Traits::Ref Ref;   // T&
  typedef Traits::CRef CRef; // const T&

private:
  // Prevent direct assignment from raw pointer to maintain safety
  ReferencePointer &operator=(Ptr pointer) = delete;

private:
  // The raw pointer being referenced - this class doesn't own the memory
  // and won't delete it when destructed
  Ptr mRawPointer = nullptr;

public:
  /** @brief Default constructor creates a null pointer. */
  ReferencePointer() : mRawPointer(nullptr) {}

  /**
   * @brief Constructs a null pointer.
   * @param nullptr_t Standard nullptr parameter.
   */
  ReferencePointer(std::nullptr_t) : mRawPointer(nullptr) {}

  /**
   * @brief Constructs a pointer that references the given raw pointer.
   * @param pointer Raw pointer to reference (ownership is not transferred).
   */
  ReferencePointer(Ptr pointer) : mRawPointer(pointer) {}

  /**
   * @brief Copy constructor.
   * @param other Another ReferencePointer to copy from.
   *
   * Creates a new reference to the same object without transferring ownership.
   */
  ReferencePointer(ReferencePointer const &other)
      : mRawPointer(other.mRawPointer) {}

  /**
   * @brief Destructor - doesn't release memory as this is just a reference.
   */
  ~ReferencePointer() {}

  /**
   * @brief Gets the raw pointer being referenced.
   * @return The raw pointer.
   */
  Ptr get() { return mRawPointer; }

  /**
   * @brief Gets the raw pointer being referenced (const version).
   * @return The const raw pointer.
   */
  CPtr get() const { return mRawPointer; }

  /**
   * @brief Dereference operator.
   * @return Reference to the referenced object.
   * @throws NullReferenceException if the pointer is null.
   *
   * Allows accessing the referenced object with the * operator.
   */
  Ref operator*() {
    if (!mRawPointer) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return *mRawPointer;
  }

  /**
   * @brief Const dereference operator.
   * @return Const reference to the referenced object.
   * @throws NullReferenceException if the pointer is null.
   *
   * Allows accessing the referenced object with the * operator in const
   * contexts.
   */
  CRef operator*() const {
    if (!mRawPointer) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return *mRawPointer;
  }

  /**
   * @brief Member access operator.
   * @return Raw pointer to the referenced object.
   * @throws NullReferenceException if the pointer is null.
   *
   * Allows access to members of the referenced object with the -> operator.
   */
  Ptr operator->() {
    if (!mRawPointer) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return mRawPointer;
  }

  /**
   * @brief Const member access operator.
   * @return Const raw pointer to the referenced object.
   * @throws NullReferenceException if the pointer is null.
   *
   * Allows access to members of the referenced object with the -> operator
   * in const contexts.
   */
  CPtr operator->() const {
    if (!mRawPointer) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return mRawPointer;
  }

  /**
   * @brief Assignment operator to update the referenced pointer.
   * @param other Another ReferencePointer to copy the reference from.
   * @return Reference to this object.
   *
   * Assigns this pointer to reference the same object as 'other'.
   */
  ReferencePointer &operator=(ReferencePointer const &other) {
    if (this != &other) {
      mRawPointer = other.mRawPointer;
    }
    return *this;
  }

  /**
   * @brief Assignment operator for nullptr.
   * @param nullptr_t Standard nullptr parameter.
   * @return Reference to this object.
   *
   * Clears this pointer to reference nothing.
   */
  ReferencePointer &operator=(std::nullptr_t) {
    mRawPointer = nullptr;
    return *this;
  }

  /**
   * @brief Boolean conversion operator.
   * @return true if the pointer is non-null, false otherwise.
   *
   * Allows using the pointer in boolean contexts like if(pointer).
   */
  operator bool() const { return mRawPointer != nullptr; }
};

/**
 * @brief MasterPointer is a smart pointer that manages the lifetime of an
 * object.
 *
 * It provides a convenient interface for accessing and manipulating the
 * underlying object while ensuring proper memory management.
 *
 * @tparam T The type of the object being managed.
 */
template <typename T> class MasterPointer {
private:
  // Prevents copying of this class.
  PROHIBIT_COPY_AND_ASSIGN(MasterPointer);

private:
  typedef Wrapper::Pointer Pointer;
  typedef TypeTraits::PointerTraits<T> Traits;
  typedef Traits::Ptr Ptr;
  typedef Traits::CPtr CPtr;
  typedef Traits::Ref Ref;
  typedef Traits::CRef CRef;

  template <typename U> friend class MasterPointer;

private:
  // Raw pointer to the managed object (not owned)
  T *mRawPointer = nullptr;
  // Type-erased wrapper pointer that handles memory management
  Pointer mPointer;

public:
  /** @brief Default constructor creates a null pointer. */
  MasterPointer() : mRawPointer(nullptr), mPointer() {}

  /**
   * @brief Constructs a null pointer.
   * @param nullptr_t Standard nullptr parameter.
   */
  MasterPointer(std::nullptr_t) : mRawPointer(nullptr), mPointer() {}

  /**
   * @brief Move constructor.
   * @param other The pointer to move from.
   */
  MasterPointer(MasterPointer &&other)
      : mRawPointer(other.mRawPointer), mPointer(std::move(other.mPointer)) {
    other.mRawPointer = nullptr;
  }

  /**
   * @brief Constructs a pointer that manages the given raw pointer.
   * @tparam U The type of the pointer (can be different from T for compatible
   * types).
   * @param pointer Raw pointer to manage.
   */
  template <typename U>
  MasterPointer(U *pointer) : mRawPointer(pointer), mPointer(pointer) {}

  /**
   * @brief Constructs a pointer with custom deletion behavior.
   * @tparam U The type of the pointer (can be different from T for compatible
   * types).
   * @tparam Deleter The type of the custom deleter.
   * @param pointer Raw pointer to manage.
   * @param deleter Custom deleter function/functor.
   */
  template <typename U, typename Deleter>
  MasterPointer(U *pointer, Deleter deleter)
      : mRawPointer(pointer), mPointer(pointer, deleter) {}

  /**
   * @brief Converting move constructor from compatible pointer type.
   * @tparam U The source type (must be compatible with T).
   * @param other The pointer to move from.
   */
  template <typename U>
  MasterPointer(MasterPointer<U> &&other)
      : mRawPointer(other.mRawPointer), mPointer(std::move(other.mPointer)) {
    other.mRawPointer = nullptr;
  }

  /**
   * @brief Gets the raw pointer being managed.
   * @return The raw pointer.
   */
  Ptr get() { return mRawPointer; }

  /**
   * @brief Gets the raw pointer being managed (const version).
   * @return The const raw pointer.
   */
  CPtr get() const { return mRawPointer; }

  /**
   * @brief Releases the managed object and resets the pointer.
   */
  void reset() {
    if (mPointer) {
      mPointer.dispose();
      mRawPointer = nullptr;
    }
  }

  /**
   * @brief Creates a non-owning reference to the managed object.
   *
   * This method allows creating a ReferencePointer that points to the same
   * object as this MasterPointer but doesn't take ownership. This is useful
   * when you need to pass a reference to the object without transferring
   * ownership.
   *
   * @return A ReferencePointer to the managed object.
   */
  ReferencePointer<T> reference() { return ReferencePointer<T>(mRawPointer); }

  /**
   * @brief Dereference operator.
   * @return Reference to the managed object.
   * @throws NullReferenceException if the pointer is null.
   */
  Ref operator*() {
    if (!mRawPointer) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return *mRawPointer;
  }

  /**
   * @brief Const dereference operator.
   * @return Const reference to the managed object.
   * @throws NullReferenceException if the pointer is null.
   */
  CRef operator*() const {
    if (!mRawPointer) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return *mRawPointer;
  }

  /**
   * @brief Member access operator.
   * @return Raw pointer to the managed object.
   * @throws NullReferenceException if the pointer is null.
   */
  Ptr operator->() {
    if (!mRawPointer) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return mRawPointer;
  }

  /**
   * @brief Const member access operator.
   * @return Const raw pointer to the managed object.
   * @throws NullReferenceException if the pointer is null.
   */
  CPtr operator->() const {
    if (!mRawPointer) {
      str msg = "Resource type: " + std::string(typeid(T).name()) + " is null";
      throw Exception::NullReferenceException(msg);
    }
    return mRawPointer;
  }

  /**
   * @brief Assignment operator for raw pointers.
   * @param pointer Rvalue reference to a raw pointer.
   * @return Reference to this object.
   */
  MasterPointer &operator=(T *&&pointer) {
    if (mRawPointer != pointer) {
      this->reset();
      mRawPointer = pointer;
      mPointer = std::move(Pointer(pointer));
    }
    return *this;
  }
  /**
   * @brief Assignment operator for nullptr.
   * @param nullptr_t Standard nullptr parameter.
   * @return Reference to this object.
   */
  MasterPointer &operator=(std::nullptr_t) {
    this->reset();
    mRawPointer = nullptr;
    return *this;
  }

  /**
   * @brief Boolean conversion operator.
   * @return true if the pointer is non-null, false otherwise.
   */
  operator bool() const { return mRawPointer != nullptr; }

public:
  /**
   * @brief Factory method to create a MasterPointer managing a newly
   * constructed object.
   * @tparam Args Type of constructor arguments.
   * @param args Arguments to forward to the constructor of T.
   * @return A MasterPointer that owns the newly created object.
   */
  template <typename... Args> static MasterPointer<T> create(Args &&...args) {
    return MasterPointer<T>(new T(std::forward<Args>(args)...));
  }
};

} // namespace Terreate::Util::Pointer
