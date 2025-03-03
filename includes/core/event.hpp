#ifndef __TERREATECORE_EVENT_HPP__
#define __TERREATECORE_EVENT_HPP__

#include <core/uuid.hpp>
#include <mutex>
#include <types.hpp>

namespace Terreate::Core {
using namespace Terreate::Types;

template <typename... Args> class Subscriber {
public:
  using Callback = Function<void(Args...)>;
  Callback mCallback;
  UUID mUUID;

public:
  Subscriber() : mCallback(nullptr), mUUID(UUID::Empty()) {}
  Subscriber(Callback callback) : mCallback(callback) {}
  Subscriber(Subscriber const &subscriber) {
    mCallback = subscriber.mCallback;
    mUUID = subscriber.mUUID;
  }
  virtual ~Subscriber() = default;

  virtual UUID const GetUUID() const { return mUUID; }
  virtual void OnEvent(Args... args) {
    if (mCallback != nullptr) {
      mCallback(std::forward<Args>(args)...);
    }
  }
};

template <typename... EventArgs> class Event final {
private:
  using Callback = Subscriber<EventArgs...>;
  typedef std::recursive_mutex mutex;

private:
  mutex mEventMutex;
  Map<UUID, Callback> mCallbacks;
  Map<UUID, Callback> mTriggers;

public:
  Event() = default;
  ~Event() = default;

  /*
   * @brief: Subscribe to the event
   * @param: subscriber: Callback function to be called when the event is
   */
  void Subscribe(Callback const &subscriber) {
    std::lock_guard<mutex> lock(mEventMutex);
    mCallbacks[subscriber.GetUUID()] = subscriber;
  }
  /*
   * @brief: Subscribe to the event as a trigger
   * @param: subscriber: Callback function to be called when the event is
   */
  void Trigger(Callback const &subscriber) {
    std::lock_guard<mutex> lock(mEventMutex);
    mTriggers[subscriber.GetUUID()] = subscriber;
  }
  /*
   * @brief: Unsubscribe from the event
   * @param: callback: Callback function to be removed from the event
   */
  void Unsubscribe(Callback const &callback) {
    std::unique_lock<mutex> lock(mEventMutex);
    for (auto it = mCallbacks.begin(); it != mCallbacks.end(); ++it) {
      if (it->first == callback.GetUUID()) {
        mCallbacks.erase(it);
        break;
      }
    }
  }

  /*
   * @brief: Publish the event
   * @param: args: Arguments to be passed to the callback functions
   */
  void Publish(EventArgs... args) {
    std::unique_lock<mutex> lock(mEventMutex);
    for (auto &[_, callback] : mCallbacks) {
      callback.OnEvent(std::forward<EventArgs>(args)...);
    }
    for (auto &[_, callback] : mTriggers) {
      callback.OnEvent(std::forward<EventArgs>(args)...);
    }
    mTriggers.clear();
  }

  Event &operator+=(Callback const &callback) {
    this->Subscribe(callback);
    return *this;
  }
  Event &operator-=(Callback const &callback) {
    this->Unsubscribe(callback);
    return *this;
  }
};

} // namespace Terreate::Core

#endif // __TERREATECORE_EVENT_HPP__
